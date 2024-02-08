#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "KMeans.h"
#include "KMeansOMP.h"
#include "EditDistance.h"
#include "EditDistanceOMP.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

std::vector<Point> readPointsTXT(const std::string& fileName) {
    std::vector<Point> points;
    std::string line;
    std::ifstream input ("C:/Users/Giulia/CLionProjects/ParallelProgramming/inputs/" + fileName, std::ios::in);
    if (input.is_open()) {
        int id = 0;
        while (std::getline(input, line)) {
            Point p(id, line);
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

double testKMeansTime(int K, int epochs, const std::string& output_dir, const std::vector<Point>& points) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();

    KMeans kMeans(K, epochs, output_dir);

    t1 = high_resolution_clock::now();
    kMeans.run(points, 111);
    t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;
    return time.count();
}

double testKMeansOMPTime(int K, int epochs, const std::string& output_dir, const std::vector<Point>& points,
                         int nThreads) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();

    KMeansOMP kMeansOmp(K, epochs, output_dir);

    t1 = high_resolution_clock::now();
    kMeansOmp.run(points, 111, nThreads);
    t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;
    return time.count();
}

std::vector<double> compareTimeKMeans(int K, int epochs, const std::string& output_dir,
                                      const std::vector<Point>& points, int nThreads) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();
    std::vector<double> timeResults;

    KMeans kMeans(K, epochs, output_dir);
    KMeansOMP kMeansOmp(K, epochs, output_dir);

    t1 = high_resolution_clock::now();
    kMeans.run(points, 111);
    t2 = high_resolution_clock::now();
    duration<double, std::milli> time1 = t2 - t1;
    timeResults.push_back(time1.count());

    t1 = high_resolution_clock::now();
    kMeansOmp.run(points, 111, nThreads);
    t2 = high_resolution_clock::now();
    duration<double, std::milli> time2 = t2 - t1;
    timeResults.push_back(time2.count());

    return timeResults;
}

void testKMeans() {
    //test1 3000 points better sequential, 19000 parallel
    std::ofstream test1File;
    test1File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test1KM.csv", std::ios::out);
    if (test1File.is_open()) {
        std::vector<Point> points = readPointsCSV("3000-2D.csv");
        std::vector<double> results = compareTimeKMeans(2, 10,
                                                        "C:/Users/Giulia/CLionProjects/ParallelProgramming"
                                                        "/results", points, 16);
        double speedUp1 = results[0] / results[1];
        test1File << speedUp1 << ",";
        std::vector<double> results2 = compareTimeKMeans(5, 15,
                                                         "C:/Users/Giulia/CLionProjects/ParallelProgramming"
                                                         "/results", points, 16);
        double speedUp2 = results2[0] / results2[1];
        test1File << speedUp2 << ",";
        std::vector<double> results3 = compareTimeKMeans(10, 24,
                                                         "C:/Users/Giulia/CLionProjects/ParallelProgramming"
                                                         "/results", points, 16);
        double speedUp3 = results3[0] / results3[1];
        test1File << speedUp3 << "\n";

        std::vector<Point> points2 = readPointsCSV("19000-8D.csv");
        points.erase(points.begin());
        std::vector<double> results4 = compareTimeKMeans(4, 20,
                                                        "C:/Users/Giulia/CLionProjects/ParallelProgramming"
                                                        "/results", points, 16);
        double speedUp4 = results4[0] / results4[1];
        test1File << speedUp4 << ",";
        std::vector<double> results5 = compareTimeKMeans(10, 25,
                                                         "C:/Users/Giulia/CLionProjects/ParallelProgramming"
                                                         "/results", points, 16);
        double speedUp5 = results5[0] / results5[1];
        test1File << speedUp5 << ",";
        std::vector<double> results6 = compareTimeKMeans(18, 30,
                                                         "C:/Users/Giulia/CLionProjects/ParallelProgramming"
                                                         "/results", points, 16);
        double speedUp6 = results6[0] / results6[1];
        test1File << speedUp6 << "\n";

        test1File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }


    //test2: 500k points, cambiare k e nThreads
    std::ofstream test2File;
    test2File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test2KM.csv", std::ios::out);
    if (test2File.is_open()) {
        std::vector<Point> points = readPointsCSV("500000-5D.csv", 5);
        points.erase(points.begin());

        double resultSeq1 = testKMeansTime(4, 30, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points);
        std::vector<double> resultsPar1;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testKMeansOMPTime(4, 30, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points, i);
            resultsPar1.push_back(resultSeq1 / resultPar);
        }

        double resultSeq2 = testKMeansTime(12, 30, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points);
        std::vector<double> resultsPar2;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testKMeansOMPTime(12, 30, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points, i);
            resultsPar2.push_back(resultSeq2 / resultPar);
        }

        double resultSeq3 = testKMeansTime(25, 30, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points);
        std::vector<double> resultsPar3;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testKMeansOMPTime(25, 30, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points, i);
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


    //test3: 4mln points, nThreads max, changing only K
    std::ofstream test3File;
    test3File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test3KM.csv", std::ios::out);
    if (test3File.is_open()) {
        std::vector<Point> points = readPointsCSV("4mln-3D.csv", 3);
        points.erase(points.begin());

        double resultSeq1 = testKMeansTime(4, 35, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points);
        double resultPar1 = testKMeansOMPTime(4, 35, "C:/User/Giulia/CLionProjects/ParallelProgramming/results", points, 16);
        double speedUp1 = resultSeq1 / resultPar1;

        double resultSeq2 = testKMeansTime(10, 35, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points);
        double resultPar2 = testKMeansOMPTime(10, 35, "C:/User/Giulia/CLionProjects/ParallelProgramming/results", points, 16);
        double speedUp2 = resultSeq2 / resultPar2;

        double resultSeq3 = testKMeansTime(20, 35, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points);
        double resultPar3 = testKMeansOMPTime(20, 35, "C:/User/Giulia/CLionProjects/ParallelProgramming/results", points, 16);
        double speedUp3 = resultSeq3 / resultPar3;

        double resultSeq4 = testKMeansTime(30, 35, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points);
        double resultPar4 = testKMeansOMPTime(30, 35, "C:/User/Giulia/CLionProjects/ParallelProgramming/results", points, 16);
        double speedUp4 = resultSeq4 / resultPar4;

        test3File << speedUp1 << "," << speedUp2 << "\n" << speedUp3 << "," << speedUp4 << "\n";

        test3File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }
}


