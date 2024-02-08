//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_CLUSTER_H
#define PARALLELPROGRAMMING_CLUSTER_H

#include <vector>
#include "Point.h"

class Cluster {
public:
    Cluster(int clusterId, const Point& centroid);

    int getClusterId() const;
    void setClusterId(int id);
    double getCentroidPos(int pos);
    void setCentroidPos(int pos, double value);
    void addPoint(Point p);
    void removeAllPoints();
    Point getPoint(int pos);
    int getClusterSize();

private:
    int clusterId;
    std::vector<double> centroid;
    std::vector<Point> points;
};

#endif //PARALLELPROGRAMMING_CLUSTER_H
