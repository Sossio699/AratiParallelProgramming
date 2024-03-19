import time
import sys
import pandas
import pathlib
import albumentations
import albumentations.augmentations.geometric.resize as resize

import DataLoader
import DataLoaderPar
import Dataset


#test different number of workers and albumentations with fixed batch_size of 300
def testDataLoaderWT():
    image_paths = pathlib.Path("D:/Programmi/PyCharmProject/DataLoader/input/images").glob('**/**/*.jpg')
    im_sorted = sorted([x for x in image_paths])

    dataset = Dataset.Dataset(len(im_sorted), im_sorted)

    n_workers = [1, 2, 4, 6, 8]


    #first albumentations
    transform1 = albumentations.Compose([
        resize.Resize(height = 256, width = 256, p = 1),
        albumentations.RandomCrop(width = 200, height = 200, p = 1)
    ])

    print("Sequential DataLoader, transform1")
    dataloader = DataLoader.DataLoader(dataset, batch_size = 300)
    startSeq = time.time()
    for batch in dataloader:
        for image in batch:
            transformed = transform1(image = image)['image']
    endSeq = time.time()
    resultSeq1 = endSeq - startSeq

    print("Parallel DataLoader, transform1, workers from 1 to 8")
    resultsPar1 = []
    for i in n_workers:
        print("N_workers: ", i)
        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset, batch_size = 300, num_workers = i)
        startPar = time.time()
        for batch in dataloaderPar:
            for image in batch:
                transformed = transform1(image = image)['image']
        endPar = time.time()
        resultPar = endPar - startPar
        print("SpeedUp: ", resultSeq1 / resultPar)
        resultsPar1.append(resultSeq1 / resultPar)


    #second albumentations
    transform2 = albumentations.Compose([
        resize.Resize(height=256, width=256, p=1),
        albumentations.HorizontalFlip(p=1),
        albumentations.RandomCrop(width=200, height=200, p=1),
        albumentations.RandomBrightnessContrast(p=1),
        albumentations.VerticalFlip(p=1)
    ])

    print("Sequential DataLoader, transform2")
    dataloader = DataLoader.DataLoader(dataset, batch_size = 300)
    startSeq = time.time()
    for batch in dataloader:
        for image in batch:
            transformed = transform2(image = image)['image']
    endSeq = time.time()
    resultSeq2 = endSeq - startSeq

    print("Parallel DataLoader, transform2, workers from 1 to 8")
    resultsPar2 = []
    for i in n_workers:
        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset, batch_size = 300, num_workers = i)
        startPar = time.time()
        for batch in dataloaderPar:
            for image in batch:
                transformed = transform2(image = image)['image']
        endPar = time.time()
        resultPar = endPar - startPar
        print("SpeedUp: ", resultSeq2 / resultPar)
        resultsPar2.append(resultSeq2 / resultPar)


    #third albumentations
    transform3 = albumentations.Compose([
        resize.Resize(height=256, width=256, p=1),
        albumentations.HorizontalFlip(p=1),
        albumentations.RandomCrop(width=200, height=200, p=1),
        albumentations.RandomBrightnessContrast(p=1),
        albumentations.VerticalFlip(p=1),
        resize.Resize(height=280, width=280, p=1),
        albumentations.RandomCrop(width = 200, height=200, p=1)
    ])

    print("Sequential DataLoader, transform3")
    dataloader = DataLoader.DataLoader(dataset, batch_size = 300)
    startSeq = time.time()
    for batch in dataloader:
        for image in batch:
            transformed = transform3(image = image)['image']
    endSeq = time.time()
    resultSeq3 = endSeq - startSeq

    print("Parallel DataLoader, transform3, workers from 1 to 8")
    resultsPar3 = []
    for i in n_workers:
        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset, batch_size = 300, num_workers = i)
        startPar = time.time()
        for batch in dataloaderPar:
            for image in batch:
                transformed = transform3(image = image)['image']
        endPar = time.time()
        resultPar = endPar - startPar
        print("SpeedUp: ", resultSeq3 / resultPar)
        resultsPar3.append(resultSeq3 / resultPar)


    #fourth albumentations
    transform4 = albumentations.Compose([
        resize.Resize(height=256, width=256, p=1),
        albumentations.HorizontalFlip(p=1),
        albumentations.RandomCrop(width=200, height=200, p=1),
        albumentations.RandomBrightnessContrast(p=1),
        albumentations.VerticalFlip(p=1),
        resize.Resize(height=280, width=280, p=1),
        albumentations.RandomCrop(width = 200, height=200, p=1),
        albumentations.HorizontalFlip(p=1),
        albumentations.RandomBrightnessContrast(p=1),
        albumentations.VerticalFlip(p=1),
        resize.Resize(height=280, width=280, p=1)
    ])

    print("Sequential DataLoader, transform4")
    dataloader = DataLoader.DataLoader(dataset, batch_size = 300)
    startSeq = time.time()
    for batch in dataloader:
        for image in batch:
            transformed = transform4(image = image)['image']
    endSeq = time.time()
    resultSeq4 = endSeq - startSeq

    print("Parallel DataLoader, transform4, workers from 1 to 8")
    resultsPar4 = []
    for i in n_workers:
        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset, batch_size = 300, num_workers = i)
        startPar = time.time()
        for batch in dataloaderPar:
            for image in batch:
                transformed = transform4(image = image)['image']
        endPar = time.time()
        resultPar = endPar - startPar
        print("SpeedUp: ", resultSeq4 / resultPar)
        resultsPar4.append(resultSeq4 / resultPar)


    speedUps = pandas.DataFrame(data = [resultsPar1, resultsPar2, resultsPar3, resultsPar4],
                                columns = ["1 worker", "2 workers", "4 workers", "6 workers", "8 workers"])
    speedUps.index = ["SpeedUpPar1", "SpeedUpPar2", "SpeedUpPar3", "SpeedUpPar4"]
    speedUps.to_csv("D:/Programmi/PyCharmProject/DataLoader/results/test1DL.csv")


