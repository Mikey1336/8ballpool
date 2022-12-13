#include "circle.h"
#include "graphics.h"
#include <ctime>
#include <iostream>
#include <vector>
#include <random>
#include "rect.h"
#include "bumper.h"
#include "Button.h"
#include "PoolBall.h"

using namespace std;

const GLdouble WIDTH = 1500;
const GLdouble HEIGHT = 750;
GLdouble tableWidth, tableHeight;
int wd;
float angle;
point2D cursorTrack;
vector<double> rackPositions, displayPositions;
vector<Circle> pockets, sights;
vector<Bumper> bumpers;
vector<Rect> cueStick;
vector<PoolBall> ballsInPocket, ballsInPlay(15, PoolBall(0, 0, 0, 0));
Circle cueBall;
Rect playArea, leftBorder, topBorder, rightBorder, bottomBorder;
Button morePower({0, 1, .2}, {1350, 100}, 100, 50, "+ Power");
Button lessPower({1, .1, 0}, {1350, 200}, 100, 50, "- Power");
Button shoot({1, 1, 0}, {1350, 300}, 100, 50, "Take Shot");
int shotPower = 5;
double shotAngle = 0.0;
//Rise and Run for shot angle
double rise;
double run;
double shotRise;
double shotRun;
//Value For checking if balls are moving
double movement;

int BALL_RADIUS;
const double FRICTION = 0.05;

double WOOD_BORDER, BUMPER_WIDTH, EMPTY_BORDER, POCKET_RADIUS;

const color tableDark(0.1725, 0.5098, 0.3412);
const color tableLight(0.2804, 0.6451, 0.4412);
const color wood(0.6, 0.3, 0.2);
const color silver(192/255.0, 192/255.0, 192/255.0);
const color cueWood(253 / 255.0, 217 / 255.0, 181 / 255.0);
const color black(0, 0, 0);
const color white(1, 1, 1);
const color pink(241 / 255.0, 145 / 255.0, 155 / 255.0);
const color gray(100 / 255.0, 100 / 255.0, 100 / 255.0);

enum screenEnum {
    watchScreen,
    shotScreen,
    scratchScreen
};

screenEnum screen = shotScreen;

void rackBalls() {
    for (int i = 0; i < ballsInPocket.size(); i++) {
        if (ballsInPocket[i].getValue() == 8) {
            ballsInPlay[4] = ballsInPocket[i];
            ballsInPocket.erase(ballsInPocket.begin() + i);
        }
    }

    for (int i = 0; i < 15; i++) {
        if (i != 4) {
            int randI = rand() % ballsInPocket.size();
            ballsInPlay[i] = ballsInPocket[randI];
            ballsInPocket.erase(ballsInPocket.begin() + randI);
        }
    }

    for (int i = 0; i < 30; i += 2) {
        ballsInPlay[i/2].setCenter({rackPositions[i], rackPositions[i+1]});
    }
}

