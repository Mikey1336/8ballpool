#include "bumper.h"
#include "graphics.h"

Bumper::Bumper(struct color c, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    color = c;
    corners.push_back(point2D(x1, y1));
    corners.push_back(point2D(x2, y2));
    corners.push_back(point2D(x3, y3));
    corners.push_back(point2D(x4, y4));
}

Bumper::Bumper(struct color c, point2D p1, point2D p2, point2D p3, point2D p4) {
    color = c;
    corners.push_back(p1);
    corners.push_back(p2);
    corners.push_back(p3);
    corners.push_back(p4);
}

// Collision Calculation
point2D Bumper::intersetionPoint(Circle c) {
    return point2D(0, 0);
}

point2D Bumper::closestPointOnLine(Circle c) {
    point2D p1 = corners[1];
    point2D p2 = corners[2];

    double A1 = p2.y - p1.y;
    double B1 = p1.x - p2.x;
    double C1 = A1*p1.x + B1*p1.y;
    double C2 = -B1*c.getCenterX() + A1*c.getCenterY();
    double det = A1*A1 - -B1*B1;

    double cx = 0;
    double cy = 0;

    if (det != 0) {
        cx = ((A1*C1 - B1*C2)/det);
        cy = ((A1*C2 - -B1*C1)/det);
    } else {
        cx = c.getCenterX();
        cy = c.getCenterY();
    }

    return point2D(cx, cy);
}

// Draw
void Bumper::draw() const {
    glColor3f(color.red, color.green, color.blue);
    glBegin(GL_QUADS);

    for (point2D point : corners) {
        glVertex2d(point.x, point.y);
    }

    glEnd();
}