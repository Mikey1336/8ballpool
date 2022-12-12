#include "Quad.h"
#include "graphics.h"
#include <string>
#include <iostream>

using namespace std;



Quad::Quad() {
    fill = {0, 0, 0};
    center = {0, 0};
    width = 50;
    height = 50;
}

Quad::Quad(color fill, point center, unsigned int width, unsigned int height) {
    this->fill = fill;
    this->center = center;
    this->width = width;
    this->height = height;
}

int Quad::getCenterX() const {
    return center.x;
}

int Quad::getLeftX() const {
    return center.x - (width / 2);
}

int Quad::getRightX() const {
    return center.x + (width / 2);
}

int Quad::getCenterY() const {
    return center.y;
}

int Quad::getTopY() const {
    return center.y - (height / 2);
}

int Quad::getBottomY() const {
    return center.y + (height / 2);
}

point Quad::getCenter() const {
    return center;
}

double Quad::getRed() const {
    return fill.red;
}

double Quad::getGreen() const {
    return fill.green;
}

double Quad::getBlue() const {
    return fill.blue;
}

color Quad::getFill() const {
    return fill;
}

unsigned int Quad::getWidth() const {
    return width;
}

unsigned int Quad::getHeight() const {
    return height;
}

void Quad::setColor(double red, double green, double blue) {
    fill = {red, green, blue};
}

void Quad::setColor(color fill) {
    this->fill = fill;
}

void Quad::move(int deltaX, int deltaY) {
    center.x += deltaX;
    center.y += deltaY;
}

void Quad::resize(unsigned int width,unsigned int height) {
    this->width = width;
    this->height = height;
}

void Quad::draw(int screen) const {
        //if enum == start screen, display start screen
        if (screen == startScreen) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glColor3f(0, 0, 1);
            string displayString = "PRESS 's' TO PROCEED WITH GAME";
            glRasterPos2f(130, 100);
            int len = displayString.length();
            string lenString;
            int i;

            for (i = 0; i < len; i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, displayString[i]);
            }
            //if enum == confettify screen, display confettify screen
        } else if (screen == confettifyScreen) {
            // Don't forget to set the color to the fill field
            // Set drawing color to fill color
            glColor3f(fill.red, fill.green, fill.blue);
            // Draw rectangle
            glBegin(GL_QUADS);
            // Draw center point
            glVertex2i(getLeftX(), getTopY());
            glVertex2i(getLeftX(), getBottomY());
            glVertex2i(getRightX(), getBottomY());
            glVertex2i(getRightX(), getTopY());
            // End quad
            glEnd();
        //if enum == end screen, display end screen
        } else if (screen == endScreen) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glColor3f(0, 0, 1);
            string displayString = "YOU HAVE COMPLETED THE GAME. CONGRATULATIONS";
            glRasterPos2f(90, 100);
            int len = displayString.length();
            string lenString;
            int i;
            for (i = 0; i < len; i++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, displayString[i]);
            }
            //if enum == presto no button, remove the button
            //if enum == presto button, set enum to confettify screen and draw again.
        } else if (screen == prestoNoButton || screen == prestoButton) {
            if (screen == prestoNoButton) {
                glColor3f(0.0f, 0.0f, 0.0f);
            } else {
                screen = confettifyScreen;
                draw(screen);
            }

        }

}
