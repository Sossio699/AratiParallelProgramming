import time
import sys
import pandas
import joblib

import DataLoader
import DataLoaderPar


def testDataLoaderW():
    #dataset1 = pandas.read_csv("D:/Programmi/PyCharmProject/DataLoader/input/dataset1.csv")
    dataset1 = list(range(200000))

    dataloader = DataLoader.DataLoader(dataset1, batch_size = 1000)
    startSeq = time.time()
    for batch in dataloader:
        time.sleep(0.2)
    endSeq = time.time()
    resultSeq = endSeq - startSeq

    resultsPar1 = []
    for i in range(1, 9, 1):
        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset1, batch_size = 1000, num_workers = i)
        startPar = time.time()
        for batch in dataloaderPar:
            time.sleep(0.2)
        endPar = time.time()
        resultPar = endPar - startPar
        resultsPar1.append(resultSeq / resultPar)

    resultsJL1 = []
    for i in range(1, 9, 1):
        dataloader = DataLoader.DataLoader(dataset1, batch_size = 1000)
        startJL = time.time()
        joblib.Parallel(n_jobs = i, prefer = "threads")(joblib.delayed(time.sleep)(0.2) for batch in dataloader)
        endJL = time.time()
        resultJL = endJL - startJL
        resultsJL1.append(resultSeq / resultJL)


    #dataset2 = pandas.read_csv("D:/Programmi/PyCharmProject/DataLoader/input/dataset2.csv")
    dataset2 = list(range(200000))

    dataloader2 = DataLoader.DataLoader(dataset2, batch_size = 1000)
    startSeq = time.time()
    for batch in dataloader2:
        time.sleep(0.8)
    endSeq = time.time()
    resultSeq2 = endSeq - startSeq

    resultsPar2 = []
    for i in range(1, 9, 1):
        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset2, batch_size = 1000, num_workers = i)
        startPar = time.time()
        for batch in dataloaderPar:
            time.sleep(0.8)
        endPar = time.time()
        resultPar = endPar - startPar
        resultsPar2.append(resultSeq2 / resultPar)

    resultsJL2 = []
    for i in range(1, 9, 1):
        dataloader = DataLoader.DataLoader(dataset2, batch_size = 1000)
        startJL = time.time()
        joblib.Parallel(n_jobs = i, prefer = "threads")(joblib.delayed(time.sleep)(0.8) for batch in dataloader)
        endJL = time.time()
        resultJL = endJL - startJL
        resultsJL2.append(resultSeq / resultJL)


    #dataset3 = pandas.read_csv("D:/Programmi/PyCharmProject/DataLoader/input/dataset3.csv")
    dataset3 = list(range(200000))

    dataloader3 = DataLoader.DataLoader(dataset3, batch_size = 1000)
    startSeq = time.time()
    for batch in dataloader3:
        time.sleep(2.5)
    endSeq = time.time()
    resultSeq3 = endSeq - startSeq

    resultsPar3 = []
    for i in range(1, 9, 1):
        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset3, batch_size = 1000, num_workers = i)
        startPar = time.time()
        for batch in dataloaderPar:
            time.sleep(2.5)
        endPar = time.time()
        resultPar = endPar - startPar
        resultsPar3.append(resultSeq3 / resultPar)

    resultsJL3 = []
    for i in range(1, 9, 1):
        dataloader = DataLoader.DataLoader(dataset3, batch_size = 1000)
        startJL = time.time()
        joblib.Parallel(n_jobs = i, prefer = "threads")(joblib.delayed(time.sleep)(2.5) for batch in dataloader)
        endJL = time.time()
        resultJL = endJL - startJL
        resultsJL3.append(resultSeq / resultJL)


    #dataset4 = pandas.read_csv("D:/Programmi/PyCharmProject/input/dataset4.csv")
    dataset4 = list(range(200000))

    dataloader4 = DataLoader.DataLoader(dataset4, batch_size = 1000)
    startSeq = time.time()
    for batch in dataloader4:
        time.sleep(4)
    endSeq = time.time()
    resultSeq4 = endSeq - startSeq

    resultsPar4 = []
    for i in range(1, 9, 1):
        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset4, batch_size = 1000, num_workers = i)
        startPar = time.time()
        for batch in dataloaderPar:
            time.sleep(4)
        endPar = time.time()
        resultPar = endPar - startPar
        resultsPar4.append(resultSeq4 / resultPar)

    resultsJL4 = []
    for i in range(1, 9, 1):
        dataloader = DataLoader.DataLoader(dataset4, batch_size = 1000)
        startJL = time.time()
        joblib.Parallel(n_jobs = i, prefer = "threads")(joblib.delayed(time.sleep)(4) for batch in dataloader)
        endJL = time.time()
        resultJL = endJL - startJL
        resultsJL4.append(resultSeq / resultJL)


    speedUps = pandas.DataFrame(data = [resultsPar1, resultsJL1, resultsPar2, resultsJL2,
                                      resultsPar3, resultsJL3, resultsPar4, resultsJL4],
                                columns = ["1 worker", "2 workers", "3 workers", "4 workers", "5 workers", "6 workers",
                                           "7 workers", "8 workers"])
    speedUps.index = ["SpeedUpPar0.2", "SpeedUpJL0.2", "SpeedUpPar0.8", "SpeedUpJL0.8", "SpeedUpPar2.5",
                      "SpeedUpJL2.5", "SpeedUpPar4", "SpeedUpJL4"]
    speedUps.to_csv("D:/Programmi/PyCharmProject/DataLoader/results/test1DL.csv")


def testDataLoaderB():
    sizes = [10, 100, 500, 1000, 5000, 10000]
    results = []
    # dataset1 = pandas.read_csv("D:/Programmi/PyCharmProject/DataLoader/input/dataset1.csv")
    dataset1 = list(range(200000))

    for size in sizes:
        dataloader = DataLoader.DataLoader(dataset1, batch_size = size)
        startSeq = time.time()
        for batch in dataloader:
            time.sleep(0.8)
        endSeq = time.time()
        resultSeq = endSeq - startSeq

        dataloaderPar = DataLoaderPar.DataLoaderPar(dataset1, batch_size = size, num_workers = 2)
        startPar = time.time()
        for batch in dataloaderPar:
            time.sleep(0.8)
        endPar = time.time()
        resultPar = endPar - startPar

        dataloader = DataLoader.DataLoader(dataset1, batch_size = size)
        startJL = time.time()
        joblib.Parallel(n_jobs = 2, prefer = "threads")(joblib.delayed(time.sleep)(0.8) for batch in dataloader)
        endJL = time.time()
        resultJL = endJL

        results.append([resultSeq / resultPar, resultSeq / resultJL])

    speedUps = pandas.DataFrame(data = results,
                                columns = ["SpeedUp Par", "SpeedUp JL"])
    speedUps.index = ["bs = 10", "bs = 100", "bs = 500", "bs = 1000", "bs = 5000", "bs = 10000"]
    speedUps.to_csv("D:/Programmi/PyCharmProject/DataLoader/results/test2DL.csv")


def main():

    #testDataLoaderW()
    testDataLoaderB()

    sys.exit(0)


if __name__ == "__main__":
    main()
