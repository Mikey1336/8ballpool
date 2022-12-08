#ifndef POOL_BUMPER_H
#define POOL_BUMPER_H

#include <vector>
#include "circle.h"

using namespace std;

class Bumper {
private:
    vector<point2D> corners;
public:
    // Constructors
    Bumper(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
    Bumper(point2D p1, point2D p2, point2D p3, point2D p4);

    // Collision Calculation
    point2D intersetionPoint(Circle c);

};


#endif //POOL_BUMPER_H
