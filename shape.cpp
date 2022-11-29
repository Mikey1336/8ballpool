#include "shape.h"
#include <iostream>
using namespace std;

/********************* Color Struct ********************/

color::color() : red(0.0), green(0.0), blue(0.0), alpha(1.0) {
}

color::color(double r, double g, double b, double a) : red(r), green(g), blue(b), alpha(a) {
}

color::color(double r, double g, double b) : red(r), green(g), blue(b), alpha(1.0) {
}

ostream& operator << (ostream& outs, const color &c) {
    outs << "(" << c.red << ", " << c.green << ", " << c.blue << ")";
    return outs;
}

/********************* Point Struct ********************/

point2D::point2D() : x(0), y(0) {}

point2D::point2D(double x, double y) : x(x), y(y) {}

ostream& operator << (ostream& outs, const point2D &p) {
    outs << "(" << p.x << ", " << p.y << ")";
    return outs;
}

/********************* Shape Class *********************/

Shape::Shape() : fill({0.0, 0.0, 0.0}), border({0.0, 0.0, 0.0}), center({0, 0}) {
}

Shape::Shape(color fill) : fill(fill), border({0.0, 0.0, 0.0}), center({0, 0}) {
}

Shape::Shape(point2D center) : fill({0, 0, 0}), border({0.0, 0.0, 0.0}), center(center) {
}

Shape::Shape(color fill, point2D center) : fill(fill), border(fill), center(center) {
}

Shape::Shape(color fill, color border, point2D center) : fill(fill), border(border), center(center) {
}

Shape::Shape(double red, double green, double blue, double alpha) : fill({red, green, blue, alpha}), border({red, green, blue, alpha}), center(0, 0) {
}

Shape::Shape(double x, double y) : fill({0, 0, 0}), border({0.0, 0.0, 0.0}), center({x, y}) {
}

Shape::Shape(double red, double green, double blue, double alpha, double x, double y) : fill({red, green, blue, alpha}), border({red, green, blue, alpha}), center({x, y}) {
}

Shape::Shape(color fill, double x, double y) : fill(fill), border(fill), center({x, y}) {
}

Shape::Shape(double red, double green, double blue, double alpha, point2D center) : fill({red, green, blue, alpha}), border({red, green, blue, alpha}), center(center) {
}

Shape::Shape(double redFill, double greenFill, double blueFill, double alphaFill, double redBorder, double greenBorder, double blueBorder, double alphaBorder, double x, double y) : fill({redFill, greenFill, blueFill, alphaFill}), border({redBorder, greenBorder, blueBorder, alphaBorder}), center({x, y}) {
}

color Shape::getFillColor() const {
    return fill;
}

double Shape::getFillRed() const {
    return fill.red;
}

double Shape::getFillGreen() const {
    return fill.green;
}

double Shape::getFillBlue() const {
    return fill.blue;
}

double Shape::getFillOpacity() const {
    return fill.alpha;
}

color Shape::getBorderColor() const {
    return border;
}

double Shape::getBorderRed() const {
    return border.red;
}

double Shape::getBorderGreen() const {
    return border.green;
}

double Shape::getBorderBlue() const {
    return border.blue;
}

double Shape::getBorderOpacity() const {
    return border.alpha;
}

point2D Shape::getCenter() const {
    return center;
}

double Shape::getCenterX() const {
    return center.x;
}

double Shape::getCenterY() const {
    return center.y;
}

double Shape::getXVelocity() const {
    return xVelocity;
}

double Shape::getYVelocity() const {
    return yVelocity;
}

void Shape::setFillColor(color c) {
    fill = c;
}

void Shape::setBorderColor(color c) {
    border = c;
}

void Shape::setFillColor(double r, double g, double b, double a) {
    fill = {r, g, b, a};
}

void Shape::setBorderColor(double r, double g, double b, double a) {
    border = {r, g, b, a};
}

void Shape::setFillRed(double r) {
    fill.red = r;
}

void Shape::setFillGreen(double g) {
    fill.green = g;
}

void Shape::setFillBlue(double b) {
    fill.blue = b;
}

void Shape::setFillOpacity(double a) {
    fill.alpha = a;
}

void Shape::setBorderRed(double r) {
    border.red = r;
}

void Shape::setBorderGreen(double g) {
    border.green = g;
}

void Shape::setBorderBlue(double b) {
    border.blue = b;
}

void Shape::setBorderOpacity(double a) {
    border.alpha = a;
}

void Shape::setCenter(point2D center) {
    this->center = center;
}

void Shape::setCenter(double x, double y) {
    center = {x, y};
}

void Shape::setCenterX(double x) {
    center.x = x;
}

void Shape::setCenterY(double y) {
    center.y = y;
}

void Shape::move(double deltaX, double deltaY) {
    center.x += deltaX;
    center.y += deltaY;
}

void Shape::moveX(double deltaX) {
    center.x += deltaX;
}

void Shape::moveY(double deltaY) {
    center.y += deltaY;
}

void Shape::setXVelocity(double xVel) {
    xVelocity = xVel;
}

void Shape::setYVelocity(double yVel) {
    yVelocity = yVel;
}

void Shape::setVelocity(double xVel, double yVel) {
    xVelocity = xVel;
    yVelocity = yVel;
}

void Shape::bounceX() {
    xVelocity *= -1;
}

void Shape::bounceY() {
    yVelocity *= -1;
}