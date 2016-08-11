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
}

void canvas_t::clear() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            point_t* p = new point_t(i, j, background);
            image[i][j] = p;
        }
    }
}

void canvas_t::put_point(point_t* point) {
    image[point->get_y()][point->get_x()] = point;
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


//line_t methods
line_t::line_t(point_t* p1, point_t* p2, color_t* color) {
    this->p1 = p1;
    this->p2 = p2;
    this->color = color;
}

point_t* line_t::get_p1() {
    return p1;
}

point_t* line_t::get_p2() {
    return p2;
}

color_t* line_t::get_color() {
    return color;
}

void line_t::draw(canvas_t* canvas) {
    int x1 = p1->get_x();
    int x2 = p2->get_x();
    int y1 = p1->get_y();
    int y2 = p2->get_y();
    if (x2 < x1) {
        // Takes care of half the cases
        swap(x1, x2);
        swap(y1, y2);
    }
    if (abs(y1-y2) > abs(x1-x2)) {
        // Slope is more than 1 or less than -1
        if (y2 > y1) {
            int x = x1;
            int error = 0;
            for (int y = y1; y <= y2; y += 1) {
                point_t* p = new point_t(x, y, color);
                p->draw(canvas);
                error += (x2 - x1);
                if (error*2 >= (y2 - y1)) {
                    x += 1;
                    error -= (y2 - y1);
                }
            }
        } else {
            int x = x1;
            int error = 0;
            for (int y = y1; y >= y2; y -= 1) {
                point_t* p = new point_t(x, y, color);
                p->draw(canvas);
                error += (x2 - x1);
                if (error*2 >= (y1 - y2)) {
                    x += 1;
                    error -= (y1 - y2);
                }
            }
        }

    } else {
        // Slope is less than 1 and more than -1
        if (y2 > y1) {
            int y = y1;
            int error = 0;
            for (int x = x1; x <= x2; x += 1) {
                point_t* p = new point_t(x, y, color);
                p->draw(canvas);
                error += (y2 - y1);
                if (error*2 >= (x2 - x1)) {
                    y += 1;
                    error -= (x2 - x1);
                }
            }
        } else {
            int y = y1;
            int error = 0;
            for (int x = x1; x <= x2; x += 1) {
                point_t* p = new point_t(x, y, color);
                p->draw(canvas);
                error += (y1 - y2);
                if (error*2 >= (x2 - x1)) {
                    y -= 1;
                    error -= (x2 - x1);
                }
            }
        }
    }
}

void swap(int* x1, int* x2) {
    int temp1 = *x1;
    *x1 = *x2;
    *x2 = temp1;
}
