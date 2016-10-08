#include "Objects.hpp"
#include <fstream>
#include <sstream>
#include <string>

// This class is the assembly of a cycle object
class Cycle {
private:
  int phase;
  float pedal_shaft;
  float wheel_radius;
  float handle_angle;
  float front_length;
  float back_length;
  float angle_rotated;
  float cycle_direction[2];
  Frame* frame;
  Wheel* front_wheel;
  Wheel* back_wheel;
  Pedal* pedal;
  Wheel* front_gear;
  Wheel* back_gear;
  Chain* chain;
  Handle* handle;
  Brake* brake;
  Seat* seat;
  Rider* rider;
public:
  Cycle(string file_name);
  void render();
  void pedal_cycle(int angle);
  void rotate(int rx, int ry, int rz);
  void move_to(float x, float y, float z);
  void turn(int angle);
};

// This class is the assembly of the room object
class Room {
private:
  BaseObject* room;
  Surface* bottom;
  Surface* left_wall;
  Surface* right_wall;
  Surface* back_wall;
  Surface* front_wall;
  Surface* ceiling;
public:
  Room(string file_name);
  void render();
};