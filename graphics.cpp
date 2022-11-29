#include "circle.h"
#include "graphics.h"
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;
vector<Circle> bubbles;
const int RADIUS = 50;

void init() {
    srand(time(0));
    width = 600;
    height = 600;

    for (int i = 100; i < width; i += 200) {
        bubbles.push_back(
                Circle(0, 0, 0, 0, 1, 0, 0,
                       0, i, 100, RADIUS, std::to_string((rand() % 15) + 1)));//(rand() % 10 + 1)*5));
        bubbles[bubbles.size()-1].setVelocity(rand() % 5 - 2, rand() % 5 - 2);

        bubbles.push_back(
                Circle(0, 0, 0, 0, 0, 1, 0,
                       0, i, 300, RADIUS, std::to_string((rand() % 15) + 1)));//(rand() % 10 + 1)*5));
        bubbles[bubbles.size()-1].setVelocity(rand() % 5 - 2, rand() % 5 - 2);

        bubbles.push_back(
                Circle(0, 0, 0, 0, 0, 0, 1,
                       0, i, 500, RADIUS, std::to_string((rand() % 15) + 1)));//(rand() % 10 + 1)*5));
        bubbles[bubbles.size()-1].setVelocity(rand() % 5 - 2, rand() % 5 - 2);
    }
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
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
    for (const Circle &bubble : bubbles) {
        bubble.draw();
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

    for (Circle &bubble : bubbles) {
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

    for (int i = 0; i < bubbles.size() - 1; ++i) {
        for (int j = i + 1; j < bubbles.size(); ++j) {
            if (bubbles[i].isOverlapping(bubbles[j])) {
                bubbles[i].collide(bubbles[j]);
            }
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