std::vector<std::string> readVocabularyTXT(const std::string& fileName) {
    std::vector<std::string> vocabulary;
    std::string word;
    std::ifstream words ("C:/Users/Giulia/CLionProjects/ParallelProgramming/inputs/" + fileName, std::ios::in);
    if (words.is_open()) {
        while (std::getline(words, word)) {
            vocabulary.push_back(word);
        }
        words.close();
    }
    else {
        std::cout << "Unable to open the file";
    }
    return vocabulary;
}

std::vector<std::string> readVocabularyCSV(const std::string& fileName, bool check = false) {
    std::vector<std::string> vocabulary;
    std::string word, line;
    std::ifstream words("C:/Users/Giulia/CLionProjects/ParallelProgramming/inputs/" + fileName, std::ios::in);
    if (words.is_open()) {
        int i = 0;
        while(std::getline(words, line)) {
            std::stringstream s(line);
            getline(s, word, ',');
            if (check && std::stoi(word) == i) {
                getline(s, word, ',');
            }
            vocabulary.push_back(word);
            i ++;
        }
        words.close();
    }
    else {
        std::cout << "Unable to open the file";
    }
    return vocabulary;
}

double testStringSearchFMTime(const std::vector<std::string>& vocabulary, const std::string& target,
                              bool repeat = true) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;

    double sum = 0.0;
    int constraint = repeat ? 10 : 1;
    for (int i = 0; i < constraint; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<int> results = stringSearchFM(vocabulary, target);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        std::cout << "FM sequential, time:" << time.count() << std::endl;
        sum += time.count();
    }

    return sum / 10;
}

double testStringSearchFM_OMP2Time(const std::vector<std::string>& vocabulary, const std::string& target,
                                   int nThreads, bool repeat = true) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;

    double sum = 0.0;
    int constraint = repeat ? 10 : 0;
    for (int i = 0; i < constraint; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<int> resultsPar2 = stringSearchFM_OMP2(vocabulary, target, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        sum += time.count();
    }

    return sum / 10;
}

double testStringSearchFM_OMPTime(const std::vector<std::string>& vocabulary, const std::string& target,
                                  int nThreads, bool repeat = true) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();

    double sum = 0.0;
    int constraint = repeat ? 10 : 1;
    for (int i = 0; i < constraint; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<int> resultsPar = stringSearchFM_OMP(vocabulary, target, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        std::cout << "FM parallel, time: " << time.count() << std::endl;
        sum += time.count();
    }

    return sum / 10;
}

