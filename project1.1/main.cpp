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
bool WireFrame= false;
float i =0;
const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };
float xpos =0;
float ypos =0;
//variables for angle change
float x;
float y;
float theta;
//slow down rotation
clock_t startTime;
//change sphere radius rotation
float r;
int dir;  //toggle value

float Wwidth,Wheight;
/* GLUT callback Handlers */
static void resize(int width, int height)
{
double Ratio;
Wwidth = (float)width;
Wheight = (float)height;
Ratio= (double)width /(double)height;
glViewport(0,0,(GLsizei) width,(GLsizei) height);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective (45.0f,Ratio,0.1f, 100.0f);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}
static void display(void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,100.0);
if(WireFrame)
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draw Our Mesh In
else
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Toggle WIRE
// your code here
glTranslatef(xpos,ypos,1.0); // initilize the translate x and y pos
glutSolidSphere (r,40,40);// Drawing a sphere








glutSwapBuffers();
}
static void key(unsigned char key, int x, int y)
{
switch (key)
{
case 27 :
case 'q':
exit(0);
break;
case 'w':
WireFrame =!WireFrame;
break;
}
}
void Specialkeys(int key, int x, int y)
{
switch(key)
{
case GLUT_KEY_UP:
break;
}
glutPostRedisplay();
}
static void idle(void)
{
  x=xpos;
  y=ypos;
  if (clock()- startTime>60){
  xpos = x*cos(theta*PI/180)-y*sin(theta*PI/180);
  ypos = y*cos(theta*PI/180)+x*sin(theta*PI/180);
  startTime=clock();
  r>1.5? dir =-1:(r<0.1?dir=1:NULL);
  r+= dir*0.01;// increases sphere size slowly overtime
  }



// Use parametric equation with t increment for xpos and y pos
// Don't need a loop
glutPostRedisplay();
}
void mouse(int btn, int state, int x, int y){
float scale = 100*(Wwidth/Wheight);
switch(btn){
case GLUT_LEFT_BUTTON:
if(state==GLUT_DOWN){
// get new mouse coordinates for x,y
// use scale to match right
}
break;
}
glutPostRedisplay();
};
static void init(void)
{
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // assign a color you
glEnable(GL_NORMALIZE);
glEnable(GL_COLOR_MATERIAL);
glEnable(GL_DEPTH_TEST);
glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
glShadeModel(GL_SMOOTH);
glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
glEnable(GL_LIGHT0);
glEnable(GL_NORMALIZE);
glEnable(GL_LIGHTING);
//initilize x and y pos
xpos = 1.0;
ypos = 1.0;
theta = 2.0;
r = 0.2;
dir=1.0;
//Timer
startTime = clock();//STart clock

}
/* Program entry point */
int main(int argc, char *argv[])
{
glutInit(&argc, argv);
glutInitWindowSize(800,600);
glutInitWindowPosition(0,0);
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
glutCreateWindow("Project Assignment 2");
init();
glutReshapeFunc(resize);
glutDisplayFunc(display);
glutMouseFunc(mouse);
glutKeyboardFunc(key);
glutSpecialFunc(Specialkeys);
glutIdleFunc(idle);
glutMainLoop();
return EXIT_SUCCESS;
}
