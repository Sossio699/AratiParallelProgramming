#include <iostream>
#include <fstream>
#include <vector>
#include "KMeans.h"

int main() {
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
        KMeans kmeans(2, 10, output_dir);
        kmeans.run(points);
    }
    else {
        std::cout << "Unable to open the file";
    }

    return 0;
}
