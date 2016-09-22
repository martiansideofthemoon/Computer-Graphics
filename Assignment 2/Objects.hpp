#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <math.h>
using namespace std;
#define PI 3.14159265

#ifndef _OBJECTS_HPP_
#define _OBJECTS_HPP_

void vertexcopy(float *src,float *dest);

// A simple class that represents a node in the hierarchy tree
class BaseObject {
public:
  std::vector<BaseObject*> children;
  BaseObject* parent;
  int rx, ry, rz;

  BaseObject();
  void add_child(BaseObject*);
  virtual void render();
  void render_tree();
  virtual void rotate(int rotate_x, int rotate_y, int rotate_z);
};

class Wheel: public BaseObject {
private:
  float* center;
  float* normal;
  int angle;
  int spokes;
  float radius;
  float tire_width;
  float* tire_color;
  float* spoke_color;
public:
  Wheel(float wheel_position[][4], float wheel_colors[][4], int spokes, float radius, float tire_width);
  void render();
  void rotate(int rotate_x, int rotate_y, int rotate_z);
};

class Pedal: public BaseObject {
private:
  float* center;
};

#endif
