//pongGame.cpp : A bouncing ball minigame

//#include <windows.h> //the windows include file, required by all windows applications
#include <GL/glut.h> //the glut file for windows operations
                     // it also includes gl.h and glu.h for the openGL library calls
#include <math.h>
#include <bits/stdc++.h>
#include <string>

#define PI 3.1415926535898

double xpos, ypos, ydir, xdir;         // x and y position for house to be drawn
double sx, sy, squash;          // xy scale factors
double rot, rdir;             // rotation
int SPEED = 50;        // speed of timer call back in msecs
double windowWidth, windowHeight;
double paddleWidth, paddleHeight;
double paddleXpos, leftPaddleYpos;
double rightPaddleXpos, rightPaddleYpos;
double paddleSpeed = 3.0;
int leftPlayerScore, rightPlayerScore;
std::map<int, bool> keyPressed = {{'w', false}, {'s', false},\
                                   {'o', false}, {'l', false}};

GLfloat T1[16] = {1.,0.,0.,0.,\
                  0.,1.,0.,0.,\
                  0.,0.,1.,0.,\
                  0.,0.,0.,1.};
GLfloat S[16] = {1.,0.,0.,0.,\
                 0.,1.,0.,0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};
GLfloat T[16] = {1.,0.,0.,0.,\
                 0., 1., 0., 0.,\
                 0.,0.,1.,0.,\
                 0.,0.,0.,1.};

#define PI 3.1415926535898
GLint circle_points = 100;
void MyCircle2f(GLfloat centerx, GLfloat centery, GLfloat radius){
    GLint i;
    GLdouble angle;
    glBegin(GL_POLYGON);
    for (i = 0; i < circle_points; i++) {
        angle = 2*PI*i/circle_points;
        glVertex2f(centerx+radius*cos(angle), centery+radius*sin(angle));
    }
    glEnd();
}

GLfloat RadiusOfBall = 3.;
// Draw the ball, centered at the origin
void draw_ball() {
    glColor3f(0.6f,0.0f,0.7f);
    MyCircle2f(0.,0.,RadiusOfBall);
}

void drawScore(const char *score, float x, float y) {
    glRasterPos2f(x, y);
    while (*score) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *score);
        score++;
    }
}

void drawLeftPaddle() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0, leftPaddleYpos);
    glVertex2f(paddleWidth, leftPaddleYpos);
    glVertex2f(paddleWidth, leftPaddleYpos+ paddleHeight);
    glVertex2f(0.0, leftPaddleYpos + paddleHeight);
    glEnd();
}

void drawRightPaddle() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(windowWidth, rightPaddleYpos);
    glVertex2f(windowWidth - paddleWidth, rightPaddleYpos);
    glVertex2f(windowWidth - paddleWidth, rightPaddleYpos + paddleHeight);
    glVertex2f(windowWidth, rightPaddleYpos + paddleHeight);
    glEnd();
}

void keyboard(unsigned char key, int x, int y) {
    keyPressed[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
    keyPressed[key] = false;
}

void updatePaddles() {
    if (keyPressed['w']) {
        leftPaddleYpos += paddleSpeed;
        if (leftPaddleYpos + paddleHeight >= windowHeight)
            leftPaddleYpos = windowHeight - paddleHeight;
    }

    if (keyPressed['s']) {
        leftPaddleYpos -= paddleSpeed;
        if (leftPaddleYpos < 0.0)
            leftPaddleYpos = 0.0;
    }

    if (keyPressed['o']) {
        rightPaddleYpos += paddleSpeed;
        if (rightPaddleYpos + paddleHeight >= windowHeight)
            rightPaddleYpos = windowHeight - paddleHeight;
    }

    if (keyPressed['l']) {
        rightPaddleYpos -= paddleSpeed;
        if (rightPaddleYpos < 0.0)
            rightPaddleYpos = 0.0;
    }
    glutPostRedisplay();
}

void drawCenterLine() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);
    glBegin(GL_LINES);
    glVertex2f(windowWidth * 0.5, 0.0);
    glVertex2f(windowWidth * 0.5, windowHeight);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void checkScore(const int score, const int player) {
    if (score == 5) {
        std::string name = (player == 0)? "Left player": "Right player";
        std::cout << name << " won the game!!" << "\n";
        exit(1);
    }
}

bool collidedLeft() {
    bool collisionX = paddleWidth >= xpos - RadiusOfBall &&\
        xpos >= 0.0;
    bool collisionY = leftPaddleYpos + paddleHeight >= ypos - RadiusOfBall &&\
        ypos >= leftPaddleYpos;
    return collisionY && collisionX;
}

