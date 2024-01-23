#include <cstdio>
#include <ctime>
#include <chrono>
#include <cassert>

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
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

std::vector<Point> readPointsCSV(const std::string& fileName) {
    std::vector<Point> points;
    std::string line;
    std::ifstream input("C:/USers/Giulia/CLionProjects/ParallelProgramming/inputs/" + fileName, std::ios::in);
    if (input.is_open()) {
        int id = 0;
        while (std::getline(input, line)) {
            Point p(id, line, true);
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

double testKMeansOMPTime(int K, int epochs, const std::string& output_dir,
                         const std::vector<Point>& points, int nThreads) {
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();

    KMeansOMP kMeansOmp(K, epochs, output_dir);

    t1 = high_resolution_clock::now();
    kMeansOmp.run(points, nThreads);
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
    kMeans.run(points);
    t2 = high_resolution_clock::now();
    duration<double, std::milli> time1 = t2 - t1;
    timeResults.push_back(time1.count());

    t1 = high_resolution_clock::now();
    kMeansOmp.run(points, nThreads);
    t2 = high_resolution_clock::now();
    duration<double, std::milli> time2 = t2 - t1;
    timeResults.push_back(time2.count());

    return timeResults;
}

void testsKMeans() {
    std::ofstream test1File;
    test1File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test1KM.csv", std::ios::out);
    if (test1File.is_open()) {
        std::vector<Point> points = readPointsTXT("xclara.txt");
        std::vector<double> results = compareTimeKMeans(10, 24, "C:/Users/Giulia/CLionProjects/ParallelProgramming/results", points, 16);
        test1File << results[0] << "," << results[1] << "\n";
        test1File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }

    std::ofstream test2File;
    test2File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test2KM.csv", std::ios::out);
    if (test2File.is_open()) {

        test2File.close();
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

std::vector<std::string> readVocabularyCSV(const std::string& fileName) {
    std::vector<std::string> vocabulary;
    std::string word, line;
    std::ifstream words("C:/Users/Giulia/CLionProjects/ParallelProgramming/inputs/" + fileName, std::ios::in);
    if (words.is_open()) {
        while(std::getline(words, line)) {
            std::stringstream s(line);
            getline(s, word, ',');
            vocabulary.push_back(word);
        }
        words.close();
    }
    else {
        std::cout << "Unable to open the file";
    }
    return vocabulary;
}

double testStringSearchTime(const std::vector<std::string>& vocabulary, const std::string& target, int threshold) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;

    double sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<std::string> results = stringSearchFM(vocabulary, target, threshold);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        sum += time.count();
    }

    return sum / 10;
}

double testStringSearchOMPTime(const std::vector<std::string>& vocabulary, const std::string& target,
                               int threshold, int nThreads) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;

    double sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<std::string> resultsPar = stringSearchFM_OMP(vocabulary, target, threshold, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time = t2 - t1;
        sum += time.count();
    }

    return sum / 10;
}

std::vector<double> compareTimeStringSearch(const std::vector<std::string>& vocabulary, const std::string& target,
                                            int threshold, int nThreads) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;
    std::vector<double> timeResults;

    double sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<std::string> resultsSeqFM = stringSearchFM(vocabulary, target, threshold);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time1 = t2 - t1;
        sum += time1.count();
        std::cout << "Full matrix sequential, time(ms): " << time1.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<std::string> resultsParFM = stringSearchFM_OMP(vocabulary, target, threshold, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time2 = t2 - t1;
        sum += time2.count();
        std::cout << "Full Matrix parallel, time(ms): " << time2.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<std::string> resultsParFM2 = stringSearchFM_OMP2(vocabulary, target, threshold, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time3 = t2 - t1;
        sum +=time3.count();
        std::cout << "Full Matrix string search parallel, time(ms): " << time3.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<std::string> resultsSeqMR = stringSearchMR(vocabulary, target, threshold);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time4 = t2 - t1;
        sum += time4.count();
        std::cout << "Matrix Row sequential, time(ms): " << time4.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<std::string> resultsParMR = stringSearchMR_OMP(vocabulary, target, threshold, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time5 = t2 - t1;
        sum += time5.count();
        std::cout << "Matrix Row parallel, time(ms): " << time5.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<std::string> resultsParMR2 = stringSearchMR_OMP2(vocabulary, target, threshold, nThreads);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time6 = t2 - t1;
        sum += time6.count();
        std::cout << "Matrix Row string search parallel, time(ms): " << time6.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    sum = 0.0;
    for (int i = 0; i < 10; i ++) {
        t1 = high_resolution_clock::now();
        std::vector<std::string> resultsSeqRec = stringSearchRec(vocabulary, target, threshold);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> time7 = t2 - t1;
        sum += time7.count();
        std::cout << "Recursive, time(ms): " << time7.count() << std::endl;
    }
    timeResults.push_back(sum / 10);

    return timeResults;
}

void testsEditDistance() {
    //compare times between 5 algorithms, target = "bout", threshold = 2, nThreads = 2
    std::ofstream test1File;
    test1File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test1ED.csv", std::ios::out);
    if (test1File.is_open()) {
        std::vector<std::string> vocabulary1 = readVocabularyTXT("5000-words.txt");
        std::cout << "5000 words, target = bout, threshold = 2, nThreads = 2" << std::endl;
        std::vector<double> results1 = compareTimeStringSearch(vocabulary1, "bout", 2, 2);
        test1File << 1;
        for (int i = 0; i < (int)results1.size(); i ++) {
            test1File << "," << results1[i];
        }
        test1File << "\n";
        //nThreads = 10, 100, 1000
        std::cout << std::endl << "5000 words, target = bout, threshold = 2, nThreads = 10" << std::endl;
        std::vector<double> results2 = compareTimeStringSearch(vocabulary1, "bout", 2, 10);
        test1File << 2;
        for (int i = 0; i < (int)results2.size(); i ++) {
            test1File << "," << results2[i];
        }
        test1File << "\n";
        std::cout << std::endl << "5000 words, target = bout, threshold = 2, nThreads = 100" << std::endl;
        std::vector<double> results3 = compareTimeStringSearch(vocabulary1, "bout", 2, 100);
        test1File << 3;
        for (int i = 0; i < (int)results3.size(); i ++) {
            test1File << "," << results3[i];
        }
        test1File << "\n";
        std::cout << std::endl << "5000 words, target = bout, threshold = 2, nThreads = 1000" << std::endl;
        std::vector<double> results4 = compareTimeStringSearch(vocabulary1, "bout", 2, 1000);
        test1File << 4;
        for (int i = 0; i < (int)results4.size(); i ++) {
            test1File << "," << results4[i];
        }
        test1File << "\n";
        //higher threshold
        std::cout << std::endl << "5000 words, target = bout, threshold = 4, nThreads = 10" << std::endl;
        std::vector<double> results5 = compareTimeStringSearch(vocabulary1, "bout", 4, 10);
        test1File << 5;
        for (int i = 0; i < (int)results5.size(); i ++) {
            test1File << "," << results5[i];
        }
        test1File << "\n";
        test1File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }


    //Test Full Matrix approach with different configurations (due to first results)
    std::ofstream test2File;
    test2File.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/Test2ED.csv", std::ios::out);
    if (test2File.is_open()) {
        //vocabulary con parole più lunghe, e target più lungo
        std::vector<std::string> vocabularyL = readVocabularyCSV("30000-words.csv");
        std::string target = "ethnographic";
        double result1 = testStringSearchTime(vocabularyL, target, 2);
        double result1OMP = testStringSearchOMPTime(vocabularyL, target, 2, 10);
        test2File << 1 << result1 << "," << result1OMP << "\n";
        double result2 = testStringSearchTime(vocabularyL, target, 1);
        double result2OMP = testStringSearchOMPTime(vocabularyL, target, 1, 10);
        test2File << 2 << result2 << "," << result2OMP << "\n";
        double result3 = testStringSearchTime(vocabularyL, target, 2);
        double result3OMP = testStringSearchOMPTime(vocabularyL, target, 2, 100);
        test2File << 3 << result3 << "," << result3OMP << "\n";
        double result4 = testStringSearchTime(vocabularyL, target, 2);
        double result4OMP = testStringSearchOMPTime(vocabularyL, target, 2, 1000);
        test2File << 4 << result4 << "," << result4OMP << "\n";
        test2File.close();
    }
    else {
        std::cout << "Error: unable to open the file" << std::endl;
    }
}

int main() {
    testsKMeans();


    testsEditDistance();
    return 0;

    //KMeans
    std::string output_dir = "C:/Users/Giulia/CLionProjects/ParallelProgramming/results";
    std::vector<Point> points;
    std::string line;
    std::ifstream input ("C:/Users/Giulia/CLionProjects/ParallelProgramming/inputs/data.txt");
    if (input.is_open()) {
        int id = 0;
        while (std::getline(input, line)) {
            Point p(id, line);
            points.push_back(p);
            id ++;
        }
        input.close();
        KMeans kMeans(3, 12, output_dir);
        kMeans.run(points);
        KMeansOMP kMeansOmp(3, 12, output_dir);
        kMeansOmp.run(points, 4);
    }
    else {
        std::cout << "Unable to open the file";
    }


    //EditDistance
    std::vector<std::string> vocabulary;
    std::string word;
    std::ifstream words ("C:/Users/Giulia/CLionProjects/ParallelProgramming/inputs/5000-words.txt");
    if (words.is_open()) {
        while (std::getline(words, word)) {
            vocabulary.push_back(word);
        }
        words.close();
        std::string target = "bout";
        std::cout << "Beginning string search" << std::endl << std::endl;
        std::vector<std::string> results = stringSearchFM(vocabulary, target, 2);
        std::vector<std::string> resultsPar = stringSearchFM_OMP(vocabulary, target, 2, 2);
        std::ofstream output;
        output.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/results/stringSearch.txt");
        if (output.is_open()) {
            std::cout << "Writing results" << std::endl;
            for (int i = 0; i < (int) results.size(); i++) {
                output << results[i] << " " << resultsPar[i] << std::endl;
            }
        }
        else {
            std::cout << "Error: unable to write results";
        }
    }
    else {
        std::cout << "Unable to open the file";
    }

    return 0;
}
