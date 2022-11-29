#ifndef SHAPE_H
#define SHAPE_H

#include <math.h> // This allows you to use sqrt and pow functions
#include <string>

const double PI = 3.14159265358979323846;

struct color {
    double red;
    double green;
    double blue;
    double alpha; // Opacity (0 is transparent, 1 is solid)

    /* Constructors */
    color();
    color(double r, double g, double b);
    color(double r, double g, double b, double a);

    /* Overloaded Operator */
    friend std::ostream& operator << (std::ostream& outs, const color &c);
};

struct point2D {
    double x;
    double y;

    /* Constructors */
    point2D();
    point2D(double x, double y);

    /* Overloaded Operator */
    friend std::ostream& operator << (std::ostream& outs, const point2D &p);
};

class Shape {
protected:
    color fill;
    color border;
    point2D center;
    double xVelocity;
    double yVelocity;

public:
    /* Constructors */
    Shape();
    explicit Shape(color fill);
    explicit Shape(point2D center);
    Shape(color fill, point2D center);
    Shape(color fill, color border, point2D center);
    Shape(double red, double green, double blue, double alpha);
    Shape(double x, double y);
    Shape(double red, double green, double blue, double alpha, double x, double y);
    Shape(color fill, double x, double y);
    Shape(double red, double green, double blue, double alpha, point2D center);
    Shape(double redFill, double greenFill, double blueFill, double alphaFill, double redBorder, double greenBorder, double blueBorder, double alphaBorder, double x, double y);

    /* Destructor */
    virtual ~Shape() = default;

    /* Getters */
    color getFillColor() const;
    double getFillRed() const;
    double getFillGreen() const;
    double getFillBlue() const;
    double getFillOpacity() const;
    color getBorderColor() const;
    double getBorderRed() const;
    double getBorderGreen() const;
    double getBorderBlue() const;
    double getBorderOpacity() const;
    point2D getCenter() const;
    double getCenterX() const;
    double getCenterY() const;
    virtual double getLeftX() const = 0;
    virtual double getRightX() const = 0;
    virtual double getTopY() const = 0;
    virtual double getBottomY() const = 0;
    double getXVelocity() const;
    double getYVelocity() const;

    /* Setters */
    void setFillColor(color c);
    void setBorderColor(color c);
    void setFillColor(double r, double g, double b, double a);
    void setBorderColor(double r, double g, double b, double a);
    void setFillRed(double r);
    void setFillGreen(double g);
    void setFillBlue(double b);
    void setFillOpacity(double a);
    void setBorderRed(double r);
    void setBorderGreen(double g);
    void setBorderBlue(double b);
    void setBorderOpacity(double a);
    void setCenter(point2D center);
    void setCenter(double x, double y);
    void setCenterX(double x);
    void setCenterY(double y);
    virtual void move(double deltaX, double deltaY);
    virtual void moveX(double deltaX);
    virtual void moveY(double deltaY);
    void setXVelocity(double xVel);
    void setYVelocity(double yVel);
    void setVelocity(double xVel, double yVel);
    void bounceX();
    void bounceY();

    virtual void draw() const = 0;
};

#endif //SHAPE_H
