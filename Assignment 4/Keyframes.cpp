#include "Keyframes.hpp"

Keyframe::Keyframe() {

}

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

Animate::Animate(Cycle* cycle, Room* room) {
  keyframes.clear();
  string line;
  ifstream myfile("keyframes.txt");
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      Keyframe* k = new Keyframe();
      k->from_string(line);
      keyframes.push_back(*k);
    }
    myfile.close();
  }
  this->cycle = cycle;
  this->room = room;
}

void Animate::clear() {
  keyframes.clear();
  ofstream myfile("keyframes.txt");
  myfile.close();
}

void Animate::write_keyframes() {
  ofstream myfile("keyframes.txt");
  vector<Keyframe>::iterator s1 = keyframes.begin();
  while (s1 != keyframes.end()) {
    myfile << s1->to_string() << endl;
    s1++;
  }
  myfile.close();
}

void Animate::add_frame() {
  bool headlight = glIsEnabled(GL_LIGHT1);
  bool roomlight = glIsEnabled(GL_LIGHT0);
  int camera_mode = cycle->camera_mode;
  float handle_angle = cycle->get_handle_angle();
  float phase = cycle->phase;
  float* direction = cycle->get_direction();
  float* center = cycle->get_center();
  Keyframe *k = new Keyframe(handle_angle, phase, headlight, roomlight,
                             camera_mode, center, direction);
  keyframes.push_back(*k);
  write_keyframes();
}

void Animate::play_back() {

}