std::vector<double> compareTimeStringSearch(const std::vector<std::string>& vocabulary, const std::string& target,
                                            int nThreads) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();
    std::vector<double> timeResults;

    double sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<int> resultsSeqFM = stringSearchFM(vocabulary, target);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time1 = t2 - t1;
        sum += time1.count();
        std::cout << "Full matrix sequential, time(ms): " << time1.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<int> resultsParFM = stringSearchFM_OMP(vocabulary, target, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time2 = t2 - t1;
        sum += time2.count();
        std::cout << "Full Matrix parallel, time(ms): " << time2.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<int> resultsParFM2 = stringSearchFM_OMP2(vocabulary, target, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time3 = t2 - t1;
        sum +=time3.count();
        std::cout << "Full Matrix string search parallel, time(ms): " << time3.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<int> resultsSeqMR = stringSearchMR(vocabulary, target);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time4 = t2 - t1;
        sum += time4.count();
        std::cout << "Matrix Row sequential, time(ms): " << time4.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<int> resultsParMR = stringSearchMR_OMP(vocabulary, target, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time5 = t2 - t1;
        sum += time5.count();
        std::cout << "Matrix Row parallel, time(ms): " << time5.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<int> resultsParMR2 = stringSearchMR_OMP2(vocabulary, target, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time6 = t2 - t1;
        sum += time6.count();
        std::cout << "Matrix Row string search parallel, time(ms): " << time6.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    return timeResults;
}

void testEditDistance() {
    //Test1: 5000 words better sequential, 30000 parallel(FM)
    std::ofstream test1File;
    test1File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test1ED.csv", std::ios::out);
    if (test1File.is_open()) {
        std::vector<std::string> vocabulary1 = readVocabularyTXT("5000-words.txt");
        std::vector<double> results1 = compareTimeStringSearch(vocabulary1, "more", 16);
        std::vector<double> results2 = compareTimeStringSearch(vocabulary1, "government", 16);

        test1File << results1[0];
        for (int i = 1; i < (int)results1.size(); i ++) {
            test1File << "," << results1[i];
        }
        test1File << "\n" << results2[0];
        for (int i = 1; i < (int)results2.size(); i ++) {
            test1File << "," << results2[i];
        }

        std::vector<std::string> vocabulary2 = readVocabularyCSV("30000-words.csv");
        std::vector<double> results3 = compareTimeStringSearch(vocabulary2, "more", 16);
        std::vector<double> results4 = compareTimeStringSearch(vocabulary2, "government", 16);

        test1File << "\n" << results3[0];
        for (int i = 1; i < (int)results3.size(); i ++) {
            test1File << "," << results3[i];
        }
        test1File << "\n" << results4[0];
        for (int i = 1; i < (int)results4.size(); i ++) {
            test1File << "," << results4[i];
        }
        test1File << "\n";

        test1File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }


    //Test2: 3 different targets varying nThreads, with 30000 words
    std::ofstream test2File;
    test2File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test2ED.csv", std::ios::out);
    if (test2File.is_open()) {
        std::vector<std::string> vocabulary = readVocabularyCSV("30000-words.csv");

        std::string target1 = "aid";
        double resultSeq1 = testStringSearchFMTime(vocabulary, target1);
        std::vector<double> resultsPar1;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testStringSearchFM_OMPTime(vocabulary, target1, i);
            resultsPar1.push_back(resultSeq1 / resultPar);
        }

        std::string target2 = "diverse";
        double resultSeq2 = testStringSearchFMTime(vocabulary, target2);
        std::vector<double> resultsPar2;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testStringSearchFM_OMPTime(vocabulary, target2, i);
            resultsPar2.push_back(resultSeq2 / resultPar);
        }

        std::string target3 = "interpretational";
        double resultSeq3 = testStringSearchFMTime(vocabulary, target3);
        std::vector<double> resultsPar3;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testStringSearchFM_OMPTime(vocabulary, target3, i);
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


    //Test3: 3 different targets varying nThreads, with 370000 words
    std::ofstream test3File;
    test3File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test3ED.csv", std::ios::out);
    if (test3File.is_open()) {
        std::vector<std::string> vocabulary = readVocabularyCSV("370000-words.csv", true);

        std::string target1 = "bot";
        double resultSeq1 = testStringSearchFMTime(vocabulary, target1, false);
        std::vector<double> resultsPar1;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testStringSearchFM_OMPTime(vocabulary, target1, i, false);
            resultsPar1.push_back(resultSeq1 / resultPar);
        }

        std::string target2 = "misqualify";
        double resultSeq2 = testStringSearchFMTime(vocabulary, target2, false);
        std::vector<double> resultsPar2;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testStringSearchFM_OMPTime(vocabulary, target2, i, false);
            resultsPar2.push_back(resultSeq2 / resultPar);
        }

        std::string target3 = "rhinolaryngology";
        double resultSeq3 = testStringSearchFMTime(vocabulary, target3, false);
        std::vector<double> resultsPar3;
        for (int i = 2; i < 17; i += 2) {
            double resultPar = testStringSearchFM_OMPTime(vocabulary, target3, i, false);
            resultsPar3.push_back(resultSeq3 / resultPar);
        }

        test3File << resultsPar1[0];
        for (int i = 1; i < (int)resultsPar1.size(); i ++) {
            test3File << "," << resultsPar1[i];
        }
        test3File << "\n" << resultsPar2[0];
        for (int i = 1; i < (int)resultsPar2.size(); i ++) {
            test3File << "," << resultsPar2[i];
        }
        test3File << "\n" << resultsPar3[0];
        for (int i = 1; i < (int)resultsPar3.size(); i ++) {
            test3File << "," << resultsPar3[i];
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

    //testEditDistance();

    return 0;
}