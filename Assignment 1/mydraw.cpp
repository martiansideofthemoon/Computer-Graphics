/*******************************
 *
 * CS475m Assignment 1 Basecode
 *
 * Parag Chaudhuri, 2016
 *
 *******************************/

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>

#include "mydraw_class.hpp"

//Window width
int win_width = 512;
//Window height
int win_height = 512;
canvas_t* canvas;


//Display callback
void display( void )
{
  //Flush the framebuffer to the screen
  glutSwapBuffers();
}

//Reshape callback
void reshape( int w, int h )
{
  if  ( h == 0 ) h = 1;

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  glOrtho( 0.0, (GLdouble)w, 0.0, (GLdouble)h, -1., 1. );
  glViewport( 0, 0, w, h );

  win_width = w;
  win_height = h;

  glutPostRedisplay();
}

//Keyboard callback
void keyboard( unsigned char key, int x, int y ) {
    switch(key) {
    //Exit on pressing escape
    case 27:
        exit(0);
        break;
    case 'c':
        canvas->clear();
        break;
    //Do something when 'N' is pressed
    case 'n':
        color_t* color = new color_t(255, 0, 0);
        canvas = new canvas_t(win_width, win_height, color);
        break;
    //Ignore all other keypresses
    }
    glutPostRedisplay();
}

//Mouse callback
void mouse(int button, int state, int x, int y)
{
   if (state == GLUT_DOWN)
     {
       if (button == GLUT_LEFT_BUTTON)
	 {
	   //Do something when the left mouse button is clicked
	 }
     }
   glutPostRedisplay();
}


int main (int argc, char *argv[])
{

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowSize( win_width, win_height );

  //Open a GLUT window
  glutCreateWindow( "MyDraw" );

  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutKeyboardFunc( keyboard );
  glutMouseFunc( mouse );

  glutMainLoop();
}
