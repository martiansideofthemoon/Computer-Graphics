#include "Assembly.hpp"

// This class is used for all the keyframe operations
class Keyframe {
  float handle_angle;
  float cycle_phase;
  bool headlight;
  bool roomlight;
  int camera_mode;
  float* center;
  float* direction;
  Keyframe(float handle_angle, float cycle_phase, bool headlight,
           bool roomlight, int camera_mode, float* center,
           float* direction);
  string to_string();
  void from_string(string input);
};

// This class is used to animate the system
class Animate {
  vector<Keyframe> keyframes;
  Animate();
  void play_back(Cycle* cycle, Room* room);
  void clear();
  void add_frame(Cycle* cycle, Room* room);
};