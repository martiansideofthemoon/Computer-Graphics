#include "Keyframes.hpp"

Keyframe::Keyframe(float handle_angle, float cycle_phase, bool headlight,
           		   bool roomlight, int camera_mode, float* center,
           		   float* direction) {
  this->handle_angle = handle_angle;
  this->cycle_phase = cycle_phase;
  this->headlight = headlight;
  this->roomlight = roomlight;
  this->camera_mode = camera_mode;
  this->center = new float[4];
  vertexcopy(center, this->center);
  this->direction = new float[4];
  vertexcopy(direction, this->direction);
}

string Keyframe::to_string() {
  stringstream ss;
  ss << handle_angle << " " << cycle_phase << " " << headlight << " " << roomlight;
  ss << " " << camera_mode << center[0] << " " << center[1] << " " << center[2] << " ";
  ss << center[3] << " " << direction[0] << " " << direction[1] << " " << direction[2] << " ";
  ss << direction[3];
  return ss.str();
}

void Keyframe::from_string(string input) {
  istringstream iss(input);
  iss >> handle_angle >> cycle_phase >> headlight >> roomlight >> camera_mode;
  this->center = new float[4];
  iss >> center[0] >> center[1] >> center[2] >> center[3];
  this->direction = new float[4];
  iss >> direction[0] >> direction[1] >> direction[2] >> direction[3];
}
