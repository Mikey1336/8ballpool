#include "circle.h"
#include "graphics.h"
#include <ctime>
#include <iostream>
#include <vector>
#include "rect.h"
#include "bumper.h"
#include "Button.h"

using namespace std;

const GLdouble WIDTH = 1500;
const GLdouble HEIGHT = 750;
GLdouble tableWidth, tableHeight;
int wd;
float angle;
vector<Circle> balls, pockets, sights;
vector<Bumper> bumpers;
vector<Rect> cueStick;
Rect playArea, leftBorder, topBorder, rightBorder, bottomBorder;
Button morePower({0, 1, .2}, {1400, 100}, 100, 50, "+ Power");
Button lessPower({1, .1, 0}, {1400, 200}, 100, 50, "- Power");
Button shoot({1, 1, 0}, {1400, 300}, 100, 50, "Take Shot");
int shotPower = 5;
double shotAngle = 0.0;
//Rise and Run for shot angle
double rise;
double run;
double shotRise;
double shotRun;
//Value For checking if balls are moving
double movement;

const int RADIUS = 12;
const double FRICTION = 0.05;

int WOOD_BORDER, BUMPER_WIDTH, EMPTY_BORDER;

const color tableDark(0.1725, 0.5098, 0.3412);
const color tableLight(0.1804, 0.5451, 0.3412);
const color wood(0.6, 0.3, 0.2);
const color silver(192/255.0, 192/255.0, 192/255.0);
const color cueWood(253 / 255.0, 217 / 255.0, 181 / 255.0);
const color black(0, 0, 0);
const color white(1, 1, 1);
const color pink(241 / 255.0, 145 / 255.0, 155 / 255.0);
const color gray(100 / 255.0, 100 / 255.0, 100 / 255.0);

enum screenEnum {
    watchScreen,
    shotScreen
};

screenEnum screen = shotScreen;

