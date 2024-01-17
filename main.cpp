#include <cstdio>
#include <ctime>
#include <chrono>
#include <cassert>

#include <iostream>
#include <fstream>
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

double testStringSearchOMPTime(const std::vector<std::string>& vocabulary, const std::string& target,
                               int threshold, int nThreads) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;

    t1 = high_resolution_clock::now();
    std::vector<std::string> resultsPar = stringSearchOMP(vocabulary, target, threshold, nThreads);
    t2 = high_resolution_clock::now();

    duration<double, std::milli> time = t2 - t1;
    return time.count();
}

std::vector<double> compareTimeStringSearch(const std::vector<std::string>& vocabulary, const std::string& target,
                                            int threshold, int nThreads) {
    auto t1 = high_resolution_clock::now();
    auto t2 = t1;
    std::vector<double> timeResults;

    t1 = high_resolution_clock::now();
    std::vector<std::string> resultsSeq = stringSearch(vocabulary, target, threshold);
    t2 = high_resolution_clock::now();
    duration<double, std::milli> time1 = t2 - t1;
    timeResults.push_back(time1.count());

    t1 = high_resolution_clock::now();
    std::vector<std::string> resultsPar = stringSearchOMP(vocabulary, target, threshold, nThreads);
    t2 = high_resolution_clock::now();
    duration<double, std::milli> time2 = t2 - t1;
    timeResults.push_back(time2.count());

    return timeResults;
}


int main() {
    //KMeans
    std::string output_dir = "C:/Users/Giulia/CLionProjects/ParallelProgramming";
    std::vector<Point> points;
    std::string line;
    std::ifstream input ("C:/Users/Giulia/CLionProjects/ParallelProgramming/data.txt");
    if (input.is_open()) {
        int id = 0;
        while (std::getline(input, line)) {
            Point p(id, line);
            points.push_back(p);
            id ++;
        }
        input.close();
        KMeans kMeans(2, 10, output_dir);
        kMeans.run(points);
        KMeansOMP kMeansOmp(2, 10, output_dir);
        kMeansOmp.run(points, 2);
    }
    else {
        std::cout << "Unable to open the file";
    }

    //EditDistance
    std::vector<std::string> vocabulary;
    std::string word;
    std::ifstream words ("C:/Users/Giulia/CLionProjects/ParallelProgramming/5000-words.txt");
    if (words.is_open()) {
        while (std::getline(words, word)) {
            vocabulary.push_back(word);
        }
        words.close();
        std::string target = "bout";
        std::cout << "Beginning string search" << std::endl << std::endl;
        std::vector<std::string> results = stringSearch(vocabulary, target, 2);
        std::vector<std::string> resultsPar = stringSearchOMP(vocabulary, target, 2, 2);
        std::ofstream output;
        output.open("C:/Users/Giulia/CLionProjects/ParallelProgramming/stringSearch.txt");
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
