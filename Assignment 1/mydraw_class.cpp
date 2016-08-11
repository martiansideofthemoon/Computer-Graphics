#include "mydraw_class.hpp"
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
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

void point_t::draw(canvas_t* canvas, bool saved) {
    if (!saved) {
        canvas->append('P', this->to_string());
    }
    canvas->put_point(this);
}

void point_t::from_string(string input) {
    istringstream iss(input);
    int r, g, b;
    iss >> x >> y >> r >> g >> b;
    color = new color_t(r, g, b);
}

string point_t::to_string() {
    stringstream ss;
    ss << x << " " << y << " ";
    ss << color->R() << " " << color->G() << " " << color->B();
    return ss.str();
}

// canvas methods
canvas_t::canvas_t(int width, int height, color_t* background) {
    this->width = width;
    this->height = height;
    this->background = background;
    drawing = new drawing_t();
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
    drawing->clear();
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

void canvas_t::append(char op, string input) {
    drawing->append(op, input);
}

void canvas_t::save(string file_name) {
    drawing->save(file_name);
}

void canvas_t::load(string file_name) {
    drawing->load(file_name);
}

void canvas_t::draw_list() {
    drawing->draw(this);
}

//line_t methods
line_t::line_t() {
    color_t* color = new color_t(0, 0, 0);
    point_t* p1 = new point_t(0, 0, color);
    point_t* p2 = new point_t(0, 0, color);
    this->p1 = p1;
    this->p2 = p2;
    this->color = color;
}

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

void line_t::draw(canvas_t* canvas, bool saved) {
    if (!saved) {
        canvas->append('L', this->to_string());
    }
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
                p->draw(canvas, true);
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
                p->draw(canvas, true);
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
                p->draw(canvas, true);
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
                p->draw(canvas, true);
                error += (y1 - y2);
                if (error*2 >= (x2 - x1)) {
                    y -= 1;
                    error -= (x2 - x1);
                }
            }
        }
    }
}

void line_t::from_string(string input) {
    istringstream iss(input);
    int x1, y1, x2, y2, r, g, b;
    iss >> x1 >> y1 >> x2 >> y2 >> r >> g >> b;
    color = new color_t(r, g, b);
    p1 = new point_t(x1, y1, color);
    p2 = new point_t(x2, y2, color);
}

string line_t::to_string() {
    stringstream ss;
    ss << p1->get_x() << " " << p1->get_y() << " ";
    ss << p2->get_x() << " " << p2->get_y() << " ";
    ss << color->R() << " " << color->G() << " " << color->B();
    return ss.str();
}



void swap(int* x1, int* x2) {
    int temp1 = *x1;
    *x1 = *x2;
    *x2 = temp1;
}

//triangle_t methods
triangle_t::triangle_t() {
    color_t* color = new color_t(0, 0, 0);
    point_t* p1 = new point_t(0, 0, color);
    point_t* p2 = new point_t(0, 0, color);
    point_t* p3 = new point_t(0, 0, color);
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->color = color;
}

triangle_t::triangle_t(point_t* p1, point_t* p2, point_t* p3, color_t* color) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->color = color;
}

point_t* triangle_t::get_p1() {
    return p1;
}

point_t* triangle_t::get_p2() {
    return p2;
}

point_t* triangle_t::get_p3() {
    return p3;
}

color_t* triangle_t::get_color() {
    return color;
}

void triangle_t::draw(canvas_t* canvas, bool saved) {
    if (!saved) {
        canvas->append('T', this->to_string());
    }
    line_t* l1 = new line_t(p1, p2, color);
    l1->draw(canvas, true);
    line_t* l2 = new line_t(p2, p3, color);
    l2->draw(canvas, true);
    line_t* l3 = new line_t(p1, p3, color);
    l3->draw(canvas, true);
}

void triangle_t::from_string(string input) {
    istringstream iss(input);
    int x1, y1, x2, y2, x3, y3, r, g, b;
    iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> r >> g >> b;
    color = new color_t(r, g, b);
    p1 = new point_t(x1, y1, color);
    p2 = new point_t(x2, y2, color);
    p3 = new point_t(x3, y3, color);
}

string triangle_t::to_string() {
    stringstream ss;
    ss << p1->get_x() << " " << p1->get_y() << " ";
    ss << p2->get_x() << " " << p2->get_y() << " ";
    ss << p3->get_x() << " " << p3->get_y() << " ";
    ss << color->R() << " " << color->G() << " " << color->B();
    return ss.str();
}

//drawing_t methods
drawing_t::drawing_t() {}

void drawing_t::load(string file_name) {
    string line;
    ifstream myfile(file_name.c_str());
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            char op = line[0];
            string input = line.substr(2);
            append(op, input);
        }
        myfile.close();
    }
}

void drawing_t::save(string file_name) {
    ofstream myfile(file_name.c_str());
    list <char>::iterator s1 = operations.begin();
    list <string>::iterator s2 = inputs.begin();
    while(s1 != operations.end()) {
        myfile << *s1 << " " << *s2 << endl;
        s1++;
        s2++;
    }
    myfile.close();
}

void drawing_t::append(char op, string input) {
    operations.push_back(op);
    inputs.push_back(input);
}

void drawing_t::draw(canvas_t* canvas) {
    list <char>::iterator s1 = operations.begin();
    list <string>::iterator s2 = inputs.begin();
    while(s1 != operations.end()) {
        switch(*s1) {
            case 'P': {
                point_t* p1 = new point_t();
                p1->from_string(*s2);
                p1->draw(canvas, true);
            } break;
            case 'L': {
                line_t* l1 = new line_t();
                l1->from_string(*s2);
                l1->draw(canvas, true);
            } break;
            case 'T': {
                triangle_t* t1 = new triangle_t();
                t1->from_string(*s2);
                t1->draw(canvas, true);
            } break;
        }
        s1++; s2++;
    }
}

void drawing_t::clear() {
    operations.clear();
    inputs.clear();
}

//pen_t methods
pen_t::pen_t() {
    size = 1;
    pen_color = new color_t(0,0,0);
    mode = true;
}

pen_t::pen_t(int _size, color_t* pc, bool _mode) {
    size = _size;
    mode = _mode;
    pen_color = pc;
}

int pen_t::get_pen_size() {
    return size;
}

bool pen_t::get_pen_mode() {
    return mode;
}

color_t* pen_t::get_pen_color() {
    return pen_color;
}

void pen_t::set_pen_size(int size) {
    this->size = size;
}

void pen_t::set_pen_color(color_t* color) {
    this->pen_color = color;
}

void pen_t::set_pen_mode() {
    if (mode == false)
    {
        mode = true;
    }
    else
    {
        mode = false;
    }
}

void pen_t::draw(canvas_t* canvas, point_t* p) {
    canvas->put_point(p);
}


//fill_t methods

void fill_t::set_fill_color(color_t* color) {
    this->fill_color = color;
}

color_t* fill_t::get_fill_color() {
    return fill_color;
}

void fill_t::draw(canvas_t* canvas, point_t* point)
{
}