void init() {
    srand(time(0));
    tableWidth = WIDTH - 500;
    tableHeight = tableWidth / 2;
    WOOD_BORDER = tableWidth * 0.05;
    BUMPER_WIDTH = tableWidth * 0.02;
    EMPTY_BORDER = (HEIGHT - tableHeight - 2*WOOD_BORDER - 2*BUMPER_WIDTH) / 2;

    // Initialize table rectangles (playing area, wood borders)
    dimensions playAreaDimensions(tableWidth + 2*BUMPER_WIDTH, tableHeight + 2*BUMPER_WIDTH);
    point2D tableCenter(EMPTY_BORDER + WOOD_BORDER + BUMPER_WIDTH + (tableWidth/2.0), HEIGHT/2);
    playArea = Rect(tableDark, tableCenter, playAreaDimensions);

    double borderDeltaWidth = (tableWidth + WOOD_BORDER) / 2.0 + BUMPER_WIDTH;
    double borderDeltaHeight = (tableHeight + WOOD_BORDER) / 2.0 + BUMPER_WIDTH;
    dimensions sideDimensions(WOOD_BORDER, tableHeight);
    dimensions topBottomDimensions(tableWidth, WOOD_BORDER);
    leftBorder = Rect(wood, tableCenter.x - borderDeltaWidth, tableCenter.y, sideDimensions);
    topBorder = Rect(wood, tableCenter.x, tableCenter.y - borderDeltaHeight, topBottomDimensions);
    rightBorder = Rect(wood, tableCenter.x + borderDeltaWidth, tableCenter.y, sideDimensions);
    bottomBorder = Rect(wood, tableCenter.x, tableCenter.y + borderDeltaHeight, topBottomDimensions);

    // Initialize sights
    double topRowSightY = EMPTY_BORDER + topBottomDimensions.height / 2;
    double bottomRowSightY = HEIGHT - EMPTY_BORDER - topBottomDimensions.height / 2;
    double leftColumnSightX = EMPTY_BORDER + sideDimensions.width / 2;
    double rightColumnSightX = EMPTY_BORDER + WOOD_BORDER + playAreaDimensions.width + sideDimensions.width / 2;
    sights.push_back(Circle(silver, playArea.getCenterX() - 3*(playAreaDimensions.width-2*BUMPER_WIDTH)/8, topRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() - (playAreaDimensions.width-2*BUMPER_WIDTH)/4, topRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() - (playAreaDimensions.width-2*BUMPER_WIDTH)/8, topRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() + (playAreaDimensions.width-2*BUMPER_WIDTH)/8, topRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() + (playAreaDimensions.width-2*BUMPER_WIDTH)/4, topRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() + 3*(playAreaDimensions.width-2*BUMPER_WIDTH)/8, topRowSightY, 5));

    sights.push_back(Circle(silver, playArea.getCenterX() - 3*(playAreaDimensions.width-2*BUMPER_WIDTH)/8, bottomRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() - (playAreaDimensions.width-2*BUMPER_WIDTH)/4, bottomRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() - (playAreaDimensions.width-2*BUMPER_WIDTH)/8, bottomRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() + (playAreaDimensions.width-2*BUMPER_WIDTH)/8, bottomRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() + (playAreaDimensions.width-2*BUMPER_WIDTH)/4, bottomRowSightY, 5));
    sights.push_back(Circle(silver, playArea.getCenterX() + 3*(playAreaDimensions.width-2*BUMPER_WIDTH)/8, bottomRowSightY, 5));

    sights.push_back(Circle(silver, leftColumnSightX, playArea.getCenterY() - (playAreaDimensions.height-2*BUMPER_WIDTH)/4, 5));
    sights.push_back(Circle(silver, leftColumnSightX, playArea.getCenterY(), 5));
    sights.push_back(Circle(silver, leftColumnSightX, playArea.getCenterY() + (playAreaDimensions.height-2*BUMPER_WIDTH)/4, 5));
    sights.push_back(Circle(silver, leftColumnSightX, playArea.getCenterY() - (playAreaDimensions.height-2*BUMPER_WIDTH)/4, 5));
    sights.push_back(Circle(silver, leftColumnSightX, playArea.getCenterY(), 5));
    sights.push_back(Circle(silver, leftColumnSightX, playArea.getCenterY() + (playAreaDimensions.height-2*BUMPER_WIDTH)/4, 5));

    sights.push_back(Circle(silver, rightColumnSightX, playArea.getCenterY() - (playAreaDimensions.height-2*BUMPER_WIDTH)/4, 5));
    sights.push_back(Circle(silver, rightColumnSightX, playArea.getCenterY(), 5));
    sights.push_back(Circle(silver, rightColumnSightX, playArea.getCenterY() + (playAreaDimensions.height-2*BUMPER_WIDTH)/4, 5));
    sights.push_back(Circle(silver, rightColumnSightX, playArea.getCenterY() - (playAreaDimensions.height-2*BUMPER_WIDTH)/4, 5));
    sights.push_back(Circle(silver, rightColumnSightX, playArea.getCenterY(), 5));
    sights.push_back(Circle(silver, rightColumnSightX, playArea.getCenterY() + (playAreaDimensions.height-2*BUMPER_WIDTH)/4, 5));

    cout << (playAreaDimensions.height-2*BUMPER_WIDTH)/4 << endl;
    cout << (playAreaDimensions.width-2*BUMPER_WIDTH)/8 << endl;

    //MAKE pocketsis
    for (int i = 0; i < 3; i++) {
        pockets.push_back(
                Circle(0, 0, 0, 0, 0, 0, 0,
                       0, i * 543 + 133, (133), 18, ".")
        );

        pockets.push_back(
                Circle(0, 0, 0, 0, 0, 0, 0,
                       0, i * 543 + 133, (618), 18, ".")
        );
    }

    tableWidth = WIDTH - 300;
    tableHeight = tableWidth / 2;

    //Generate balls in columns

    //Back most column
    for (int i = 0; i < 125; i += 25) {
        balls.push_back(
                Circle(0, 0, 0, 0, 1, 0, 0,
                       0, 1080, (i + 300), RADIUS, to_string(i / 25)));//(rand() % 10 + 1)*5));
    }
    //increment x by -21 and y by -12.5
    //reduce number of balls by one each new loop

    //Second Back most column
    for (int i = 25; i < 125; i += 25) {
        balls.push_back(
                Circle(0, 0, 0, 0, 1, 0, 0,
                       0, 1059, (i + 287.5), RADIUS, to_string(5 + i / 25)));
    }
    //Third from Back most column
    for (int i = 50; i < 125; i += 25) {
        balls.push_back(
                Circle(0, 0, 0, 0, 1, 0, 0,
                       0, 1038, (i + 275), RADIUS, to_string(9 + i / 25)));
    }

    for (int i = 75; i < 125; i += 25) {
        balls.push_back(
                Circle(0, 0, 0, 0, 1, 0, 0,
                       0, 1017, (i + 262.5), RADIUS, to_string(12 + i / 25)));
    }
    //Front(Left) Most column
    balls.push_back(Circle(0, 0, 0, 0, 1, 0, 0,
                           0, 996, (350), RADIUS, "1"));

    //Cue Ball
    balls.push_back(
            Circle(1, 1, 1, 1, 1, 1, 1,
                   0, 350, (350), RADIUS, to_string(0)));

    //Bumpers
    dimensions bumperSize;

    //Left and right
    bumperSize.width = 30;
    bumperSize.height = 410;
    bumpers.push_back(Bumper(tableLight, 115, 170, 145, 170, 145, 580, 115, 580));
    bumpers.push_back(Bumper(tableLight, 1235, 170, 1205, 170, 1205, 580, 1235, 580));

    //Top and bottom
    bumperSize.width = 1010;
    bumperSize.height = 30;

    bumpers.push_back(Bumper(tableLight, 170, 115, 170, 145, 1180, 145, 1180, 115));
    bumpers.push_back(Bumper(tableLight, 170, 635, 170, 605, 1180, 605, 1180, 635));

    // Create pool cue
    dimensions cueSize;
    cueSize.height = 10;
    cueSize.width = 160;
    cueStick.push_back(
            Rect(gray,
                 720,
                 700,
                 cueSize));

    cueSize.width = 360;
    cueStick.push_back(
            Rect(cueWood,
                 460,
                 700,
                 cueSize));

    cueSize.width = 20;
    cueStick.push_back(
            Rect(white,
                 280,
                 700,
                 cueSize));

    cueSize.width = 4;
    cueStick.push_back(
            Rect(pink,
                 268,
                 700,
                 cueSize));


}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

