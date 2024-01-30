import queue


class DataLoader:
    def __init__(self, dataset, collate_fn, batch_size = 64):
        self.dataset = dataset
        self.batch_size = batch_size
        self.collate_fn = collate_fn
        self.index = 0

    def __iter__(self):
        self.index = 0
        return self

    def __next__(self):
        if self.index >= len(self.dataset):
            #stop iteration once index is out of bounds
            raise StopIteration
        batch_size = min(len(self.dataset) - self.index, self.batch_size)
        return self.collate_fn([self.get() for _ in range(batch_size)])

    def get(self):
        item = self.dataset[self.index]
        self.index += 1
        return item

    def worker_fn(dataset, index_queue, output_queue):
        while True:
            try:
                index = index_queue.get(timeout = 0)
            except queue.Empty:
                continue
            if index is None:
                break
            output_queue.put((index, dataset[index]))