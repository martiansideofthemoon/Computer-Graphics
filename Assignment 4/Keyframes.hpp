#include "Assembly.hpp"

// This class is used for all the keyframe operations
class Keyframe {
private:
  float handle_angle;
  float cycle_phase;
  bool headlight;
  bool roomlight;
  int camera_mode;
  float* center;
  float* direction;
public:
  Keyframe();
  Keyframe(float handle_angle, float cycle_phase, bool headlight,
           bool roomlight, int camera_mode, float* center,
           float* direction);
  string to_string();
  void from_string(string input);
};

// This class is used to animate the system
class Animate {
private:
  vector<Keyframe> keyframes;
public:
  Animate();
  void play_back(Cycle* cycle, Room* room);
  void clear();
  void add_frame(Cycle* cycle, Room* room);
  void write_keyframes();
};