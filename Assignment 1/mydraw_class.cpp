#include "mydraw_class.hpp"
#include <GL/glut.h>
#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
using namespace std;
//-------------------
bool compare(color_t* color1, color_t* color2)
{   bool check = true;
    if (color1->R() != color2->R())
    {
        check = false;
    }
    if (color1->G() != color2->G())
    {
        check = false;
    }
    if (color1->B() != color2->B())
    {
        check = false;
    }
    return check;
}

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
    int x, y;
    float r, g, b;
    iss >> x >> y >> r >> g >> b;
    color = new color_t(r, g, b);
}

string point_t::to_string() {
    stringstream ss;
    ss << x << " " << y << " ";
    ss << color->R() << " " << color->G() << " " << color->B();
    return ss.str();
}

void point_t::set_color(color_t* color) {
    this->color = color;
}

// canvas methods
canvas_t::canvas_t(int width, int height, color_t* background) {
    this->width = width;
    this->height = height;
    this->background = background;
    drawing = new drawing_t();
    this->pen = new pen_t(this);
    color_t* c1 = new color_t(1, 0, 0);
    this->fill = new fill_t(c1);
    image.resize(height);
    for (int i = 0; i < height; i++) {
        image[i].resize(width);
    }
    clear(true);
    append('B', background_to_string());
}

void canvas_t::clear(bool put_config) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            point_t* p = new point_t(j, i, background);
            image[i][j] = p;
        }
    }
    drawing->clear();
    if (put_config) {
        this->pen = new pen_t(this, this->pen->get_size(), this->pen->get_color(), this->pen->get_eraser());
    }
}

void canvas_t::put_point(point_t* point) {
    color_t* color;
    if (this->pen->get_eraser()) {
        color = this->background;
    } else {
        color = this->pen->get_color();
    }
    int size = this->pen->get_size();
    int centre_x = point->get_x();
    int centre_y = point->get_y();
    int lower_y = centre_y - size + 1 > 0 ? centre_y - size + 1 : 0;
    int lower_x = centre_x - size + 1 > 0 ? centre_x - size + 1 : 0;
    int upper_y = centre_y + size - 1 < this->height-1 ? centre_y + size - 1 : this->height-1;
    int upper_x = centre_x + size - 1 < this->width-1 ? centre_x + size - 1 : this->width-1;
    for (int i = lower_y; i <= upper_y; i++) {
        for (int j = lower_x; j <= upper_x; j++) {
            point_t* point1 = new point_t(j, i, color);
            image[i][j] = point1;
        }
    }
}


void canvas_t::draw_grid() {
    GLubyte checkImage[height][width][3];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            color_t* color = image[i][j]->get_color();
            checkImage[i][j][0] = (GLubyte) (int) 255*color->R();
            checkImage[i][j][1] = (GLubyte) (int) 255*color->G();
            checkImage[i][j][2] = (GLubyte) (int) 255*color->B();
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
    clear(false);
    drawing->load(file_name);
}

void canvas_t::draw_list() {
    drawing->draw(this);
}

point_t* canvas_t::get_point(int x, int y) {
    point_t* p = image[y][x];
    return p;
}

void canvas_t::set_point_color(int x, int y, color_t* color) {
    point_t* p1 = image[y][x];
    p1->set_color(color);
}

int canvas_t::get_height() {
    return height;
}

int canvas_t::get_width() {
    return width;
}

void canvas_t::set_background(color_t* color, bool save) {
    color_t* old = this->background;
    this->background = color;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            color_t* pixel = image[i][j]->get_color();
            if (compare(pixel, old)) {
                color_t* new_color = new color_t(color->R(), color->G(), color->B());
                set_point_color(j, i, new_color);
            }
        }
    }
    if (save) {
        append('B', background_to_string());
    }
}

color_t* canvas_t::get_background() {
    return background;
}

void canvas_t::background_from_string(string input) {
    istringstream iss(input);
    float r, g, b;
    iss >> r >> g >> b;
    color_t* color = new color_t(r, g, b);
    set_background(color, false);
}

