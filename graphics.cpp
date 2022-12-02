#include "circle.h"
#include "graphics.h"
#include <ctime>
#include <iostream>
#include <vector>
#include "rect.h"
using namespace std;

GLdouble width, height;
int wd;
vector<Circle> balls;
vector<Rect> bumpers;

const int RADIUS = 12;
const double FRICTION = 0.02;

const color tableDark(0.1725, 0.5098, 0.3412);
const color tableLight(0.1804, 0.5451, 0.3412);
const color wood(0.6, 0.3, 0.2);

void init() {
    srand(time(0));
    width = 1500;
    height = 750;
//Generate balls in columns


//Back most column
    for (int i = 0; i < 125; i += 25) {
        balls.push_back(
                Circle(0, 0, 0, 0, 1, 0, 0,
                       0, 1080, (i+300), RADIUS, std::to_string((rand() % 15) + 1)));//(rand() % 10 + 1)*5));
    }
    //increment x by -21 and y by -12.5
    //reduce number of balls by one each new loop

    //Second Back most column
    for (int i = 25; i < 125; i += 25) {
        balls.push_back(
                Circle(0, 0, 0, 0, 1, 0, 0,
                       0, 1059, (i+287.5), RADIUS, std::to_string((rand() % 15) + 1)));//(rand() % 10 + 1)*5));
    }
    //Third from Back most column
    for (int i = 50; i < 125; i += 25) {
        balls.push_back(
                Circle(0, 0, 0, 0, 1, 0, 0,
                       0, 1038, (i+275), RADIUS, std::to_string((rand() % 15) + 1)));//(rand() % 10 + 1)*5));
    }

    for (int i = 75; i < 125; i += 25) {
        balls.push_back(
                Circle(0, 0, 0, 0, 1, 0, 0,
                       0, 1017, (i+262.5), RADIUS, std::to_string((rand() % 15) + 1)));//(rand() % 10 + 1)*5));
    }
    //Front(Left) Most column
    balls.push_back(Circle(0, 0, 0, 0, 1, 0, 0,
                           0, 996, (350), RADIUS, std::to_string((rand() % 15) + 1)));

    //Cue Ball
    balls.push_back(
            Circle(0, 0, 0, 0, 1, 0, 0,
                   0, 350, (350), RADIUS, std::to_string((rand() % 15) + 1)));

    balls[balls.size()-1].setVelocity(6, .01);
    //Bumpers
    dimensions bumperSize;

    //Left and right
    bumperSize.width = 30;
    bumperSize.height = 500;
    bumpers.push_back(
            Rect(tableLight,
                    130,
                    375,
                    bumperSize));
    bumpers.push_back(
            Rect(tableLight,
                 1220,
                 375,
                 bumperSize));

    //Top and bottom
    bumperSize.width = 1120;
    bumperSize.height = 30;

    bumpers.push_back(
            Rect(tableLight,
                 675,
                 130,
                 bumperSize));

    bumpers.push_back(
            Rect(tableLight,
                 675,
                 620,
                 bumperSize));





}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

void drawTable() {
    const int CORNER_RADIUS = 10;
    const int WOOD_BORDER = 40;
    const int TABLE_BORDER = 30;
    int tableWidth = width - 300;
    int tableHeight = tableWidth / 2;
    int border = (height - tableHeight) / 2;

    glBegin(GL_QUADS);
//Table Borders
    glColor3f(wood.red, wood.green, wood.blue);
    glVertex2f(border, border);
    glVertex2f(border + tableWidth, border);
    glVertex2f(border + tableWidth, border + tableHeight);
    glVertex2f(border, border + tableHeight);

//Table Green Playing surface
    glColor3f(tableDark.red, tableDark.green, tableDark.blue);
    glVertex2f(border + WOOD_BORDER + TABLE_BORDER, border + WOOD_BORDER + TABLE_BORDER);
    glVertex2f(border + tableWidth - WOOD_BORDER - TABLE_BORDER, border + WOOD_BORDER + TABLE_BORDER);
    glVertex2f(border + tableWidth - WOOD_BORDER - TABLE_BORDER, border + tableHeight - WOOD_BORDER - TABLE_BORDER);
    glVertex2f(border + WOOD_BORDER + TABLE_BORDER, border + tableHeight - WOOD_BORDER - TABLE_BORDER);

    glEnd();
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    /*
     * Draw here
     */

    // Draw Table
    drawTable();
//Draw Balls
    for (const Circle &bubble : balls) {
        bubble.draw();
    }
//Draw Bumpers
    for (const Rect &bumper : bumpers){
        bumper.draw();
    }




    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    switch(key) {
        case 27: {
            // escape
            glutDestroyWindow(wd);
            exit(0);
        }
    }
    glutPostRedisplay();
}

void kbdUp(unsigned char key, int x, int y) {

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:

            break;
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
        case GLUT_KEY_UP:

            break;
    }

    glutPostRedisplay();
}

void cursor(int x, int y) {

    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

    glutPostRedisplay();
}

void timer(int dummy) {

    for (Circle &bubble : balls) {
        bubble.move(bubble.getXVelocity(), bubble.getYVelocity());
        if (bubble.getCenterX() < bubble.getRadius()) {
            bubble.bounceX();
            bubble.setCenterX(bubble.getRadius());
        } else if (bubble.getCenterX() > (width-bubble.getRadius())) {
            bubble.bounceX();
            bubble.setCenterX(width-bubble.getRadius());
        }
        if (bubble.getCenterY() < bubble.getRadius()) {
            bubble.bounceY();
            bubble.setCenterY(bubble.getRadius());
        } else if (bubble.getCenterY() > (height-bubble.getRadius())) {
            bubble.bounceY();
            bubble.setCenterY(height-bubble.getRadius());
        }
    }
//Ball collisions
    for (int i = 0; i < balls.size() - 1; ++i) {
        for (int j = i + 1; j < balls.size(); ++j) {
            if (balls[i].isOverlapping(balls[j])) {
                balls[i].collide(balls[j]);
            }
        }
    }
//Bumper collisions
    for (int i = 0; i < balls.size() - 1; ++i) {
        for (int j = i + 1; j < bumpers.size(); ++j) {
            if (balls[i].isOverlapping(bumpers[j])) {
                balls[i].collide(bumpers[j]);
            }
        }
    }




    for (int i = 0; i < balls.size(); ++i) {
        if (balls[i].getXVelocity() > 0.001) {
            balls[i].setXVelocity(balls[i].getXVelocity() - FRICTION);
        } else if (balls[i].getXVelocity() < -0.001) {
            balls[i].setXVelocity(balls[i].getXVelocity() + FRICTION);
        } else {
            balls[i].setVelocity(0, 0);
        }

        if (balls[i].getYVelocity() > 0.001) {
            balls[i].setYVelocity(balls[i].getYVelocity() - FRICTION);
        } else if (balls[i].getYVelocity() < -0.001) {
            balls[i].setYVelocity(balls[i].getYVelocity() + FRICTION);
        } else {
            balls[i].setVelocity(0, 0);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Screen Saver" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbdUp);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}