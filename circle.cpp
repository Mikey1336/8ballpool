#include "circle.h"
#include "graphics.h"

Circle::Circle() : Shape(), radius(0) {
}

Circle::Circle(double radius) : Shape() {
    setRadius(radius);
}

Circle::Circle(color fill) : Shape(fill), radius(0) {
}

Circle::Circle(point2D center) : Shape(center), radius(0) {
}

Circle::Circle(color fill, point2D center) : Shape(fill, center), radius(0) {
}

Circle::Circle(double red, double green, double blue, double alpha) : Shape(red, green, blue, alpha), radius(0) {
}

Circle::Circle(double x, double y) : Shape(x, y), radius(0) {
}

Circle::Circle(double red, double green, double blue, double alpha, double x, double y) : Shape(red, green, blue, alpha, x, y), radius(0) {
}

Circle::Circle(color fill, double x, double y) : Shape(fill, x, y), radius(0) {
}

Circle::Circle(double red, double green, double blue, double alpha, point2D center) : Shape(red, green, blue, alpha, center), radius(0) {
}

Circle::Circle(color fill, double radius) : Shape(fill) {
    setRadius(radius);
}

Circle::Circle(point2D center, double radius) : Shape(center) {
    setRadius(radius);
}

Circle::Circle(color fill, point2D center, double radius) : Shape(fill, center) {
    setRadius(radius);
}

Circle::Circle(double red, double green, double blue, double alpha, double radius) : Shape(red, green, blue, alpha) {
    setRadius(radius);
}

Circle::Circle(double x, double y, double radius) : Shape(x, y) {
    setRadius(radius);
}

Circle::Circle(double red, double green, double blue, double alpha, double x, double y, double radius, std::string num) : Shape(red, green, blue, alpha, x, y) {
    setRadius(radius);
    setNumber(num);
}

Circle::Circle(color fill, double x, double y, double radius) : Shape(fill, x, y) {
    setRadius(radius);
}

Circle::Circle(double red, double green, double blue, double alpha, point2D center, double radius) : Shape(red, green, blue, alpha, center) {
    setRadius(radius);
}

Circle::Circle(double redFill, double greenFill, double blueFill, double alphaFill, double redBorder, double greenBorder, double blueBorder, double alphaBorder, double x, double y, double radius, std::string num) : Shape(redFill, greenFill, blueFill, alphaFill, redBorder, greenBorder, blueBorder, alphaBorder, x, y) {
    setRadius(radius);
    setNumber(num);
}

double Circle::getRadius() const {
    return radius;
}

double Circle::getLeftX() const {
    return center.x - radius;
}

double Circle::getRightX() const  {
    return center.x + radius;
}

double Circle::getTopY() const {
    return center.y - radius;
}

double Circle::getBottomY() const {
    return center.y + radius;
}

void Circle::setRadius(double r) {
    if (r < 0) {
        r = 0;
    }
    radius = r;
}

void Circle::setNumber(std::string num) {
    number = num;
}

void Circle::changeRadius(double delta) {
    setRadius(radius + delta);
}

bool Circle::isOverlapping(const Circle &c) const {
    // return true if the distance between the center points is <= the sum of the radii
    return (sqrt((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) <= c.radius + radius);
}
bool Circle::isOverlapping(const Rect &r) const {
    //There are only two cases when rectangles are *not* overlapping:
    //1. when one is to the left of the other
    //2. when one is above the other

    if (r.getLeftX() > getRightX() or r.getRightX() < getLeftX()) {
        return false;
    }
    if (r.getBottomY() < getTopY() or r.getTopY() > getBottomY()) {
        return false;
    }
    return true;

}

void Circle::collide(Circle &c) {
    // 2-dimensional elastic collision
    double radiusSum = radius + c.radius;
    double xVel = xVelocity, yVel = yVelocity, xVel2 = c.xVelocity, yVel2 = c.yVelocity;
    xVelocity = (xVel - ((2.0*c.radius)/radiusSum) * ((xVel - xVel2)*(c.center.x - center.x) + (yVel - yVel2)*(c.center.y - center.y)) / double((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) * (c.center.x - center.x));
    yVelocity = (yVel - ((2.0*c.radius)/radiusSum) * ((xVel - xVel2)*(c.center.x - center.x) + (yVel - yVel2)*(c.center.y - center.y)) / double((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) * (c.center.y - center.y));
    c.xVelocity = (xVel2 - ((2.0*radius)/radiusSum) * ((xVel2 - xVel)*(center.x - c.center.x) + (yVel2 - yVel)*(center.y - c.center.y)) / double((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) * (center.x - c.center.x));
    c.yVelocity = (yVel2 - ((2.0*radius)/radiusSum) * ((xVel2 - xVel)*(center.x - c.center.x) + (yVel2 - yVel)*(center.y - c.center.y)) / double((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) * (center.y - c.center.y));

    while (isOverlapping(c)) {
        move(xVelocity/100.0, yVelocity/100.0);
        c.move(c.xVelocity/100.0, c.yVelocity/100.0);
    }
}


//Collide Method Still doesn't work :(
//void Circle::collide(Rect &r) {
//    // 2-dimensional non-elastic collision
//    double radiusSum = radius + r.getWidth()/2;
//    double xVel = xVelocity, yVel = yVelocity;
//    xVelocity = (xVel - ((2.0*r.getWidth()/2)/radiusSum) * ((xVel)*(r.getCenterX() - center.x) + (yVel)*(r.getCenterY() - center.y)) / double((r.getCenterX() - center.x)*(r.getCenterX() - center.x) + (r.getCenterY() - center.y)*(r.getCenterY() - center.y)) * (r.getCenterX() - center.x));
//    yVelocity = (yVel - ((2.0*r.getWidth()/2)/radiusSum) * ((xVel)*(r.getCenterX() - center.x) + (yVel)*(r.getCenterY() - center.y)) / double((r.getCenterX() - center.x)*(r.getCenterX() - center.x) + (r.getCenterY() - center.y)*(r.getCenterY() - center.y)) * (r.getCenterY() - center.y));
//
//    while (isOverlapping(r)) {
//        move(xVelocity/100.0, yVelocity/100.0);
//    }
//}

void Circle::draw() const {
    // Set drawing color to fill color
//    glColor3f(fill.red, fill.green, fill.blue);
    glColor3f(border.red, border.green, border.blue);
    // Draw circle as Triangle Fan
    glBegin(GL_TRIANGLE_FAN);
    // Draw center point
    glVertex2i(center.x, center.y);
    // Set drawing color to border color
//    glColor3f(border.red, border.green, border.blue);
    // Draw points on edge of circle
    for (double i = 0; i < 2.0*PI+0.05; i += (2.0*PI)/360.0) {
        glVertex2i(center.x + (radius * cos(i)),
                   center.y + (radius * sin(i)));
    }
    // End Triangle Fan
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(center.x, center.y);
    for (double i = 0; i < 2.0*PI+0.05; i += (2.0*PI)/360.0) {
        glVertex2i(center.x + (radius/1.75 * cos(i)),
                   center.y + (radius/1.75 * sin(i)));
    }
    glEnd();

    glColor3f(0, 0, 0);
    glRasterPos2i(center.x-5, center.y+2);
    for (char letter : number) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, letter);
    }
}
