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
  gluLookAt(0.0, 0.0, 2.0,  // eye is at (0,0,5)
  0.0, 0.0, 0.0,      // center is at (0,0,0)
  0.0, 1.0, 0.0);      // up is in positive Y direction
}

int main(int argc, char **argv)
{
  // //Parent Node
  // float pedal_position[2][4] = {
  //   {0,0,0,1}, // centre position
  //   {0,0,1,1}, // normal direction, Z direction
  // };
  // float wheel_position[2][4] = {
  //   {0,0,0,1}, // centre position
  //   {0,0,1,1}, // normal direction, Z direction
  // };
  // float wheel_colors[2][4] = {
  //   {0.4, 0.4, 0.4, 1.0}, // tire color
  //   {0.4, 0.4, 0.4, 1} // spoke color
  // };
  // float pedal_colors[2][4] = {
  //   {0, 0, 0, 1.0}, // pedal color
  //   {0.4, 0.4, 0.4, 1} // shaft color
  // };
  // float chain_color[4] = {1, 0, 0, 1};
  // //node = new Wheel(wheel_position, wheel_colors, 40, 1, 0.07);
  // //node = new Pedal(pedal_position, pedal_colors, 0.3, 0.5);
  // //node2 = new Wheel(wheel_position, wheel_colors, 5, 0.1, 0.01);
  // //node3 = new Chain(pedal_position, chain_color, 0.1, 0.03, 1.5);
  // //node->add_child(node2);
  // //node2->add_child(node3);
  // float frame_len[2] = {1.5, 1};
  // //node = new Frame(pedal_position, chain_color, 1, frame_len, 0.05);
  cycle = new Cycle("arbit.txt");
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
