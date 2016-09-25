#include "Objects.hpp"
#include <fstream>
#include <sstream>
#include <string>

// This class is the assembly of a cycle object
class Cycle {
private:
  Frame* frame;
  Wheel* front_wheel;
  Wheel* back_wheel;
  Pedal* pedal;
  Wheel* front_gear;
  Wheel* back_gear;
  Chain* chain;
  Handle* handle;
  Seat* seat;
public:
  Cycle(string file_name);
  void render();
  void pedal_cycle(int angle);
  void rotate(int rx, int ry, int rz);
  void move_to(float x, float y, float z);
  void turn(int angle);
};