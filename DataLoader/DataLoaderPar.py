import itertools
import multiprocessing
import queue

import DataLoader


def worker_funct(dataset, index_queue, output_queue):
    while True:
        try:
            index = index_queue.get(timeout = 0)
        except queue.Empty:
            continue
        if index is None:
            break
        output_queue.put((index, dataset[index]))


class DataLoaderPar(DataLoader.DataLoader):
    def __init__(self, dataset, batch_size = 64, shuffle = False,
                 num_workers = 1, prefetch_batches = 2):
        super().__init__(dataset, batch_size, shuffle)

        self.num_workers = num_workers
        self.prefetch_batches = prefetch_batches
        self.output_queue = multiprocessing.Queue()
        self.index_queues = []
        self.workers = []
        self.worker_cycle = itertools.cycle(range(num_workers))
        self.cache = {}
        self.prefetch_index = 0

        for _ in range(num_workers):
            index_queue = multiprocessing.Queue()
            worker = multiprocessing.Process(target = worker_funct,
                                             args = (self.dataset, index_queue,
                                                     self.output_queue))
            worker.daemon = True
            worker.start()
            self.workers.append(worker)
            self.index_queues.append(index_queue)

        self.prefetch()

    def prefetch(self):
        while (self.prefetch_index < len(self.dataset) and
               self.prefetch_index < self.index + 2 * self.num_workers * self.batch_size) :
            # if the prefetch_index hasn't reached the end of the dataset &
            # it is not 2 batches ahead, add indexes to the index queues
            self.index_queues[next(self.worker_cycle)].put(self.prefetch_index) #cycles through workers
            self.prefetch_index += 1

    def get(self):
        self.prefetch()
        if self.index in self.cache:
            item = self.cache[self.index]
            del self.cache[self.index]
        else:
            while True:
                try:
                    (index, data) = self.output_queue.get(timeout = 0)
                except queue.Empty:
                    continue
                if index == self.index:
                    item = data
                    break
                else:
                    self.cache[index] = data

        self.index += 1
        return item

    def __iter__(self):
        self.index = 0
        self.cache = {}
        self.prefetch_index = 0
        self.prefetch()
        return self

    def __del__(self):
        try:
            for i, w in enumerate(self.workers):
                self.index_queues[i].put(None)
                w.join(timeout = 5.0)
            for q in self.index_queues:
                q.cancel_join_thread()
                q.close()
            self.output_queue.cancel_join_thread()
            self.output_queue.close()
        finally:
            for w in self.workers:
                if w.is_alive():
                    w.terminate()