//
// Created by Niccolò Arati on 08/01/2024.
//

#include "KMeans.h"
#include <cstdlib>
#include <cfloat>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

KMeans::KMeans(int K, int epochs, std::string output_dir) {
    this->K = K;
    this->epochs = epochs;
    this->output_dir = output_dir;
    dimensions = 0;
    nPoints = 0;
}

void KMeans::clearClusters() {
    for (int i = 0; i < K; i ++) {
        clusters[i].removeAllPoints();
    }
}

int KMeans::getNearestClusterId(Point p) {
    double sum, min_dist = DBL_MAX;
    int nearestClusterId;
    for (int i = 0; i < K; i ++) {
        double dist;
        sum = 0.0;
        if (dimensions == 1) {
            dist = fabs(clusters[i].getCentroidPos(0) - p.getVal(0));
        }
        else {
            for (int j = 0; j < dimensions; j ++) {
                sum += pow(clusters[i].getCentroidPos(j) - p.getVal(j), 2.0);
            }
            dist = sqrt(sum);
        }
        if (dist < min_dist) {
            min_dist = dist;
            nearestClusterId = clusters[i].getClusterId();
        }
    }
    return nearestClusterId;
}

void KMeans::run(std::vector<Point> allPoints) {
    nPoints = (int)allPoints.size();
    dimensions = allPoints[0].getDimensions();
    //initializing clusters
    std::vector<int> usedPointsIds;
    bool init;
    for (int i = 1; i <= K; i ++) {
        init = false;
        while(!init) {
            int index = rand() % nPoints;
            if (std::find(usedPointsIds.begin(), usedPointsIds.end(), index) == usedPointsIds.end()) {
                usedPointsIds.push_back(index);
                allPoints[index].setClusterId(i);
                Cluster cluster(i, allPoints[index]);
                clusters.push_back(cluster);
                init = true;
            }
        }
    }
    std::cout << "Clusters initialized = " << clusters.size() << std::endl << std::endl;
    std::cout << "Running K-Means clustering.." << std::endl;
    int epoch = 1;
    bool run = true;
    while(run) {
        std::cout << "Epoch " << epoch << " / " << epochs << std::endl;
        bool changed = false;
        //add all points to their nearest cluster
        for (int i = 0; i < nPoints; i ++) {
            int currentClusterId = allPoints[i].getClusterId();
            int nearestClusterId = getNearestClusterId(allPoints[i]);
            if (currentClusterId != nearestClusterId) {
                allPoints[i].setClusterId(nearestClusterId);
                changed = true;
            }
        }
        //clear all existing clusters
        clearClusters();
        //reassign points to their new clusters
        for (int i = 0; i < nPoints; i ++) {
            //cluster index is ID-1
            clusters[allPoints[i].getClusterId() - 1].addPoint(allPoints[i]);
        }
        //recalculating the center of each cluster
        for (int i = 0; i < K; i ++) {
            int clusterSize = clusters[i].getClusterSize();
            for (int j = 0; j < dimensions; j ++) {
                double sum = 0.0;
                if (clusterSize > 0) {
                    for (int p = 0; p < clusterSize; p ++) {
                        sum += clusters[i].getPoint(p).getVal(j);
                    }
                    clusters[i].setCentroidPos(j, sum / clusterSize);
                }
            }
        }
        if (!changed || epoch >= epochs) {
            std::cout << "Clustering completed in epoch : " << epoch << std::endl << std::endl;
            run = false;
        }
        epoch++;
    }
    //writing results
    std::ofstream pointsFile;
    pointsFile.open(output_dir + "/" + std::to_string(K) + "-points.txt", std::ios::out);
    for (int i = 0; i < nPoints; i ++) {
        pointsFile << allPoints[i].getId() << " " << allPoints[i].getClusterId() << std::endl;
    }
    pointsFile.close();
    //write cluster centers to file
    std::ofstream outfile;
    outfile.open(output_dir + "/" + std::to_string(K) + "-clusters.txt");
    if (outfile.is_open()) {
        for (int i = 0; i < K; i ++) {
            std::cout << "Cluster " << clusters[i].getClusterId() << " centroid: ";
            for (int j = 0; j < dimensions; j ++) {
                std::cout << clusters[i].getCentroidPos(j) << " "; //output to console
                outfile << clusters[i].getCentroidPos(j) << " "; //output to file
            }
            std::cout << std::endl;
            outfile << std::endl;
        }
        outfile.close();
    }
    else {
        std::cout << "Error: unable to write to clusters.txt";
    }
}
