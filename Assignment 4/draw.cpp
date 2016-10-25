#include <GL/glut.h>
#include <iostream>
#include <vector>
#include "Objects.hpp"
#include "Keyframes.hpp"

using namespace std;

Cycle* cycle;
Room* room;
Animate* animation;
bool is_recording = false;
int curr=0; //Index of current oprational node

void play_back(int);

//Our function for processing ASCII keys
void processNormalKeys(unsigned char key, int x, int y) {
  switch(key) {
    case '1':
      cycle->use_camera(0);
      break;
    case '2':
      cycle->use_camera(1);
      break;
    case '3':
      room->use_camera();
      cycle->use_camera(2);
      break;
    case 'l':
    case 'L':
      if (glIsEnabled(GL_LIGHT0))
        glDisable(GL_LIGHT0);
      else
        glEnable(GL_LIGHT0);
      break;
    case 'h':
    case 'H':
      if (glIsEnabled(GL_LIGHT1))
        glDisable(GL_LIGHT1);
      else
        glEnable(GL_LIGHT1);
      break;
    case 's':
    case 'S':
      if (is_recording) {
        animation->add_frame();
      }
      break;
    case 'p':
    case 'P':
      play_back(0);
      break;
    case 'c':
    case 'C':
      if (is_recording) {
        animation->clear();
      }
      break;
    case 'r':
    case 'R':
      is_recording = !is_recording;
      break;
  }
  if (key == 27)
  exit(0);
  glutPostRedisplay();
}

//Our function for processing Non-ASCII keys
void processSpecialKeys(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_PAGE_UP :
    cycle->rotate(0,0,1);
    break;
    case GLUT_KEY_PAGE_DOWN :
    cycle->rotate(0,0,-1);
    break;
    case GLUT_KEY_UP :
    cycle->pedal_cycle(1);
    break;
    case GLUT_KEY_DOWN :
    cycle->pedal_cycle(-1);
    break;
    case GLUT_KEY_LEFT:
    cycle->turn(1);
    break;
    case GLUT_KEY_RIGHT:
    cycle->turn(-1);
    break;
  }
  //Redraw
  glutPostRedisplay();
}

void display(void)
{

   //GLfloat light_position[] = { 0.0, 1.0, 0.0, 0.0 };
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor (0.0, 0.0, 0.0, 0.0);

  //glClearColor(0,0,0,1);
  cycle->adjust_headlight();
  room->adjust_light();
  cycle->render();
  room->render();
  glutSwapBuffers();
}

void play_back(int) {
  bool carry_on = animation->play_next();
  if (carry_on) {
    glutTimerFunc(50, play_back, 0);
  }
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
  gluLookAt(0.0, 1.0, 4.5,  // eye is at (0,0,5)
  0.0, 0.0, 0.0,      // center is at (0,0,0)
  0.0, 1.0, 0.0);      // up is in positive Y direction
  glShadeModel (GL_SMOOTH);

  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glEnable(GL_LIGHTING);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(640,640);
  glutCreateWindow("Hierarchical Modeling");
  glutDisplayFunc(display);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
  init();
  cycle = new Cycle("cycle.txt");
  cycle->turn(30);
  room = new Room("room.txt");
  animation = new Animate(cycle, room);
  room->use_camera();
  cycle->use_camera(2);
  glutMainLoop();
  return 0;
}
