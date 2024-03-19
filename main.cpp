#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include "KMeans.h"
#include "KMeansOMP.h"
#include "EditDistance.h"
#include "EditDistanceOMP.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


//test KMeans
std::vector<Point> readPointsCSV(const std::string& fileName, int stop = 1000) {
    std::vector<Point> points;
    std::string line;
    std::ifstream input("C:/USers/Giulia/CLionProjects/ParallelProgramming/inputs/" + fileName, std::ios::in);
    if (input.is_open()) {
        int id = 0;
        while (std::getline(input, line)) {
            Point p(id, line, true, stop);
            points.push_back(p);
            id ++;
        }
        input.close();
    }
    else {
        std::cout << "Unable to open the file";
    }
    return points;
}

double testKMeansTime(int K, int epochs, const std::vector<Point>& points) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();

    KMeans kMeans(K, epochs);

    t1 = high_resolution_clock::now();
    kMeans.run(points, 111);
    t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;
    return time.count();
}

double testKMeansOMPTime(int K, int epochs, const std::vector<Point>& points, int nThreads) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();

    KMeansOMP kMeansOmp(K, epochs);

    t1 = high_resolution_clock::now();
    kMeansOmp.run(points, 111, nThreads);
    t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;
    return time.count();
}

void testKMeans() {
    //test1: understanding number of points to use KMeans
    std::ofstream test1File;
    test1File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test1KM.csv", std::ios::out);
    if (test1File.is_open()) {
        //3000 points 2D
        std::vector<Point> points1 = readPointsCSV("3000-2D.csv");
        std::vector<double> resultSeq;
        std::vector<double> resultPar;

        resultSeq.push_back(testKMeansTime(3, 30, points1));
        resultPar.push_back(testKMeansOMPTime(3, 30, points1, 4));
        resultPar.push_back(testKMeansOMPTime(3, 30, points1, 8));

        resultSeq.push_back(testKMeansTime(6, 30, points1));
        resultPar.push_back(testKMeansOMPTime(6, 30, points1, 4));
        resultPar.push_back(testKMeansOMPTime(6, 30, points1, 8));

        //19000 points 8D
        std::vector<Point> points2 = readPointsCSV("19000-8D.csv");
        resultSeq.push_back(testKMeansTime(5, 30, points2));
        resultPar.push_back(testKMeansOMPTime(5, 30, points2, 4));
        resultPar.push_back(testKMeansOMPTime(5, 30, points2, 8));

        resultSeq.push_back(testKMeansTime(8, 30, points2));
        resultPar.push_back(testKMeansOMPTime(8, 30, points2, 4));
        resultPar.push_back(testKMeansOMPTime(8, 30, points2, 8));

        test1File << resultSeq[0] / resultPar[0];
        int j = 0;
        for (int i = 1; i < (int)resultPar.size(); i ++) {
            if (i % 2 == 0) {
                j ++;
            }
            if (i % 4 == 0) {
                test1File << resultSeq[j] / resultPar[i];
            }
            else {
                test1File << "," << resultSeq[j] / resultPar[i];
            }
            if ((i + 1) % 4 == 0) {
                test1File << "\n";
            }
        }

        test1File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }


    //test2: 500k points with 5D, changing nThreads and K
    std::ofstream test2File;
    test2File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test2KM.csv", std::ios::out);
    if (test2File.is_open()) {
        std::vector<Point> points = readPointsCSV("500000-5D.csv", 5);
        points.erase(points.begin());

        double resultSeq1 = testKMeansTime(4, 30, points);
        std::vector<double> resultsPar1;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testKMeansOMPTime(4, 30, points, i);
            resultsPar1.push_back(resultSeq1 / resultPar);
        }

        double resultSeq2 = testKMeansTime(12, 30, points);
        std::vector<double> resultsPar2;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testKMeansOMPTime(12, 30, points, i);
            resultsPar2.push_back(resultSeq2 / resultPar);
        }

        double resultSeq3 = testKMeansTime(25, 30, points);
        std::vector<double> resultsPar3;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testKMeansOMPTime(25, 30, points, i);
            resultsPar3.push_back(resultSeq3 / resultPar);
        }

        test2File << resultsPar1[0];
        for (int i = 1; i < (int)resultsPar1.size(); i ++) {
            test2File << "," << resultsPar1[i];
        }
        test2File << "\n" << resultsPar2[0];
        for (int i = 1; i < (int)resultsPar2.size(); i ++) {
            test2File << "," << resultsPar2[i];
        }
        test2File << "\n" << resultsPar3[0];
        for (int i = 1; i < (int)resultsPar3.size(); i ++) {
            test2File << "," << resultsPar3[i];
        }
        test2File << "\n";

        test2File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }

    //test3: 4mln points with fixed nThreads and varying K
    std::ofstream test3File;
    test3File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test3KM.csv", std::ios::out);
    if (test3File.is_open()) {
        std::vector<Point> points = readPointsCSV("4mln-3D.csv", 3);
        points.erase(points.begin());

        double resultSeq1 = testKMeansTime(4, 35, points);
        double resultPar1 = testKMeansOMPTime(4, 35, points, 8);
        double speedUp1 = resultSeq1 / resultPar1;

        double resultSeq2 = testKMeansTime(10, 35, points);
        double resultPar2 = testKMeansOMPTime(10, 35, points, 8);
        double speedUp2 = resultSeq2 / resultPar2;

        double resultSeq3 = testKMeansTime(20, 35, points);
        double resultPar3 = testKMeansOMPTime(20, 35, points, 8);
        double speedUp3 = resultSeq3 / resultPar3;

        double resultSeq4 = testKMeansTime(30, 35, points);
        double resultPar4 = testKMeansOMPTime(30, 35, points, 8);
        double speedUp4 = resultSeq4 / resultPar4;

        test3File << speedUp1 << "," << speedUp2 << "\n" << speedUp3 << "," << speedUp4 << "\n";

        test3File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }
}