#test different batch sizes with fixed albumentations and num_workers
def testDataLoaderB():
    sizes = [10, 50, 100, 300, 500, 1000]
    results = []

    image_paths = pathlib.Path("D:/Programmi/PyCharmProject/DataLoader/input/images").glob('**/**/*.jpg')
    im_sorted = sorted([x for x in image_paths])

    dataset = Dataset.Dataset(len(im_sorted), im_sorted)

    transform = albumentations.Compose([
        resize.Resize(height=256, width=256, p=1),
        albumentations.HorizontalFlip(p=1),
        albumentations.RandomCrop(width=200, height=200, p=1),
        albumentations.RandomBrightnessContrast(p=1),
        albumentations.VerticalFlip(p=1)
    ])

    for size in sizes:
        print("Sequential DataLoader, batch_size: ", size)
        dataloader = DataLoader.DataLoader(dataset, batch_size = size)
        startSeq = time.time()
        for batch in dataloader:
            for image in batch:
                transformed = transform(image = image)['image']
        endSeq = time.time()
        resultSeq = endSeq - startSeq

        print("Parallel DataLoader, batch_size: ", size)
        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset, batch_size = size, num_workers = 2)
        startPar = time.time()
        for batch in dataloaderPar:
            for image in batch:
                transformed = transform(image = image)['image']
        endPar = time.time()
        resultPar = endPar - startPar

        print("SpeedUp: ", resultSeq / resultPar)
        results.append([resultSeq / resultPar])

    speedUps = pandas.DataFrame(data = results,
                                columns = ["SpeedUp Par"])
    speedUps.index = ["bs = 10", "bs = 50", "bs = 100", "bs = 300", "bs = 500", "bs = 1000"]
    speedUps.to_csv("D:/Programmi/PyCharmProject/DataLoader/results/test2DL.csv")


def main():

    testDataLoaderWT()
    testDataLoaderB()

    sys.exit(0)


if __name__ == "__main__":
    main()
