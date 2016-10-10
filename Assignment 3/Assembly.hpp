#include "Objects.hpp"
#include <fstream>
#include <sstream>
#include <string>

// This class is the assembly of a cycle object
class Cycle {
private:
  int phase;
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
  int camera_mode;
public:
  Cycle(string file_name);
  void render();
  void pedal_cycle(float angle);
  void rotate(int rx, int ry, int rz);
  void move_to(float x, float y, float z);
  void turn(float angle);
  void generate_headlight();
  void adjust_headlight();
  void use_camera(int mode);
};

// This class is the assembly of the room object
class Room {
private:
  float room_height;
  float room_width;
  float room_depth;
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
  void use_camera();
  void generate_light();
  void adjust_light();
};