string canvas_t::background_to_string() {
    stringstream ss;
    color_t* color = this->background;
    ss << color->R() << " " << color->G() << " " << color->B();
    return ss.str();
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
    int x1, y1, x2, y2;
    float r, g, b;
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
            case 'C': {
                canvas->pen->from_string(*s2);
            } break;
            case 'F': {
                canvas->fill->from_string(*s2, canvas);
            } break;
            case 'B': {
                canvas->background_from_string(*s2);
            }
        }
        s1++; s2++;
    }
}

void drawing_t::clear() {
    operations.clear();
    inputs.clear();
}

//pen_t methods
pen_t::pen_t(canvas_t* canvas) {
    size = 1;
    color = new color_t(1, 1, 1);
    eraser = false;
    this->canvas = canvas;
    canvas->append('C', this->to_string());
}

pen_t::pen_t(canvas_t* canvas, int size, color_t* color, bool eraser) {
    this->size = size;
    this->eraser = eraser;
    this->color = color;
    this->canvas = canvas;
    canvas->append('C', this->to_string());
}

int pen_t::get_size() {
    return size;
}

bool pen_t::get_eraser() {
    return eraser;
}

color_t* pen_t::get_color() {
    return color;
}

void pen_t::set_size(int size) {
    this->size = size;
    canvas->append('C', this->to_string());
}

void pen_t::set_color(color_t* color) {
    this->color = color;
    canvas->append('C', this->to_string());
}

void pen_t::toggle_eraser() {
    eraser = !eraser;
    canvas->append('C', this->to_string());
}

void pen_t::from_string(string input) {
    istringstream iss(input);
    float r, g, b;
    float back_r, back_g, back_b;
    int size, eraser;
    iss >> r >> g >> b >> size >> eraser;
    color = new color_t(r, g, b);
    this->color = color;
    this->size = size;
    if (eraser == 1) {
        this->eraser = true;
    } else {
        this->eraser = false;
    }
}

string pen_t::to_string() {
    stringstream ss;
    color_t* color = this->get_color();
    ss << color->R() << " " << color->G() << " " << color->B();
    ss << " " << size << " ";
    if (eraser) {
        ss << 1;
    } else {
        ss << 0;
    }
    return ss.str();
}

//fill_t methods
fill_t::fill_t(color_t* color) {
    this->fill_color = color;
}

void fill_t::set_fill_color(color_t* color) {
    this->fill_color = color;
}

color_t* fill_t::get_fill_color() {
    return fill_color;
}

void fill_t::draw(canvas_t* canvas, int x, int y, bool from_string)
{
    point_t* target_point = canvas->get_point(x, y);
    if (target_point->get_color() == fill_color) {
        return;
    }
    queue<point_t*> myqueue;
    myqueue.push(target_point);
    color_t* target_color = target_point->get_color();
    while (!myqueue.empty())
    {
        point_t* n = myqueue.front();
        myqueue.pop();
        int west_x = n->get_x(); int west_y = n->get_y();
        int east_x = n->get_x(); int east_y = n->get_y();
        while(west_x > -1 && compare(canvas->get_point(west_x, west_y)->get_color(), target_color)) {
            west_x--;
        }
        west_x++;
        while(east_x < canvas->get_width() && compare(canvas->get_point(east_x, east_y)->get_color(), target_color)) {
            east_x++;
        }
        east_x--;
        for (int i = west_x; i <= east_x; i++) {
            canvas->set_point_color(i, west_y, fill_color);
            if (west_y + 1 < canvas->get_height() && compare(canvas->get_point(i, west_y+1)->get_color(), target_color)) {
                myqueue.push(canvas->get_point(i, west_y+1));
            }
            if (west_y - 1 > -1 && compare(canvas->get_point(i, west_y-1)->get_color(), target_color)) {
                myqueue.push(canvas->get_point(i, west_y-1));
            }
        }
    }
    if (!from_string)
        canvas->append('F', this->to_string(x, y));
}


void fill_t::from_string(string input, canvas_t* canvas) {
    istringstream iss(input);
    float r, g, b;
    int x, y;
    iss >> r >> g >> b >> x >> y;
    color_t* color = new color_t(r, g, b);
    this->fill_color = color;
    this->draw(canvas, x, y, true);
}

string fill_t::to_string(int x, int y) {
    stringstream ss;
    color_t* color = this->fill_color;
    ss << color->R() << " " << color->G() << " " << color->B() << " ";
    ss << x << " " << y << " ";
    return ss.str();
}
