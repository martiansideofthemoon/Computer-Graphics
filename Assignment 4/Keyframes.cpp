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
  ss << " " << camera_mode << " " << center[0] << " " << center[1] << " " << center[2] << " ";
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

Animate::Animate(Cycle* cycle, Room* room, int fps) {
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
  current_keyframe = 0;
  current_frame = 0;
  this->fps = fps;
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
  float* direction_2D = cycle->get_direction();
  float* direction = new float[4];
  direction[0] = direction_2D[0];
  direction[1] = 0;
  direction[2] = direction_2D[1];
  direction[3] = 1;
  float* center = cycle->get_center();
  Keyframe *k = new Keyframe(handle_angle, phase, headlight, roomlight,
                             camera_mode, center, direction);
  keyframes.push_back(*k);
  write_keyframes();
}

Keyframe* Animate::interpolate()
{
  bool headlight;
  bool roomlight;
  int camera_mode;
  int next_keyframe = current_keyframe + 1;
  //section formula parameters
  float ratio1 = current_frame;
  float ratio2 = fps - current_frame;
  if (current_frame < fps/2)
  {
    headlight = keyframes[current_keyframe].headlight;
    roomlight = keyframes[current_keyframe].roomlight;
    camera_mode = keyframes[current_keyframe].camera_mode;
  }
  else
  {
    headlight = keyframes[next_keyframe].headlight;
    roomlight = keyframes[next_keyframe].roomlight;
    camera_mode = keyframes[next_keyframe].camera_mode;
  }
  //handle_angle
  float next_handle_angle = keyframes[next_keyframe].handle_angle;
  float current_handle_angle = keyframes[current_keyframe].handle_angle;
  float handle_angle = (ratio1 * next_handle_angle + ratio2 * current_handle_angle)/fps;
  //cycle_phase
  float next_cycle_phase = keyframes[next_keyframe].cycle_phase;
  float current_cycle_phase = keyframes[current_keyframe].cycle_phase;
  if (next_cycle_phase < current_cycle_phase )
  {
    next_cycle_phase = next_cycle_phase + 360;
  }  float phase = (ratio1 * next_cycle_phase + ratio2 * current_cycle_phase)/fps;
  //phase = int(phase) % 360;
  //center
  float* center_old = keyframes[current_keyframe].center;
  float* center_new = keyframes[next_keyframe].center;
  float center[4];
  center[0] = (ratio1 * center_new[0] + ratio2 * center_old[0])/fps;
  center[1] = (ratio1 * center_new[1] + ratio2 * center_old[1])/fps;
  center[2] = (ratio1 * center_new[2] + ratio2 * center_old[2])/fps;
  center[3] = (ratio1 * center_new[3] + ratio2 * center_old[3])/fps;
  //direction
  float* direction_old = keyframes[current_keyframe].direction;
  float* direction_new = keyframes[next_keyframe].direction;
  float direction[4];
  direction[0] = (ratio1 * direction_new[0] + ratio2 * direction_old[0])/fps;
  direction[1] = (ratio1 * direction_new[1] + ratio2 * direction_old[1])/fps;
  direction[2] = (ratio1 * direction_new[2] + ratio2 * direction_old[2])/fps;
  direction[3] = (ratio1 * direction_new[3] + ratio2 * direction_old[3])/fps;
  Keyframe *k = new Keyframe(handle_angle, phase, headlight, roomlight,
                             camera_mode, center, direction);
  return k;
}

bool Animate::play_next() {
  if (current_keyframe < keyframes.size()-1) {
    if (current_frame < fps)
    {
      build_scene(interpolate());
      glutPostRedisplay();
      current_frame++;
      return true;
    }
    else
    {
    current_keyframe++;
    current_frame = 0;
    return true;
    }
  } else {
    current_keyframe = 0;
    current_frame = 0;
    return false;
  }
}

void Animate::build_scene(Keyframe* frame) {
  cycle->move_to(frame->center[0], frame->center[1], frame->center[2]);
  cycle->set_direction(frame->direction[0], frame->direction[1], frame->direction[2]);
  cycle->set_phase(frame->cycle_phase);
  cycle->set_handle_angle(frame->handle_angle);
  if (frame->headlight) {
    glEnable(GL_LIGHT1);
  } else {
    glDisable(GL_LIGHT1);
  }
  if (frame->roomlight) {
    glEnable(GL_LIGHT0);
  } else {
    glDisable(GL_LIGHT0);
  }
  cycle->use_camera(frame->camera_mode);
  if (frame->camera_mode == 2) {
    room->use_camera();
  }
}