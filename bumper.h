#ifndef POOL_BUMPER_H
#define POOL_BUMPER_H

#include <vector>
#include "circle.h"

using namespace std;

struct edge {
    point2D p1;
    point2D p2;

    edge(double x1, double y1, double x2, double y2);
    edge(point2D point1, point2D point2);

    point2D closestPointOnLine(Circle c);
};

class Bumper {
private:
    vector<point2D> corners;
    color color;
public:
    // Constructors
    Bumper(struct color c, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
    Bumper(struct color c, point2D p1, point2D p2, point2D p3, point2D p4);

    // Collision Calculation
    bool isOverlapping(Circle c);

    // Draw
    void draw() const;
};


#endif //POOL_BUMPER_H
