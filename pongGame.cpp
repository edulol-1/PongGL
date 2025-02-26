//Example2_4.cpp : A bouncing ball

//#include <windows.h> //the windows include file, required by all windows applications
#include <GL/glut.h> //the glut file for windows operations
                     // it also includes gl.h and glu.h for the openGL library calls
#include <math.h>

#define PI 3.1415926535898


double xpos, ypos, ydir, xdir;         // x and y position for house to be drawn
double sx, sy, squash;          // xy scale factors
double rot, rdir;             // rotation
int SPEED = 50;        // speed of timer call back in msecs
double windowWidth, windowHeight;

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

GLfloat RadiusOfBall = 5.;
// Draw the ball, centered at the origin
void draw_ball() {
    glColor3f(0.6,0.0,0.7);
    MyCircle2f(0.,0.,RadiusOfBall);
}

void Display(void)
{
    // swap the font and back buffers (double buffering)
    glutSwapBuffers();

    //clear all pixels with the specified clear color
    glClear(GL_COLOR_BUFFER_BIT);
    // 160 is max X value in our world

    ypos = ypos + ydir * 0.5 - (1. - sy) * RadiusOfBall;
    if (ypos == windowHeight - RadiusOfBall)
        ydir = -1;

    else if (ypos < RadiusOfBall)
        ydir = 1;

    xpos = xpos + xdir * 0.5 - (1.0 - sx) * RadiusOfBall;
    // Left wall collision
    if (xpos < RadiusOfBall)
        xdir = 1;
    // Right wall collision
    else if (xpos == windowWidth - RadiusOfBall)
        xdir = -1;

    /*  //reset transformation state
        glLoadIdentity();

        // apply translation
        glTranslatef(xpos,ypos, 0.);

        // Translate ball back to center
        glTranslatef(0.,-RadiusOfBall, 0.);
        // Scale the ball about its bottom
        glScalef(sx,sy, 1.);
        // Translate ball up so bottom is at the origin
        glTranslatef(0.,RadiusOfBall, 0.);
        // draw the ball
        draw_ball();
    */

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
    glClearColor(0.0f, 0.9f, 0.0f, 0.0f);
    // initial position set to 0,0
    windowHeight = 200.0;
    windowWidth = 160.0;
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
    // Enter the GLUT event processing loop.
    // Won't return until the program is finished.
    glutMainLoop();

    return 1;
}
