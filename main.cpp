#include <iostream>
#include <fstream>
#include <vector>
#include "KMeans.h"
#include "KMeansOMP.h"
#include "EditDistance.h"

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
        std::vector<std::string> results = stringSearch(vocabulary, target, 2);
        for (int i = 0; i < (int)results.size(); i ++) {
            std::cout << results[i] << std::endl;
        }
    }
    else {
        std::cout << "Unable to open the file";
    }

    return 0;
}