//test EditDistance
std::string random_string(std::size_t length, int seed) {
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    generator.seed(seed);
    std::uniform_int_distribution<> distribution(0, (int)CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

//Sequential tests
double testStringSearchFMTime(const std::string& word1, const std::string& word2, bool repeat) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;

    double sum = 0.0;
    int constraint = repeat ? 10 : 1;
    for (int i = 0; i < constraint; i ++) {
        t1 = high_resolution_clock::now();
        int result = levenshteinDistFM(word1, word2, (int)word1.size(), (int)word2.size());
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        std::cout << "Full Matrix sequential, time:" << time.count() << ", result: " << result << std::endl;
        sum += time.count();
    }

    return sum / constraint;
}

double testStringSearchSDTime(const std::string& word1, const std::string& word2, bool repeat) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;

    double sum = 0.0;
    int constraint = repeat ? 10 : 1;
    for (int i = 0; i < constraint; i ++) {
        t1 = high_resolution_clock::now();
        int result = levenshteinDistSD(word1, word2, (int)word1.size(), (int)word2.size());
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        std::cout << "Skew Diagonal sequential, time:" << time.count() << ", result: " << result << std::endl;
        sum += time.count();
    }

    return sum / constraint;
}

double testStringSearchMRTime(const std::string& word1, const std::string& word2, bool repeat) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;

    double sum = 0.0;
    int constraint = repeat ? 10 : 1;
    for (int i = 0; i < constraint; i ++) {
        t1 = high_resolution_clock::now();
        int result = levenshteinDistMR(word1, word2, (int)word1.size(), (int)word2.size());
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        std::cout << "Matrix Row sequential, time:" << time.count() << ", result: " << result << std::endl;
        sum += time.count();
    }

    return sum / constraint;
}


