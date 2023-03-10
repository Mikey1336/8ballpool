#include "bumper.h"
#include "graphics.h"

/************** Edge Struct ****************/

edge::edge() {
    p1 = point2D();
    p2 = point2D();
}

edge::edge(double x1, double y1, double x2, double y2) {
    p1 = point2D(x1, y1);
    p2 = point2D(x2, y2);
}

edge::edge(point2D point1, point2D point2) {
    p1 = point1;
    p2 = point2;
}

double edge::getLength() {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

double edge::getLengthX() {
    return p1.x-p2.x;
}

double edge::getLengthY() {
    return p1.y-p2.y;
}

point2D edge::closestPointOnLine(Circle c) {
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

/**************** Bumper Class ******************/

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
edge Bumper::isOverlapping(Circle c) {
//    for (int i = 0; i < 3; i++) {
//        edge border(corners[i], corners[i+1]);
//        point2D closestPoint = border.closestPointOnLine(c);
//        double dist = sqrt(pow(closestPoint.x - c.getCenterX(), 2) + pow(closestPoint.y - c.getCenterY(), 2));
//        if (dist <= c.getRadius()) {
//            return border;
//        }
//    }
//    return edge();
    edge border(corners[1], corners[2]);
    point2D closestPoint = border.closestPointOnLine(c);
    double dist = sqrt(pow(closestPoint.x - c.getCenterX(), 2) + pow(closestPoint.y - c.getCenterY(), 2));
    if (dist <= c.getRadius()) {
        return border;
    }
    return edge();
}

void Bumper::collide(Circle &c) {
    edge overlappingEdge = isOverlapping(c);
    if (overlappingEdge.getLengthX() != 0) {
        c.bounceY();
    } else if (overlappingEdge.getLengthY() != 0) {
        c.bounceX();
    }

    while (isOverlapping(c).getLength() != 0) {
        c.move(c.getXVelocity()/100.0, c.getYVelocity()/100.0);
    }
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