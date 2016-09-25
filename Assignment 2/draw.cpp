#include <GL/glut.h>
#include <iostream>
#include <vector>
#include "Objects.hpp"
#include "Assembly.hpp"

using namespace std;

Cycle* cycle;
BaseObject *node;
BaseObject *node2;
BaseObject *node3;
int curr=0; //Index of current oprational node

//Our function for processing ASCII keys
void processNormalKeys(unsigned char key, int x, int y) {
  switch(key) {
    case '1':
      curr=0;
      break;
    case '2':
      curr=1;
      break;
    case '3':
      curr=2;
      break;
  }
  if (key == 27)
  exit(0);
}

//Our function for processing Non-ASCII keys
void processSpecialKeys(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_LEFT :
    cycle->rotate(0,-1,0);
    break;
    case GLUT_KEY_RIGHT :
    cycle->rotate(0,1,0);
    break;
    case GLUT_KEY_UP :
    cycle->rotate(-1,0,0);
    break;
    case GLUT_KEY_DOWN :
    cycle->rotate(1,0,0);
    break;
    case GLUT_KEY_PAGE_UP :
    cycle->pedal_cycle(-1);
    break;
    case GLUT_KEY_PAGE_DOWN :
    cycle->pedal_cycle(1);
    break;
    case GLUT_KEY_HOME:
    cycle->turn(1);
    break;
    case GLUT_KEY_END:
    cycle->turn(-1);
    break;
  }
  //Redraw
  glutPostRedisplay();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0,191.0/255,1,1);
  cycle->render();
  glutSwapBuffers();
}

void init(void)
{
  // Use depth buffering for hidden surface elimination.
  glEnable(GL_DEPTH_TEST);
  // Setup the view of the cube.
  glMatrixMode(GL_PROJECTION);
  //gluPerspective(field of view in degree, aspect ratio, Z near, Z far);
  gluPerspective(100.0, 1.0, 1.0, 20.0);

  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 3.0,  // eye is at (0,0,5)
  0.0, 0.0, 0.0,      // center is at (0,0,0)
  0.0, 1.0, 0.0);      // up is in positive Y direction
}

int main(int argc, char **argv)
{
  cycle = new Cycle("cycle.txt");
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(640,640);
  glutCreateWindow("Hierarchical Modeling");
  glutDisplayFunc(display);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
  init();
  glutMainLoop();
  return 0;
}