void drawTable() {
    leftBorder.draw();
    topBorder.draw();
    rightBorder.draw();
    bottomBorder.draw();


    glColor3d(silver.red, silver.green, silver.blue);

    // Top left corner
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(playArea.getLeftX() + BUMPER_WIDTH, playArea.getTopY() + BUMPER_WIDTH);
    glVertex2d(playArea.getLeftX() - WOOD_BORDER, playArea.getTopY() + BUMPER_WIDTH);
    glVertex2d(playArea.getLeftX() - WOOD_BORDER, playArea.getTopY());
    for (double i = PI; i < 3*PI/2+0.05; i += (2.0*PI)/360.0) {
        glVertex2d(playArea.getLeftX() + ((WOOD_BORDER) * cos(i)),
                   playArea.getTopY() + ((WOOD_BORDER) * sin(i)));
    }
    glVertex2d(playArea.getLeftX(), playArea.getTopY() - WOOD_BORDER);
    glVertex2d(playArea.getLeftX() + BUMPER_WIDTH, playArea.getTopY() - WOOD_BORDER);

    glEnd();

    // Top right corner
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(playArea.getRightX() - BUMPER_WIDTH, playArea.getTopY() + BUMPER_WIDTH);
    glVertex2d(playArea.getRightX() - BUMPER_WIDTH, playArea.getTopY() - WOOD_BORDER);
    glVertex2d(playArea.getRightX(), playArea.getTopY() - WOOD_BORDER);
    for (double i = 3*PI/2; i < 2*PI+0.05; i += (2.0*PI)/360.0) {
        glVertex2d(playArea.getRightX() + ((WOOD_BORDER) * cos(i)),
                   playArea.getTopY() + ((WOOD_BORDER) * sin(i)));
    }
    glVertex2d(playArea.getRightX() + WOOD_BORDER, playArea.getTopY());
    glVertex2d(playArea.getRightX() + WOOD_BORDER, playArea.getTopY() + BUMPER_WIDTH);

    glEnd();

    // Bottom right corner
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(playArea.getRightX() - BUMPER_WIDTH, playArea.getBottomY() - BUMPER_WIDTH);
    glVertex2d(playArea.getRightX() + WOOD_BORDER, playArea.getBottomY() - BUMPER_WIDTH);
    glVertex2d(playArea.getRightX() + WOOD_BORDER, playArea.getBottomY());
    for (double i = 0; i < PI/2+0.05; i += (2.0*PI)/360.0) {
        glVertex2d(playArea.getRightX() + ((WOOD_BORDER) * cos(i)),
                   playArea.getBottomY() + ((WOOD_BORDER) * sin(i)));
    }
    glVertex2d(playArea.getRightX(), playArea.getBottomY() + WOOD_BORDER);
    glVertex2d(playArea.getRightX() - BUMPER_WIDTH, playArea.getBottomY() + WOOD_BORDER);

    glEnd();

    // Bottom left corner
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(playArea.getLeftX() + BUMPER_WIDTH, playArea.getBottomY() - BUMPER_WIDTH);
    glVertex2d(playArea.getLeftX() + BUMPER_WIDTH, playArea.getBottomY() + WOOD_BORDER);
    glVertex2d(playArea.getLeftX(), playArea.getBottomY() + WOOD_BORDER);
    for (double i = PI/2; i < PI+0.05; i += (2.0*PI)/360.0) {
        glVertex2d(playArea.getLeftX() + ((WOOD_BORDER) * cos(i)),
                   playArea.getBottomY() + ((WOOD_BORDER) * sin(i)));
    }
    glVertex2d(playArea.getLeftX() - WOOD_BORDER, playArea.getBottomY());
    glVertex2d(playArea.getLeftX() - WOOD_BORDER, playArea.getBottomY() - BUMPER_WIDTH);

    glEnd();

    for (Circle sight : sights) {
        sight.draw();
    }

    playArea.draw();
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, WIDTH, HEIGHT);

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    /*
     * Draw here
     */

    // Draw Table

    switch (screen) {
        //when screen is start print message to enter program
        case watchScreen: {
            drawTable();

//Draw Bumpers
//            for (Bumper bumper: bumpers) {
//                bumper.draw();
//                point2D center = bumper.closestPointOnLine(balls[balls.size() - 1]);
//                Circle dot(center, 5);
//                glColor3f(1, 1, 1);
//                dot.draw();
//            }


//Draw Balls
            for (const Circle &bubble: balls) {
                bubble.draw();
            }
//Draw Pockets
            for (const Circle &pocket: pockets) {
                pocket.draw();
            }


            glFlush();  // Render now
        }

        case shotScreen: {

            drawTable();

//Draw Bumpers
//            for (Bumper bumper: bumpers) {
//                bumper.draw();
//                point2D center = bumper.closestPointOnLine(balls[balls.size() - 1]);
//                Circle dot(center, 5);
//                glColor3f(1, 1, 1);
//                dot.draw();
//            }


//Draw Balls
            for (const Circle &bubble: balls) {
                bubble.draw();
            }
//Draw Pockets
            for (const Circle &pocket: pockets) {
                pocket.draw();
            }


        }

//Draw pool cue initially
            cueStick[0].setCenterX(balls[balls.size() - 1].getCenterX() + 464);
            cueStick[0].setCenterY(balls[balls.size() - 1].getCenterY());

            cueStick[1].setCenterX(balls[balls.size() - 1].getCenterX() + 254);
            cueStick[1].setCenterY(balls[balls.size() - 1].getCenterY());

            cueStick[2].setCenterX(balls[balls.size() - 1].getCenterX() + 64);
            cueStick[2].setCenterY(balls[balls.size() - 1].getCenterY());

            cueStick[3].setCenterX(balls[balls.size() - 1].getCenterX() + 52);
            cueStick[3].setCenterY(balls[balls.size() - 1].getCenterY());


            for (const Rect &section: cueStick) {
                glColor3f(section.getFillRed(), section.getFillGreen(), section.getFillBlue());
                section.rotatePoint(section, angle, balls[balls.size() - 1].getCenterX(),
                                    balls[balls.size() - 1].getCenterY());
            }
            shoot.draw(screen);
            morePower.draw(screen);
            lessPower.draw(screen);
            glFlush();
    }
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    switch (key) {
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
    switch (key) {
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
    angle = atan2(y - balls[balls.size() - 1].getCenterY() + 50, x - balls[balls.size() - 1].getCenterX());
    rise = y - balls[balls.size() - 1].getCenterY() + 50;
    run = x - balls[balls.size() - 1].getCenterX();
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && shotAngle == 0) {
        shotAngle = angle;
        shotRise = rise;
        shotRun = run;
        cout << shotAngle << endl;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && morePower.isOverlapping(x, y)) {
        morePower.pressDown();
    } else {
        morePower.release();
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && morePower.isOverlapping(x, y)) {
        shotPower += .2;
        cout << shotPower << endl;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && lessPower.isOverlapping(x, y)) {
        lessPower.pressDown();
    } else {
        lessPower.release();
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && lessPower.isOverlapping(x, y)) {
        shotPower = shotPower - .2;
        cout << shotPower << endl;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && shoot.isOverlapping(x, y)) {
        shoot.pressDown();
    } else {
        shoot.release();
    }

//Shooting

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && shoot.isOverlapping(x, y)) {
        cout << shotRise << "  " << shotRun << endl;
        if ((shotRise < 0) and (shotRun < 0)) {
            balls[balls.size() - 1].setVelocity(((shotRun / shotRise) * shotPower), ((shotRise / shotRun) * shotPower));
        }

        if ((shotRise >= 0) and (shotRun < 0)) {
            balls[balls.size() - 1].setVelocity(-((shotRun / shotRise) * shotPower),
                                                ((shotRise / shotRun) * shotPower));
        }

        if ((shotRise < 0) and (shotRun > 0)) {
            balls[balls.size() - 1].setVelocity(((shotRun / shotRise) * shotPower),
                                                -((shotRise / shotRun) * shotPower));
        }
        if ((shotRise >= 0) and (shotRun >= 0)) {
            balls[balls.size() - 1].setVelocity(-((shotRun / shotRise) * shotPower),
                                                -((shotRise / shotRun) * shotPower));
        }
    }

    glutPostRedisplay();
}