bool collidedRight() {
    bool collisionX = windowWidth >= xpos - RadiusOfBall &&\
        xpos + RadiusOfBall >= windowWidth - RadiusOfBall;
    bool collisionY = rightPaddleYpos + paddleHeight >= ypos - RadiusOfBall &&\
        ypos + RadiusOfBall >= rightPaddleYpos;
    return collisionY && collisionX;
}

void Display(void)
{
    // swap the font and back buffers (double buffering)
    glutSwapBuffers();

    // Check and update paddles
    updatePaddles();

    //clear all pixels with the specified clear color
    glClear(GL_COLOR_BUFFER_BIT);

    // Position of the ball
    ypos = ypos + ydir * 0.5 - (1. - sy) * RadiusOfBall;
    xpos = xpos + xdir * 0.5 - (1.0 - sx) * RadiusOfBall;

    // Vertical wall collision
    if (ypos == windowHeight - RadiusOfBall || ypos == RadiusOfBall)
        ydir *= -1;

    if (collidedLeft()) {
        xdir = 1;
        if (ypos - RadiusOfBall <= leftPaddleYpos - RadiusOfBall * 0.9 &&\
            ypos > RadiusOfBall &&\
            xpos < paddleWidth) {
            ydir = -1;
        }
        if (ypos + RadiusOfBall >= leftPaddleYpos + paddleHeight + RadiusOfBall * 0.9 &&\
            ypos < windowHeight - RadiusOfBall &&\
            xpos < paddleWidth) {
            ydir = 1;
        }
    }

    if (collidedRight()) {
        xdir = -1;
        if (ypos - RadiusOfBall <= rightPaddleYpos - RadiusOfBall * 0.9 &&\
            ypos > RadiusOfBall)
            ydir = -1;
        if (ypos + RadiusOfBall >= rightPaddleYpos + paddleHeight  + RadiusOfBall * 0.9 &&\
            ypos < windowHeight - RadiusOfBall)
            ydir = 1;
    }

    // Left wall collision
    if (xpos <= RadiusOfBall * 0.6) {
        xpos = windowWidth * 0.5;
        ypos = windowHeight * 0.5;
        ydir = -1;
        rightPlayerScore++;
        checkScore(rightPlayerScore, 1);
    }

    // Right wall collision
    if (xpos >= windowWidth - 0.6 * RadiusOfBall) {
        xpos = windowWidth * 0.5;
        ypos = windowHeight * 0.5;
        ydir = -1;
        leftPlayerScore++;
        checkScore(leftPlayerScore, 0);
    }

    glPushMatrix();

    //Translate the bouncing ball to its new position
    T[12]= xpos;
    T[13] = ypos;
    glLoadMatrixf(T);

    T1[13] = -RadiusOfBall;
    // Translate ball back to center
    glMultMatrixf(T1);
    S[0] = sx;
    S[5] = sy;
    // Scale the ball about its bottom
    glMultMatrixf(S);
    T1[13] = RadiusOfBall;
    // Translate ball up so bottom is at the origin
    glMultMatrixf(T1);

    draw_ball();
    glPopMatrix();
    drawLeftPaddle();
    drawRightPaddle();
    drawCenterLine();
    char scoreText[20];
    sprintf(scoreText, "%d", leftPlayerScore);
    drawScore(scoreText, windowWidth * 0.25, windowHeight - 20);
    sprintf(scoreText, "%d", rightPlayerScore);
    drawScore(scoreText, windowWidth * 0.75, windowHeight - 20);
    glutPostRedisplay();
}

void reshape (int w, int h)
{
    // on reshape and on startup, keep the viewport to be the entire size of the window
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    // keep our logical coordinate system constant
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}

void init(void){
    //set the clear color to be white
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // initial position set to 0,0
    windowHeight = 250;
    windowWidth = 160.0;
    paddleWidth = 5.0;
    paddleHeight = 30.0;
    leftPaddleYpos = 60.0;
    rightPaddleYpos = 60.0;
    leftPlayerScore = 0, rightPlayerScore = 0;
    xpos = windowWidth * 0.5; ypos = RadiusOfBall; xdir = 1; ydir = 1;
    sx = 1.; sy = 1.; squash = 0.9;
    rot = 0;
}

int main(int argc, char* argv[])
{
    // Initialize the GLUT library and process any comand line arguments.
    glutInit( & argc, argv );
    // with GLUT_DOUBLE we enable double buffering, i.e. smooth animation.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    // Set the initial window size to 320 x 240 pixels.
    glutInitWindowSize (420, 640);
    glutCreateWindow("Example");
    // Set up the initial OpenGL state.
    init();
    // Register the display callback function to render the scene.
    glutDisplayFunc(Display);
    // Register the reshape callback function to handle window resizing.
    glutReshapeFunc(reshape);
    glutKeyboardUpFunc(keyboardUp);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 1;
}
