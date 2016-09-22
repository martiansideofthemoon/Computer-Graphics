#include "Objects.hpp"

void vertexcopy(float *src,float *dest){
  dest[0]=src[0];
  dest[1]=src[1];
  dest[2]=src[2];
  dest[3]=src[3];
}

BaseObject::BaseObject() {
  // Nothing to do in the base class constructor
}

void BaseObject::add_child(BaseObject *child){
  children.push_back(child);
}

void BaseObject::render() {
  // Each object has its own rendering style
}

void BaseObject::render_tree() {
  // Same for all objects
  glPushMatrix();
  render();
  for(int i=0;i<children.size();i++){
    children[i]->render_tree();
  }
  glPopMatrix();
}

void BaseObject::rotate(int rotate_x, int rotate_y, int rotate_z) {
  // Each object has its own rotation scheme. This is the general rotation scheme
  rx += rotate_x;
  while (rx > 360) rx -= 360;
  ry += rotate_y;
  while (ry > 360) ry -= 360;
  rz += rotate_z;
  while (rz > 360) rz -= 360;
  while (rx < 0) rx += 360;
  while (ry < 0) ry += 360;
  while (rz < 0) rz += 360;
}

// Functions of the Wheel Class
Wheel::Wheel(float* center, float* normal, int angle, int spokes, int radius) {
  this->center = new float[4];
  vertexcopy(this->center, center);
  this->normal = new float[4];
  vertexcopy(this->normal, normal);
  this->angle = angle;
  this->spokes = spokes;
  this->radius = radius;
}

void Wheel::render() {

}

void Wheel::rotate(int rotate_x, int rotate_y, int rotate_z) {
  
}
