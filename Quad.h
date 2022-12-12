#ifndef CONFETTIFY_QUAD_H
#define CONFETTIFY_QUAD_H
#include "shape.h"



struct point {
    int x;
    int y;
};

//enum for switching through screens
enum screensEnum{
    startScreen,
    confettifyScreen,
    endScreen,
    prestoNoButton,
    prestoButton
};

class Quad {
protected:
    color fill;
    point center;
    unsigned int width;
    unsigned int height;

public:
    Quad();
    Quad(color fill, point center, unsigned int width, unsigned int height);

    int getCenterX() const;
    int getLeftX() const;
    int getRightX() const;
    int getCenterY() const;
    int getTopY() const;
    int getBottomY() const;
    point getCenter() const;

    double getRed() const;
    double getGreen() const;
    double getBlue() const;
    color getFill() const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void setColor(double red, double green, double blue);
    void setColor(color fill);
    void move(int deltaX, int deltaY);
    void resize(unsigned int width, unsigned int height);

    virtual void draw(int screen) const;
};

#endif //CONFETTIFY_QUAD_H
