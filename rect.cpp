#include "graphics.h"
#include "rect.h"
#include <iostream>
using namespace std;

/********************* Dimensions Struct ********************/

dimensions::dimensions() : width(0), height(0) {}

dimensions::dimensions(double w, double h) : width(w), height(h) {}

ostream& operator << (ostream& outs, const dimensions &d) {
    outs << "[" << d.width << ", " << d.height << "]";
    return outs;
}


Rect::Rect() : Shape(), size({0, 0}) {
}

Rect::Rect(dimensions size) : Shape() {
    setSize(size);
}

Rect::Rect(color fill) : Shape(fill), size({0, 0}) {
}

Rect::Rect(point2D center) : Shape(center), size({0, 0}) {
}

Rect::Rect(color fill, point2D center) : Shape(fill, center), size({0, 0}) {
}

Rect::Rect(double red, double green, double blue, double alpha) : Shape(red, green, blue, alpha), size({0, 0}) {
}

Rect::Rect(double x, double y) : Shape(x, y), size({0, 0}) {
}

Rect::Rect(double red, double green, double blue, double alpha, double x, double y) : Shape(red, green, blue, alpha, x, y), size({0, 0}) {
}

Rect::Rect(color fill, double x, double y) : Shape(fill, x, y), size({0, 0}) {
}

Rect::Rect(double red, double green, double blue, double alpha, point2D center) : Shape(red, green, blue, alpha, center), size({0, 0}) {
}

Rect::Rect(color fill, dimensions size) : Shape(fill) {
    setSize(size);
}

Rect::Rect(point2D center, dimensions size) : Shape(center) {
    setSize(size);
}

Rect::Rect(color fill, point2D center, dimensions size) : Shape(fill, center) {
    setSize(size);
}

Rect::Rect(double red, double green, double blue, double alpha, dimensions size) : Shape(red, green, blue, alpha) {
    setSize(size);
}

Rect::Rect(double x, double y, dimensions size) : Shape(x, y) {
    setSize(size);
}

Rect::Rect(double red, double green, double blue, double alpha, double x, double y, dimensions size) : Shape(red, green, blue, alpha, x, y) {
    setSize(size);
}

Rect::Rect(color fill, double x, double y, dimensions size) : Shape(fill, x, y) {
    setSize(size);
}

Rect::Rect(double red, double green, double blue, double alpha, point2D center, dimensions size) : Shape(red, green, blue, alpha, center) {
    setSize(size);
}

dimensions Rect::getSize() const {
    return size;
}

double Rect::getWidth() const {
    return size.width;
}

double Rect::getHeight() const {
    return size.height;
}

double Rect::getLeftX() const {
    return center.x - (size.width / 2.0);
}

double Rect::getRightX() const {
    return center.x + (size.width / 2.0);
}

double Rect::getTopY() const {
    return center.y - (size.height / 2.0);
}

double Rect::getBottomY() const {
    return center.y + (size.height / 2.0);
}

void Rect::setSize(dimensions size) {
    if (size.width >= 0 && size.height >= 0) {
        this->size = size;
    }
}

void Rect::setSize(double width, double height) {
    setSize({width, height});
}

void Rect::setWidth(double width) {
    setSize({width, size.height});
}

void Rect::setHeight(double height) {
    setSize({size.width, height});
}

void Rect::changeSize(double deltaWidth, double deltaHeight) {
    setSize({size.width + deltaWidth, size.height + deltaHeight});
}

void Rect::changeWidth(double delta) {
    setSize({size.width + delta, size.height});
}

void Rect::changeHeight(double delta) {
    setSize({size.width, size.height + delta});
}


bool Rect::isOverlapping(const Rect &r) const {
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

bool Rect::isOverlapping(int x, int y) const {
    if (x > getRightX() || x < getLeftX()) {
        return false;
    }
    if (y < getTopY() || y > getBottomY()) {
        return false;
    }
    return true; // Placeholder for compilation
}

void Rect::draw() const {
    glColor3f(fill.red, fill.green, fill.blue);
    glBegin(GL_QUAD_STRIP);
    glVertex2i(center.x + size.width/2, center.y - size.height/2 );
    glVertex2i(center.x - size.width/2, center.y - size.height/2);
    glVertex2i(center.x + size.width/2, center.y + size.height/2);
    glVertex2i(center.x - size.width/2, center.y + size.height/2);
    glEnd();
    // Don't forget to set the color to the fill field
}


void Rect::rotatePoint(Rect section, float angle, float rotatePointX, float rotatePointY) const {

    float s = sin(angle);
    float c = cos(angle);


    int v1x = (section.getCenterX() + section.getWidth()/2) - rotatePointX;
    int v1y = (section.getCenterY() - section.getHeight()/2) - rotatePointY;

    int v2x = (section.getCenterX() + section.getWidth()/2) - rotatePointX;
    int v2y = (section.getCenterY() + section.getHeight()/2) - rotatePointY;

    int v3x = (section.getCenterX() - section.getWidth()/2) - rotatePointX;
    int v3y = (section.getCenterY() + section.getHeight()/2) - rotatePointY;

    int v4x = (section.getCenterX() - section.getWidth()/2) - rotatePointX;
    int v4y = (section.getCenterY() - section.getHeight()/2) - rotatePointY;

    float v1xNew;
    float v1yNew;

    float v2xNew;
    float v2yNew;

    float v3xNew;
    float v3yNew;

    float v4xNew;
    float v4yNew;

        v1xNew = v2x * c + v2y * s;
        v1yNew = v2x * s + v2y * c;

        v2xNew = v1x * c + v1y * s;
        v2yNew = v1x * s + v1y * c;

        v3xNew = v4x * c + v4y * s;
        v3yNew = v4x * s + v4y * c;

        v4xNew = v3x * c + v3y * s;
        v4yNew = v3x * s + v3y * c;

    v1x = v1xNew + rotatePointX;
    v1y = v1yNew + rotatePointY;

    v2x = v2xNew + rotatePointX;
    v2y = v2yNew + rotatePointY;

    v3x = v3xNew + rotatePointX;
    v3y = v3yNew + rotatePointY;

    v4x = v4xNew + rotatePointX;
    v4y = v4yNew + rotatePointY;

    glBegin(GL_QUADS);
    glVertex2f(v1x,
               v1y);
    glVertex2f(v2x,
               v2y);
    glVertex2f(v3x,
               v3y);
    glVertex2f(v4x,
               v4y);
    glEnd();

}