#ifndef _MYDRAW_CLASS_HPP_
#define _MYDRAW_CLASS_HPP_
#include <vector>
#include <list>
#include <string>
using namespace std;
//Define all classes like the color class, adding appropriate methods and data members. 
//Implementation of the methods go into the corresponding cpp file

//------------------------
//color_t class
class canvas_t;
class drawing_t;

class color_t {
private:
  float r,g,b;
public:
  color_t();
  color_t(const float _r, const float _g, const float _b);

  void set(const float _r, const float _g, const float _b);
  float R(void);
  float G(void);
  float B(void);
};

//------------------------

//point_t class
class point_t {
private:
	int x,y;
	color_t* color;
public:
	point_t();
	point_t(int _x, int _y, color_t* c);
	int get_x(void);
	int get_y(void);
	color_t* get_color(void);
	void draw(canvas_t* canvas, bool saved);
	void from_string(string input);
	string to_string();
};

//------------------------

//canvas_t class
class canvas_t {
private:
	int width;
	int height;
	vector< vector<point_t*> > image;
	color_t* background;
	drawing_t* drawing;
public:
	canvas_t(int width, int height, color_t* background);
	void clear(void);
	void put_point(point_t* point);
	void draw_grid(void);
	void append(char op, string input);
	void save(string file_name);
	void load(string file_name);
	void draw_list(void);
};

//line_t class
class line_t {
private:
	point_t* p1;
	point_t* p2;
	color_t* color;
public:
	line_t();
	line_t(point_t* p1, point_t* p2, color_t* color);
	point_t* get_p1();
	point_t* get_p2();
	color_t* get_color();
	void draw(canvas_t* canvas, bool saved);
	void from_string(string input);
	string to_string();

};

//triangle_t class
class triangle_t {
private:
	point_t* p1;
	point_t* p2;
	point_t* p3;
	color_t* color;
public:
	triangle_t();
	triangle_t(point_t* p1, point_t* p2, point_t* p3, color_t* color);
	point_t* get_p1();
	point_t* get_p2();
	point_t* get_p3();
	color_t* get_color();
	void draw(canvas_t* canvas, bool saved);
	void from_string(string input);
	string to_string();
};



//drawing_t class
class drawing_t {
private:
	list<char> operations;
	list<string> inputs;
public:
	drawing_t();
	void load(string file_name);
	void save(string file_name);
	void append(char op, string input);
	void draw(canvas_t* canvas);
};

//pen_t class
class pen_t {
private:
	int size;
	color_t* pen_color;
	bool mode;
public:
	pen_t();
	pen_t(int _size, color_t* pc, bool _mode);
	int get_pen_size(void);
	color_t* get_pen_color(void);
	bool get_pen_mode(void);
	void set_pen_size(int size);
	void set_pen_color(color_t* color);
	void set_pen_mode(void);
	void draw(canvas_t* canvas, point_t* point);
};

//fill_t class
class fill_t {
private:
	point_t* input;
	color_t* fill_color;
public:
	void set_fill_color(color_t* color);
	color_t* get_fill_color(void);
	void draw(canvas_t* canvas, point_t* point);
};

#endif
