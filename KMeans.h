//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_KMEANS_H
#define PARALLELPROGRAMMING_KMEANS_H

#include <vector>
#include <string>

class Point {
public:
    Point(int id, std::string line);

    int getDimensions() const;
    int getClusterId() const;
    int getId() const;
    void setClusterId(int c);
    double getVal(int pos);

private:
    int pointId, clusterId;
    int dimensions;
    std::vector<double> values;

    std::vector<double> linetoVec(std::string &line);
};

class Cluster {
public:
    Cluster(int clusterId, Point centroid);

    void addPoint(Point p);
    void removeAllPoints();
    int getClusterId() const;
    Point getPoint(int pos);
    int getClusterSize();
    double getCentroidPos(int pos);
    void setCentroidPos(int pos, double value);

private:
    int clusterId;
    std::vector<double> centroid;
    std::vector<Point> points;
};

class KMeans {
public:
    KMeans(int K, int epochs, std::string output_dir);

    void run(std::vector<Point> allPoints);

private:
    int K, epochs, dimensions, nPoints;
    std::vector<Cluster> clusters;
    std::string output_dir;

    void clearClusters();
    int getNearestClusterId(Point p);
};

#endif //PARALLELPROGRAMMING_KMEANS_H
