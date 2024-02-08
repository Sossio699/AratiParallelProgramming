import numpy as np
import random


class DataLoader:
    def __init__(self, dataset, batch_size = 1, shuffle = False):
        self.dataset = dataset
        self.batch_size = batch_size
        self.shuffle = shuffle
        self.index = 0 #next index that needs to be loaded

    def __iter__(self):
        self.index = 0
        return self

    def __next__(self):
        if self.index >= len(self.dataset):
            raise StopIteration
        batch_size = min(len(self.dataset) - self.index, self.batch_size)
        batch = np.array([self.get() for _ in range(batch_size)])
        if self.shuffle:
            random.shuffle(batch)
        return batch

    def get(self):
        item = self.dataset[self.index]
        self.index += 1
        return item