void init() {
    srand(time(0));
    tableWidth = WIDTH - 500;
    tableHeight = tableWidth / 2;
    BALL_RADIUS = tableWidth * 0.0225 / 2;
    POCKET_RADIUS = tableWidth * 0.018;
    WOOD_BORDER = tableWidth * 0.05;
    BUMPER_WIDTH = tableWidth * 0.02;
    EMPTY_BORDER = (HEIGHT - tableHeight - 2 * WOOD_BORDER - 2 * BUMPER_WIDTH) / 2;

    // Initialize table rectangles (playing area, wood borders)
    dimensions playAreaDimensions(tableWidth + 2 * BUMPER_WIDTH, tableHeight + 2 * BUMPER_WIDTH);
    point2D tableCenter(EMPTY_BORDER + WOOD_BORDER + BUMPER_WIDTH + (tableWidth / 2.0), HEIGHT / 2);
    playArea = Rect(tableDark, tableCenter, playAreaDimensions);

    double rackOriginX = playArea.getCenterX() + tableWidth/4;
    double rackOriginY = playArea.getCenterY();
    rackPositions = {rackOriginX, rackOriginY,
                     rackOriginX + 2 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY - 2 * BALL_RADIUS * sin(30 * PI / 180) - .01,
                     rackOriginX + 2 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY + 2 * BALL_RADIUS * sin(30 * PI / 180) + .01,
                     rackOriginX + 4 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY - 4 * BALL_RADIUS * sin(30 * PI / 180) - .02,
                     rackOriginX + 4 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY,
                     rackOriginX + 4 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY + 4 * BALL_RADIUS * sin(30 * PI / 180) + .02,
                     rackOriginX + 6 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY - 6 * BALL_RADIUS * sin(30 * PI / 180) - .03,
                     rackOriginX + 6 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY - 2 * BALL_RADIUS * sin(30 * PI / 180) - .01,
                     rackOriginX + 6 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY + 2 * BALL_RADIUS * sin(30 * PI / 180) + .01,
                     rackOriginX + 6 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY + 6 * BALL_RADIUS * sin(30 * PI / 180) + .03,
                     rackOriginX + 8 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY - 8 * BALL_RADIUS * sin(30 * PI / 180) - .04,
                     rackOriginX + 8 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY - 4 * BALL_RADIUS * sin(30 * PI / 180) - .02,
                     rackOriginX + 8 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY,
                     rackOriginX + 8 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY + 4 * BALL_RADIUS * sin(30 * PI / 180) + .02,
                     rackOriginX + 8 * BALL_RADIUS * cos(30 * PI / 180) + .01, rackOriginY + 8 * BALL_RADIUS * sin(30 * PI / 180) + .04
    };

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

    //MAKE pocketsis
    double radWidthDiff = BUMPER_WIDTH - POCKET_RADIUS;
    pockets.push_back(Circle(0, 0, 0, 1, playArea.getLeftX() + POCKET_RADIUS, playArea.getTopY() + POCKET_RADIUS, POCKET_RADIUS));
    pockets.push_back(Circle(0, 0, 0, 1, playAreaDimensions.width/2 + playArea.getLeftX(), playArea.getTopY(), POCKET_RADIUS));
    pockets.push_back(Circle(0, 0, 0, 1, playArea.getRightX() - POCKET_RADIUS, playArea.getTopY() + POCKET_RADIUS, POCKET_RADIUS));

    pockets.push_back(Circle(0, 0, 0, 1, playArea.getLeftX() + POCKET_RADIUS, playArea.getBottomY() - POCKET_RADIUS, POCKET_RADIUS));
    pockets.push_back(Circle(0, 0, 0, 1, playAreaDimensions.width/2 + playArea.getLeftX(), playArea.getBottomY(), POCKET_RADIUS));
    pockets.push_back(Circle(0, 0, 0, 1, playArea.getRightX() - POCKET_RADIUS, playArea.getBottomY() - POCKET_RADIUS, POCKET_RADIUS));

    // Initialize balls
    for (int i = 1; i < 16; i++) {
        ballsInPocket.push_back(PoolBall(0, 0, BALL_RADIUS, i));
    }
    rackBalls();

    //Cue Ball
    cueBall = Circle(1, 1, 1, 1, playArea.getCenterX() - tableWidth/4, playArea.getCenterY(), BALL_RADIUS);

    //Bumpers
    //Left and right
    double tangentCorner = BUMPER_WIDTH * tan(52*PI/180);
    double tangentMiddle = BUMPER_WIDTH/4;
    bumpers.push_back(Bumper(tableLight, playArea.getLeftX(), playArea.getTopY() + BUMPER_WIDTH, playArea.getLeftX() + BUMPER_WIDTH, playArea.getTopY() + BUMPER_WIDTH + tangentCorner, playArea.getLeftX() + BUMPER_WIDTH, playArea.getBottomY() - BUMPER_WIDTH - tangentCorner, playArea.getLeftX(), playArea.getBottomY() - BUMPER_WIDTH));
    bumpers.push_back(Bumper(tableLight, playArea.getRightX(), playArea.getTopY() + BUMPER_WIDTH, playArea.getRightX() - BUMPER_WIDTH, playArea.getTopY() + BUMPER_WIDTH + tangentCorner, playArea.getRightX() - BUMPER_WIDTH, playArea.getBottomY() - BUMPER_WIDTH - tangentCorner, playArea.getRightX(), playArea.getBottomY() - BUMPER_WIDTH));

    //Top and bottom
    bumpers.push_back(Bumper(tableLight, playArea.getLeftX() + BUMPER_WIDTH, playArea.getTopY(), playArea.getLeftX() + BUMPER_WIDTH + tangentCorner, playArea.getTopY() + BUMPER_WIDTH, playArea.getCenterX() - POCKET_RADIUS - tangentMiddle, playArea.getTopY() + BUMPER_WIDTH, playArea.getCenterX() - POCKET_RADIUS, playArea.getTopY()));
    bumpers.push_back(Bumper(tableLight, playArea.getRightX() - BUMPER_WIDTH, playArea.getTopY(), playArea.getRightX() - BUMPER_WIDTH - tangentCorner, playArea.getTopY() + BUMPER_WIDTH, playArea.getCenterX() + POCKET_RADIUS + tangentMiddle, playArea.getTopY() + BUMPER_WIDTH, playArea.getCenterX() + POCKET_RADIUS, playArea.getTopY()));
    bumpers.push_back(Bumper(tableLight, playArea.getLeftX() + BUMPER_WIDTH, playArea.getBottomY(), playArea.getLeftX() + BUMPER_WIDTH + tangentCorner, playArea.getBottomY() - BUMPER_WIDTH, playArea.getCenterX() - POCKET_RADIUS - tangentMiddle, playArea.getBottomY() - BUMPER_WIDTH, playArea.getCenterX() - POCKET_RADIUS, playArea.getBottomY()));
    bumpers.push_back(Bumper(tableLight, playArea.getRightX() - BUMPER_WIDTH, playArea.getBottomY(), playArea.getRightX() - BUMPER_WIDTH - tangentCorner, playArea.getBottomY() - BUMPER_WIDTH, playArea.getCenterX() + POCKET_RADIUS + tangentMiddle, playArea.getBottomY() - BUMPER_WIDTH, playArea.getCenterX() + POCKET_RADIUS, playArea.getBottomY()));

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

    playArea.draw();

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

    for (Circle pocket : pockets) {
        pocket.draw();
    }

    for (PoolBall ball : ballsInPlay) {
        ball.draw();
    }

    cueBall.draw();

    for (Bumper bumper : bumpers) {
        bumper.draw();
    }
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
            glFlush();  // Render now
        }
        case scratchScreen:{
            drawTable();
            glFlush();
        }

        case shotScreen: {
            drawTable();

        }

            //Draw pool cue initially
            cueStick[0].setCenterX(cueBall.getCenterX() + 464);
            cueStick[0].setCenterY(cueBall.getCenterY());

            cueStick[1].setCenterX(cueBall.getCenterX() + 254);
            cueStick[1].setCenterY(cueBall.getCenterY());

            cueStick[2].setCenterX(cueBall.getCenterX() + 64);
            cueStick[2].setCenterY(cueBall.getCenterY());

            cueStick[3].setCenterX(cueBall.getCenterX() + 52);
            cueStick[3].setCenterY(cueBall.getCenterY());


            for (const Rect &section: cueStick) {
                glColor3f(section.getFillRed(), section.getFillGreen(), section.getFillBlue());
                section.rotatePoint(section, angle, cueBall.getCenterX(),
                                    cueBall.getCenterY());
            }
            shoot.draw(screen);
            morePower.draw(screen);
            lessPower.draw(screen);

            double rightEmptyBorderWidth = WIDTH - rightBorder.getRightX();
            for (int i = 0; i < ballsInPocket.size(); i++) {
                ballsInPocket[i].setCenter({rightBorder.getRightX() + (i%4+1)*rightEmptyBorderWidth/5, playArea.getCenterY() + 50});
                ballsInPocket[i].draw();
            }

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
    angle = atan2(y - cueBall.getCenterY() + 50, x - cueBall.getCenterX());
    rise = y - cueBall.getCenterY() + 50;
    run = x - cueBall.getCenterX();
    cursorTrack.x = x;
    cursorTrack.y = y;

    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

    //Scratches
    if (screen == scratchScreen){
        cout << "scratchin balls" << endl;
        cueBall.setVelocity(0,0);
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && screen == scratchScreen) {

            cueBall.move(cursorTrack.x - 350, cursorTrack.y - 350);
            shotAngle = 0;
            angle = 0;
            screen = watchScreen;
        }
    }

    if (screen == shotScreen) {
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
                cueBall.setVelocity(((shotRun / shotRise) * shotPower),
                                                    ((shotRise / shotRun) * shotPower));

            }

            if ((shotRise >= 0) and (shotRun < 0)) {
                cueBall.setVelocity(-((shotRun / shotRise) * shotPower),
                                                    ((shotRise / shotRun) * shotPower));
            }

            if ((shotRise < 0) and (shotRun > 0)) {
                cueBall.setVelocity(((shotRun / shotRise) * shotPower),
                                                    -((shotRise / shotRun) * shotPower));
            }
            if ((shotRise >= 0) and (shotRun >= 0)) {
                cueBall.setVelocity(-((shotRun / shotRise) * shotPower),
                                                    -((shotRise / shotRun) * shotPower));
            }
            shotAngle = 0;
            angle = 0;
        }

    }

    glutPostRedisplay();
}

