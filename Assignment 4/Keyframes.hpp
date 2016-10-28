#include "Assembly.hpp"

// This class is used for all the keyframe operations
class Keyframe {
public:
  float handle_angle;
  float cycle_phase;
  bool headlight;
  bool roomlight;
  int camera_mode;
  float* center;
  float* direction;
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
  Cycle* cycle;
  Room* room;
  int current_keyframe;
  int fps;
  int current_frame;
public:
  vector<Keyframe> keyframes;
  Animate(Cycle* cycle, Room* room, int fps);
  bool play_next();
  void clear();
  void add_frame();
  void write_keyframes();
  void build_scene(Keyframe* frame);
  Keyframe* interpolate();
};