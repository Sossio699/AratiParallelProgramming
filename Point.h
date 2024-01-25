//
// Created by Niccolò Arati on 08/01/2024.
//

#ifndef PARALLELPROGRAMMING_POINT_H
#define PARALLELPROGRAMMING_POINT_H

#include <vector>
#include <string>
#include <sstream>

class Point {
public:
    Point(int id, const std::string& line, bool csv = false);

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

    std::vector<double> linetoVecTXT(const std::string& line);
    std::vector<double> linetoVecCSV(const std::string& line);
};


#endif //PARALLELPROGRAMMING_POINT_H
