#include "mydraw_class.hpp"
#include <GL/glut.h>
#include <iostream>
using namespace std;
//-------------------
//color_t methods

color_t::color_t():r(0.0),g(0.0),b(0.0) { }

color_t::color_t(const float _r, const float _g, const float _b)
         :r(_r),g(_g),b(_b) { }

void color_t::set(const float _r, const float _g, const float _b) {
  r=_r; g=_g; b=_b;
}

float color_t::R(void) { return r; }
float color_t::G(void) { return g; }
float color_t::B(void) { return b; }


//---------------------


//point_t methods
point_t::point_t() {
    x = 0;
    y = 0;
    color = new color_t(0,0,0);
}

point_t::point_t(int _x, int _y, color_t* c) {
    x = _x;
    y = _y;
    color = c;
}

int point_t::get_x() {
    return x;
}

int point_t::get_y() {
    return y;
}

color_t* point_t::get_color() {
    return color;
}

void point_t::draw(canvas_t* canvas) {
    canvas->put_point(this);
}

// canvas methods
canvas_t::canvas_t(int width, int height, color_t* background) {
    this->width = width;
    this->height = height;
    this->background = background;
    image.resize(height);
    for (int i = 0; i < height; i++) {
        image[i].resize(width);
    }
    clear();
    draw_grid();
}

void canvas_t::clear() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            point_t* p = new point_t(i, j, background);
            image[i][j] = p;
        }
    }
    draw_grid();
}

void canvas_t::put_point(point_t* point) {
    image[point->get_x()][point->get_y()] = point;
}


void canvas_t::draw_grid() {
    GLubyte checkImage[height][width][3];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            color_t* color = image[i][j]->get_color();
            checkImage[i][j][0] = (GLubyte) color->R();
            checkImage[i][j][1] = (GLubyte) color->G();
            checkImage[i][j][2] = (GLubyte) color->B();
      }
    }
    glRasterPos2i(0, 0);
    glDrawPixels(width, height, GL_RGB,
                 GL_UNSIGNED_BYTE, checkImage);
}

