// CSCI 173 Project
// Javier Escareno (Student ID: 300593944)
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <time.h>
#define PI 3.14159

using namespace std;

bool wireFrame = false; // Flag to toggle wireframe mode

const GLfloat lightAmbient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat lightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat lightPosition[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat materialAmbient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat materialDiffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat materialSpecular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat highShininess[] = { 100.0f };

clock_t startTime;
GLfloat camera = 0; // Using arrow keys to rotate left and right
// x, y, z positions
float xPos = 0;
float yPos = 0;
float zPos = 0;
// previous x, y positions and mouse coordinates
float xPrevious;
float yPrevious;
float xMouse;
float yMouse;
float t; // Timer
float windowWidth, windowHeight;

const int maxControlPoints = 15; // Maximum number of control points
int numControlPoints = 0;  // Number of control points
float controlPoints[maxControlPoints][3]; // Control Points Array

bool spacePressed = false; // Flag to indicate if space bar is pressed (reset scene)
bool displayControlPoints = true; // Flag to toggle display of control points


// Factorial calculation
float factorial(int n)
{
    int result = 1;
    for(int i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

// Binomial coefficient calculation
float binomialCoefficient(int n, int i)
{
    return (factorial(n)/((factorial(i)) * factorial(n - i)));
}

// Calculate Bezier curve point
void bezierCurve(float t, float &x, float &y, float &z)
{
    x = 0;
    y = 0;
    z = 0;
    for(int i = 0; i <= numControlPoints; i++)
    {
        float bernsteinCoefficient = (factorial(numControlPoints) / (factorial(i) * factorial(numControlPoints - i))*(pow((1 - t),(numControlPoints - i)))* pow(t,i));
        x += controlPoints[i - 1][0] * bernsteinCoefficient;
        y += controlPoints[i - 1][1] * bernsteinCoefficient;
        z += controlPoints[i - 1][2] * bernsteinCoefficient;
    }
}
// Resize callback function
static void resize(int width, int height)
{
    // Update the viewport and projection matrix for resizing the window
    double aspectRatio;
    windowWidth = (float)width;
    windowHeight = (float)height;
    aspectRatio = (double)width /(double)height;
    glViewport(0,0,(GLsizei) width,(GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45.0f, aspectRatio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// Display control points
void controlPointDisplay()
{
    glBegin(GL_POINTS);
    // Draw the control points
    for (int i = 0; i <= numControlPoints; i++)
    {
        if (controlPoints[i][0] == 0 && controlPoints[i][1] == 0 && controlPoints[i][2] == 0)
        {
            break;
        }
        glVertex3f(controlPoints[i - 1][0], controlPoints[i - 1][1], controlPoints[i - 1][2]);
    }
    glEnd();
}
// Display Bezier curve
void bezierDisplay()
{
    glBegin(GL_POINTS);
    // Generate points along the bezier curve
    for(float t = 0; t <= 1; t += 0.01)
    {
        float x, y, z;
        bezierCurve(t, x, y, z);
        glVertex3f(x, y, z);
    }
    glEnd();
}



// Idle callback function
static void idle(void)
{
    // Increment the parameter 't' for animation and calculate the new position of the Bezier curve
    if(clock() - startTime > 10)
    {
        t += 0.01;
        startTime = clock();
    }
    xPos = 0;
    yPos = 0;
    zPos =0;
    for(int i = 0; i <= numControlPoints; i++)
    {
        float binomial = binomialCoefficient(numControlPoints, i);
        float ti = pow(t, i);
        float ti1 = pow(1 - t, numControlPoints-i);

        xPos += binomial * ti1 * ti * controlPoints[i - 1][0];
        yPos += binomial * ti1 * ti * controlPoints[i - 1][1];
        zPos += binomial * ti1 * ti * controlPoints[i - 1][2];
    }
    if(t > 1) t = 1;

    glutPostRedisplay();
}

// Mouse callback function
void mouse(int button, int state, int x, int y)
{
    // Get new mouse coordinates for x, y
    // Adjust coordinates to match the right scale
    float scale = 70 * (windowWidth/windowHeight);
    xMouse = (float) (x-windowWidth/2)/scale;
    yMouse = (float) (windowHeight/2-y)/scale;

    switch(button)
    {
    // Left mouse click to add control points for drawing the curve
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN)
        {
            if(numControlPoints < 15)
            {
                float xCoordinate = xMouse;
                float yCoordinate = yMouse;

                controlPoints[numControlPoints][0] = xCoordinate;
                controlPoints[numControlPoints][1] = yCoordinate;
                controlPoints[numControlPoints][2] = rand() % 3;

                numControlPoints++;

                t = 0;
                xPrevious = xPos;
                yPrevious = yPos;
            }

        }
        break;

    // Right mouse click to toggle display of control points
    case GLUT_RIGHT_BUTTON:

        if(state == GLUT_DOWN){
            displayControlPoints = !displayControlPoints;
        }
        break;

    // Middle mouse click to RESET
    case GLUT_MIDDLE_BUTTON:
        if(spacePressed == GLUT_DOWN){
            camera = 0.0;
            numControlPoints = 0;
            spacePressed = false;
        }
        break;
    }
    glutPostRedisplay();
}

// Display callback function
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,100.0);

    if(wireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw the mesh in wireframe mode
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Toggle wireframe mode

    // Rotate the camera left and right
    glRotatef(camera, 0.0, 5.0, 0.0);

    // Display the control points
    glPushMatrix();
    if(displayControlPoints)
    {
        glPointSize(5);
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_POINTS);
        for (int i = 0; i < numControlPoints; i++)
        {
            glVertex3f(controlPoints[i][0], controlPoints[i][1], controlPoints[i][2]);
        }
    glEnd();
    }
    glPopMatrix();

    // Display the Bezier curve
    glPushMatrix();
    glPointSize(2);
    glBegin(GL_POINTS);
    for(float t = 0; t <= 1; t += 0.001)
    {
        float x, y, z;
        bezierCurve(t, x, y, z);
        glVertex3f(x, y, z);
    }
    glEnd();
    glPopMatrix();

    // Display the sphere
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(xPos, yPos, zPos);
    glutSolidSphere(0.5,40,40);
    glPopMatrix();

    glutSwapBuffers();
}

// Keyboard callback function
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;

    case 'w':
        wireFrame =! wireFrame;
        break;

    // Repeats animation by pressing ENTER key
    case 13:
        for(int i = 0; i < 15; i++){
            t=0;
        }
        break;

    // Spacebar to RESET
    case ' ':
        spacePressed = true;
        if(spacePressed){
            camera = 0.0;
            numControlPoints = 0;
            spacePressed = false;
        }
        break;
    }
}

// Special keyboard callback function
void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT : // Rotate camera left
        camera -= 2.0;
        break;

    case GLUT_KEY_RIGHT : // Rotate camera right
        camera += 2.0;
        break;
    }
    glutPostRedisplay();
}

// Initialization function
static void init(void)
{
    glClearColor(0.5f, 1.5f, 1.5f, 0.0f); // Set background color

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  materialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, highShininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    xPrevious = 0;
    yPrevious = 0;

    xMouse = 4.0;
    yMouse = 4.0;

    startTime = clock();
}

/* Program entry point */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Bezier Curve");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);
    glutMainLoop();
    return EXIT_SUCCESS;
}