//Parallel tests
double testStringSearchSD_OMPTime(const std::string& word1, const std::string& word2, int nThreads, bool repeat) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();

    double sum = 0.0;
    int constraint = repeat ? 10 : 1;
    for (int i = 0; i < constraint; i ++) {
        t1 = high_resolution_clock::now();
        int resultPar = levenshteinDistSD_OMP(word1, word2, (int)word1.size(), (int)word2.size(),
                                              nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        std::cout << "Skew Diagonal parallel, time: " << time.count() << ", result: " << resultPar << std::endl;
        sum += time.count();
    }

    return sum / constraint;
}

double testStringSearchSD_OMP2Time(const std::string& word1, const std::string& word2, int nThreads, bool repeat) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();

    double sum = 0.0;
    int constraint = repeat ? 10 : 1;
    for (int i = 0; i < constraint; i ++) {
        t1 = high_resolution_clock::now();
        int resultPar = levenshteinDistSD_OMP2(word1, word2, (int)word1.size(), (int)word2.size(),
                                              nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        std::cout << "Skew Diagonal parallel 2, time: " << time.count() << ", result: " << resultPar << std::endl;
        sum += time.count();
    }

    return sum / constraint;
}


std::vector<double> compareTimeStringSearch(const std::string& word1, const std::string& word2, int nThreads,
                                            bool repeat = true) {
    std::vector<double> timeResults;


    double resultsSeqFM = testStringSearchFMTime(word1, word2, repeat);
    std::cout << "Full Matrix sequential, average time(ms): " << resultsSeqFM << std::endl;
    timeResults.push_back(resultsSeqFM);

    double resultSeqSD = testStringSearchSDTime(word1, word2, repeat);
    std::cout << "Skew Diagonal sequential, average time(ms): " << resultSeqSD << std::endl;
    timeResults.push_back(resultSeqSD);

    double resultSeqMR = testStringSearchMRTime(word1, word2, repeat);
    std::cout << "Matrix Row sequential, average time(ms): " << resultSeqMR << std::endl;
    timeResults.push_back(resultSeqMR);

    double resultParSD = testStringSearchSD_OMPTime(word1, word2, nThreads, repeat);
    std::cout << "Skew Diagonal parallel, average time(ms): " << resultParSD << std::endl;
    timeResults.push_back(resultParSD);

    double resultParSD2 = testStringSearchSD_OMP2Time(word1, word2, nThreads, repeat);
    std::cout << "Skew Diagonal parallel 2, average time(ms): " << resultParSD2 << std::endl;
    timeResults.push_back(resultParSD2);


    std::vector<double> speedUps;

    //Skew Diagonal Parallel
    for (int i = 0; i < 3; i ++) {
        speedUps.push_back(timeResults[i] / timeResults[3]);
    }
    //Skew Diagonal Parallel 2
    for (int i = 0; i < 3; i ++) {
        speedUps.push_back(timeResults[i] / timeResults[4]);
    }

    return speedUps;
}

double compareTimeStringSearchSD(const std::string& word1, const std::string& word2, int nThreads,
                                              bool repeat = true) {
    std::vector<double> timeResults;


    double resultSeqSD = testStringSearchSDTime(word1, word2, repeat);
    std::cout << "Skew Diagonal sequential, average time(ms): " << resultSeqSD << std::endl;
    timeResults.push_back(resultSeqSD);

    double resultParSD = testStringSearchSD_OMPTime(word1, word2, nThreads, repeat);
    std::cout << "Skew Diagonal parallel, average time(ms): " << resultParSD << std::endl;
    timeResults.push_back(resultParSD);


    double speedUp = resultSeqSD / resultParSD;

    return speedUp;
}

