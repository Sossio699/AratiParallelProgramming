//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_POINT_H
#define PARALLELPROGRAMMING_POINT_H

#include <vector>
#include <string>

class Point {
public:
    Point(int id, std::string line);

    Point (Point const &p);

    int getId() const;
    void setId(int id);
    int getClusterId() const;
    void setClusterId(int c);
    int getDimensions() const;
    void setDimensions (int d);
    double getVal(int pos) const;

private:
    int pointId, clusterId;
    int dimensions;
    std::vector<double> values;

    std::vector<double> linetoVec(std::string &line);
};


#endif //PARALLELPROGRAMMING_POINT_H