void timer(int dummy) {
    for (Circle pocket : pockets) {
        if (cueBall.isOverlapping(pocket)) {
            cueBall.setCenter(playArea.getCenterX() - tableWidth / 4, playArea.getCenterY());
            cueBall.setVelocity(0,0);
            screen = scratchScreen;

        }
    }

    if ((cueBall.getXVelocity() > .01 or cueBall.getXVelocity() < -.01 or cueBall.getYVelocity() > .01 or cueBall.getYVelocity() < -.01 ) and (screen != scratchScreen)){
        screen = watchScreen;
    }
    else if (!(cueBall.getXVelocity() > .01 or cueBall.getXVelocity() < -.01 or cueBall.getYVelocity() > .01 or cueBall.getYVelocity() < -.01 ) and (screen != scratchScreen)){
            screen = shotScreen;

        }

    // Bumper collisions
    // Colored ball collisions
    for (Circle ball: ballsInPlay) {
        ball.move(ball.getXVelocity(), ball.getYVelocity());
        // Colored balls friction
        if (ball.getXVelocity() < -0.01) {
            ball.setXVelocity(ball.getXVelocity() + FRICTION);
        } else if (ball.getXVelocity() > 0.01) {
            ball.setXVelocity(ball.getXVelocity() - FRICTION);
        } else {
            ball.setXVelocity(0);
        }
        if (ball.getYVelocity() < -0.01) {
            ball.setYVelocity(ball.getYVelocity() + FRICTION);
        } else if (ball.getYVelocity() > 0.01) {
            ball.setYVelocity(ball.getYVelocity() - FRICTION);
        } else {
            ball.setYVelocity(0);
        }
        for (Bumper bumper : bumpers) {
            bumper.collide(ball);
        }
    }

    // Cue ball collisions
    cueBall.move(cueBall.getXVelocity(), cueBall.getYVelocity());
    // Cue ball friction
    if (cueBall.getXVelocity() < -0.01) {
        cueBall.setXVelocity(cueBall.getXVelocity() + FRICTION);
    } else if (cueBall.getXVelocity() > 0.01) {
        cueBall.setXVelocity(cueBall.getXVelocity() - FRICTION);
    } else {
        cueBall.setXVelocity(0);
    }
    if (cueBall.getYVelocity() < -0.01) {
        cueBall.setYVelocity(cueBall.getYVelocity() + FRICTION);
    } else if (cueBall.getYVelocity() > 0.01) {
        cueBall.setYVelocity(cueBall.getYVelocity() - FRICTION);
    } else {
        cueBall.setYVelocity(0);
    }
    for (Bumper bumper : bumpers) {
        bumper.collide(cueBall);
    }

    // Ball collisions
    for (int i = 0; i < ballsInPlay.size(); ++i) {
        for (int j = 0; j < pockets.size(); ++j) {
            if (ballsInPlay[i].isOverlapping(pockets[j])) {
                ballsInPocket.push_back(ballsInPlay[i]);
                ballsInPlay.erase(ballsInPlay.begin() + i);
                ballsInPlay[i].setVelocity(0, 0);
                cout << "pocket collisions are being called" << endl;
            }
        }
        for (int j = i + 1; j < ballsInPlay.size(); ++j) {
            if (ballsInPlay[i].isOverlapping(ballsInPlay[j])) {
                ballsInPlay[i].collide(ballsInPlay[j]);
            }
            if (ballsInPlay[i].isOverlapping(cueBall)) {
                ballsInPlay[i].collide(cueBall);
            }
        }
    }

    if (movement < .001) {
        screen = shotScreen;
    } else {
        screen = watchScreen;
        shotAngle = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
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