void timer(int dummy) {
    movement = 0;
    for (Circle &bubble: balls) {
        bubble.move(bubble.getXVelocity(), bubble.getYVelocity());
        if (bubble.getCenterX() < bubble.getRadius()) {
            bubble.bounceX();
            bubble.setCenterX(bubble.getRadius());
        } else if (bubble.getCenterX() > (WIDTH - bubble.getRadius())) {
            bubble.bounceX();
            bubble.setCenterX(WIDTH - bubble.getRadius());
        }
        if (bubble.getCenterY() < bubble.getRadius()) {
            bubble.bounceY();
            bubble.setCenterY(bubble.getRadius());
        } else if (bubble.getCenterY() > (HEIGHT - bubble.getRadius())) {
            bubble.bounceY();
            bubble.setCenterY(HEIGHT - bubble.getRadius());
        }
    }
    glutPostRedisplay();

//Ball collisions
    for (int i = 0; i < balls.size(); ++i) {
        for (int j = 0; j < pockets.size(); ++j) {
            if (balls[i].isOverlapping(pockets[j])) {

                balls.erase(balls.begin() + i);
                balls[i].setVelocity(0, 0);
                cout << "pocket collisions are being called" << endl;

            }
        }
        for (int j = i + 1; j < balls.size(); ++j) {
            if (balls[i].isOverlapping(balls[j])) {
                balls[i].collide(balls[j]);
            }
        }
        //Bumper collisions
//        for (int j = 0; j < bumpers.size(); ++j) {
//            if (balls[i].isOverlapping(bumpers[j])) {
//                if(j<2){
//                    balls[i].bounceX();
//                }
//                if(j>1) {
//                    balls[i].bounceY();
//                }
//
//                cout << "bumper collisions are being called" << endl;
//            }
//        }
    }


    for (int i = 0; i < balls.size(); ++i) {
        if (balls[i].getXVelocity() > 0.001) {
            balls[i].setXVelocity(balls[i].getXVelocity() - FRICTION);
            movement += (balls[i].getYVelocity());
        } else if (balls[i].getXVelocity() < -0.001) {
            balls[i].setXVelocity(balls[i].getXVelocity() + FRICTION);
            movement += -(balls[i].getXVelocity());
        } else {
            balls[i].setVelocity(0, 0);
        }

        if (balls[i].getYVelocity() > 0.001) {
            balls[i].setYVelocity(balls[i].getYVelocity() - FRICTION);
            movement += (balls[i].getYVelocity());
        } else if (balls[i].getYVelocity() < -0.001) {
            balls[i].setYVelocity(balls[i].getYVelocity() + FRICTION);
            movement += -(balls[i].getYVelocity());
        } else {
            balls[i].setVelocity(0, 0);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
    if (movement < .001) {
        screen = shotScreen;
    } else {
        screen = watchScreen;
        shotAngle = 0;
    }
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv) {

    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int) WIDTH, (int) HEIGHT);
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