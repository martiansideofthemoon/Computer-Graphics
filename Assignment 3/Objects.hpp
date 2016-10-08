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
  float rx, ry, rz;

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
  float* normal;
  int angle;
  float width;
  float length;
  float pedal_separation;
  float pedal_height;
  float* pedal_color;
  float* shaft_color;
public:
  Pedal(float pedal_position[][4],
        float pedal_colors[][4],
        float width,
        float length,
        float pedal_separation);
  void render();
  void rotate(int rotate_x, int rotate_y, int rotate_z);
};

class Chain: public BaseObject {
private:
  float* center;
  float* normal;
  int angle;
  float radius;
  float length;
  float* chain_color;
  float segment_radius;
  float thickness;
public:
  Chain(float chain_position[][4], float chain_color[4], float radius, float segment_radius, float length);
  void render();
  void rotate(int rotate_x, int rotate_y, int rotate_z);
};

class Frame: public BaseObject {
private:
  float* center;
  float* normal;
  float height;
  float front_len;
  float back_len;
  float* frame_color;
  float thickness;
  float cycle_width;
public:
  Frame(float frame_position[][4],
        float frame_color[4],
        float height,
        float length[2],
        float thickness,
        float cycle_width);
  void render();
  void rotate(int rotate_x, int rotate_y, int rotate_z);
  void translate(float x, float y, float z);
};

class Handle: public BaseObject {
private:
  float* center;
  float* handle_body_color;
  float* handle_frame_color;
  float* handle_actual_color;
  float length_of_handle;
  float length_of_grip;
  float handle_angle;
  float handle_offset;
  float l_tyre_thick;
  float l_frame_height;
  float l_tyre_radius;
  float l_frame_thick;
public:
  Handle(float* Handle_center, float handle_colors[][4], float handle_dimension[8]);
  void render();
  void rotate(int rotate_x, int rotate_y, int rotate_z);
};

class Brake: public BaseObject {
private:
  float* brake_center;
  float* brake_color;
  float length_of_brake;
  float brake_angle;
  float brake_thick;
  float length_of_handle;
public:
  Brake(float* brake_center, float* brake_color, float brake_dimension[4]);
  void render();
  void rotate(int rotate_x, int rotate_y, int rotate_z);
};

class Seat: public BaseObject {
private:
  float* center;
  float* seat_color;
  float seat_height;
  float seat_width;
  float seat_length;
  float r_seat;
public:
  Seat(float* seat_center, float* seat_color, float seat_dimension[4]);
  void render();
  void rotate(int rotate_x, int rotate_y, int rotate_z);
};


class Rider: public BaseObject {
private:
  float* center;
  float* normal;
  float* rider_color;
  float width;
  float thigh;
  float leg;
  float thigh_angle;
  float leg_angle;
  float thigh_angle2;
  float leg_angle2;
public:
  Rider(float rider_position[][4], float rider_color[4], float width, float thigh, float leg);
  void render();
  void bend_leg(int pedal_angle, float pedal_shaft);
  void rotate(int rotate_x, int rotate_y, int rotate_z);
};

class Surface: public BaseObject {
private:
  float* center;
  float* normal;
  float* surface_color;
  float width;
  float height;
  int detail; // An integer value from 1
public:
  Surface(float surface_position[][4], float surface_color[4], float width, float height, int detail);
  void render();
  void rotate(int rotate_x, int rotate_y, int rotate_z);
};

#endif