void testEditDistance() {
    //Test1: length 5000 vs 15000 vs 25000, better approaches
    std::ofstream test1File;
    test1File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test1ED.csv", std::ios::out);
    if (test1File.is_open()) {
        std::string word1 = random_string(5000, 111);
        std::string word2 = random_string(5000, 11);
        std::vector<double> results = compareTimeStringSearch(word1, word2, 4);

        test1File << results[0];
        for (int i = 1; i < (int)results.size(); i ++) {
            if (i % 3 == 0) {
                test1File << results[i];
            }
            else {
                test1File << "," << results[i];
            }
            if ((i + 1) % 3 == 0) {
                test1File << "\n";
            }
        }

        std::string word3 = random_string(15000, 111);
        std::string word4 = random_string(15000, 11);
        std::vector<double> results2 = compareTimeStringSearch(word3, word4, 4);

        test1File << results2[0];
        for (int i = 1; i < (int)results2.size(); i ++) {
            if (i % 3 == 0) {
                test1File << results2[i];
            }
            else {
                test1File << "," << results2[i];
            }
            if ((i + 1) % 3 == 0) {
                test1File << "\n";
            }
        }

        std::string word5 = random_string(28000, 111);
        std::string word6 = random_string(28000, 11);
        std::vector<double> results3 = compareTimeStringSearch(word5, word6, 4);

        test1File << results3[0];
        for (int i = 1; i < (int)results3.size(); i ++) {
            if (i % 3 == 0) {
                test1File << results3[i];
            }
            else {
                test1File << "," << results3[i];
            }
            if ((i + 1) % 3 == 0) {
                test1File << "\n";
            }
        }

        test1File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }


    //Test2: Skew Diagonal Sequential vs Parallel varying length
    std::ofstream test2File;
    test2File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test2ED.csv", std::ios::out);
    if (test2File.is_open()) {
        std::vector<double> speedUps;

        std::string word1 = random_string(200, 111);
        std::string word2 = random_string(200, 11);
        double speedUp1 = compareTimeStringSearchSD(word1, word2, 4);
        speedUps.push_back(speedUp1);

        std::string word3 = random_string(3000, 111);
        std::string word4 = random_string(3000, 11);
        double speedUp2 = compareTimeStringSearchSD(word3, word4, 4);
        speedUps.push_back(speedUp2);

        std::string word5 = random_string(12000, 111);
        std::string word6 = random_string(12000, 11);
        double speedUp3 = compareTimeStringSearchSD(word5, word6, 4);
        speedUps.push_back(speedUp3);

        std::string word7 = random_string(20000, 111);
        std::string word8 = random_string(20000, 11);
        double speedUp4 = compareTimeStringSearchSD(word7, word8, 4);
        speedUps.push_back(speedUp4);

        std::string word9 = random_string(32000, 111);
        std::string word10 = random_string(32000, 11);
        double speedUp5 = compareTimeStringSearchSD(word9, word10, 4);
        speedUps.push_back(speedUp5);

        std::string word11 = random_string(45000, 111);
        std::string word12 = random_string(45000, 11);
        double speedUp6 = compareTimeStringSearchSD(word11, word12, 4);
        speedUps.push_back(speedUp6);

        test2File << speedUps[0];
        for (int i = 1; i < (int)speedUps.size(); i ++) {
            if (i % 3 == 0) {
                test2File << speedUps[i];
            }
            else {
                test2File << "," << speedUps[i];
            }
            if ((i + 1) % 3 == 0) {
                test2File << "\n";
            }
        }
        test2File << "\n";

        test2File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }


    //Test3: Skew Diagonal with length 20000, varying nThreads
    std::ofstream test3File;
    test3File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test3ED.csv", std::ios::out);
    if (test3File.is_open()) {
        std::vector<double> speedUps;

        std::string word1 = random_string(20000, 111);
        std::string word2 = random_string(20000, 11);

        for (int i = 2; i < 17; i += 2) {
            double speedUp = compareTimeStringSearchSD(word1, word2, i);
            speedUps.push_back(speedUp);
        }
        std::cout << "SpeedUps length: " << speedUps.size() << std::endl;

        test3File << speedUps[0];
        for (int i = 1; i < (int)speedUps.size(); i ++) {
            if (i % 4 == 0) {
                test3File << speedUps[i];
            }
            else {
                test3File << "," << speedUps[i];
            }
            if ((i + 1) % 4 == 0) {
                test3File << "\n";
            }
        }
        test3File << "\n";

        test3File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }
}


int main() {

    testKMeans();

    testEditDistance();

    return 0;
}