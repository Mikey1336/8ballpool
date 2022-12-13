#include "poolBall.h"
#include "graphics.h"

PoolBall::PoolBall(double x, double y, double radius, int value) : Circle(1, 1, 1, 1, x, y, radius) {
    this->value = value;
    switch (value % 8) {
        case 0: {
            // Black
            fill = color(0, 0, 0);
            break;
        } case 1: {
            // Yellow
            fill = color(246/255.0, 190/255.0, 0);
            break;
        } case 2: {
            // Blue
            fill = color(0, 0, 1);
            break;
        } case 3: {
            // Red
            fill = color(1, 0, 0);
            break;
        } case 4: {
            // Purple
            fill = color(97/255.0, 38/255.0, 107/255.0);
            break;
        } case 5: {
            // Orange
            fill = color(1, 0.5, 0);
            break;
        } case 6: {
            // Green
            fill = color(0, 0.25, 0);
            break;
        } case 7: {
            // Maroon
            fill = color(0.5, 0, 0);
            break;
        }
    }
}

void PoolBall::draw() const {
    if (value / 8.0 > 1) {
        // Stripes

        // Draw background white
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1, 1, 1);
        glVertex2i(center.x, center.y);
        for (double i = 0; i < 2.0*PI+0.05; i += (2.0*PI)/360.0) {
            glVertex2i(center.x + (getRadius() * cos(i)),
                       center.y + (getRadius() * sin(i)));
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glColor3f(fill.red, fill.green, fill.blue);
        for (double i = -45*PI/180; i < 45*PI/180; i += (2.0*PI)/360.0) {
            glVertex2i(center.x + (getRadius() * cos(i)),
                       center.y + (getRadius() * sin(i)));
        }
        for (double i = 135*PI/180; i < 225*PI/180; i += (2.0*PI)/360.0) {
            glVertex2i(center.x + (getRadius() * cos(i)),
                       center.y + (getRadius() * sin(i)));
        }
        glEnd();
    } else {
        glColor3f(fill.red, fill.green, fill.blue);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2i(center.x, center.y);
        for (double i = 0; i < 2.0*PI+0.05; i += (2.0*PI)/360.0) {
            glVertex2i(center.x + (getRadius() * cos(i)),
                       center.y + (getRadius() * sin(i)));
        }
        glEnd();
    }

    // Draw small white inner circle
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(center.x, center.y);
    for (double i = 0; i < 2.0*PI+0.05; i += (2.0*PI)/360.0) {
        glVertex2i(center.x + (getRadius()/1.75 * cos(i)),
                   center.y + (getRadius()/1.75 * sin(i)));
    }
    glEnd();

    int hozModifier = 3;
    if (std::to_string(value).size() > 1) {
        hozModifier = 6;
    }

    glColor3f(0, 0, 0);
    glRasterPos2d(center.x-hozModifier, center.y+3);
    for (char letter : std::to_string(value)) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, letter);
    }
}