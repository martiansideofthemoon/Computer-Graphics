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
int LINE_DRAWING = 0;
int TRI_DRAWING = 2;
int mode = 1;
int pen_color[3] = {255, 0, 0};
int point1[2] = {-1, -1};
int point2[2] = {-1, -1};
int point3[2] = {-1, -1};
//Display callback
void display( void )
{
  canvas->draw_grid();
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
    case '1': {
        // Enter line drawing mode
        mode = LINE_DRAWING;
    } break;
    case '2': {
        // Enter triangle drawing mode
        mode = TRI_DRAWING;
    } break;
    case 'c': {
        canvas->clear();
    } break;
    case 'n': {
        // Do something when 'N' is pressed
        cout << "Enter the RGB" << endl;
        int r = 0, g = 0, b = 0;
        //cin >> r >> g >> b;
        color_t* color = new color_t(r, g, b);
        canvas = new canvas_t(win_width, win_height, color);
    } break;
    case 's': {
        // Do something when 'S' is pressed
        cout << "Enter file name" << endl;
        string s = "";
        cin >> s;
        canvas->save(s);
    } break;
    case 'l': {
        // Do something when 'l' is pressed
        cout << "Enter file name" << endl;
        string s = "";
        cin >> s;
        canvas->load(s);
        canvas->draw_list();
    } break;
    //Ignore all other keypresses
    }
    glutPostRedisplay();
}

//Mouse callback
void mouse(int button, int state, int x, int y) {
    y = 512 - y;
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            if (mode == LINE_DRAWING) {
                if (point1[0] == -1 || point1[1] == -1) {
                    point1[0] = x;
                    point1[1] = y;
                } else {
                    point2[0] = x;
                    point2[1] = y;
                    color_t* color = new color_t(pen_color[0], pen_color[1], pen_color[2]);
                    point_t* p1 = new point_t(point1[0], point1[1], color);
                    point_t* p2 = new point_t(point2[0], point2[1], color);
                    line_t* l1 = new line_t(p1, p2, color);
                    l1->draw(canvas, false);
                    point1[0] = -1;
                    point1[1] = -1;
                    point2[0] = -1;
                    point2[1] = -1;
                }
            } else if(mode == TRI_DRAWING) {
                if (point1[0] == -1 || point1[1] == -1) {
                    point1[0] = x;
                    point1[1] = y;
                } else if(point2[0] == -1 || point2[1] == -1) {
                    point2[0] = x;
                    point2[1] = y;
                } else {
                    point3[0] = x;
                    point3[1] = y;
                    color_t* color = new color_t(pen_color[0], pen_color[1], pen_color[2]);
                    point_t* p1 = new point_t(point1[0], point1[1], color);
                    point_t* p2 = new point_t(point2[0], point2[1], color);
                    point_t* p3 = new point_t(point3[0], point3[1], color);
                    triangle_t* t1 = new triangle_t(p1, p2, p3, color);
                    t1->draw(canvas, false);
                    point1[0] = -1;
                    point1[1] = -1;
                    point2[0] = -1;
                    point2[1] = -1;
                    point3[0] = -1;
                    point3[1] = -1;
                }
            }
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
  color_t* color = new color_t(0, 0, 0);
  canvas = new canvas_t(win_width, win_height, color);
  glutMainLoop();
}
