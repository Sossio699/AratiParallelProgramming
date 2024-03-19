import cv2

class Dataset:
    def __init__(self, size, im_paths):
        self.size = size
        self.im_paths = im_paths

    def __len__(self):
        return self.size

    def __getitem__(self, index):
        image = cv2.imread(str(self.im_paths[index]))
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        return image
