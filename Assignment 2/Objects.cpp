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
Wheel::Wheel(float wheel_position[][4], float wheel_colors[][4], int spokes, float radius, float tire_width) {
  this->center = new float[4];
  vertexcopy(wheel_position[0], this->center);
  this->normal = new float[4];
  vertexcopy(wheel_position[1], this->normal);
  // Normalizing the normal vector
  float magnitude = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
  normal[0] /= magnitude;
  normal[1] /= magnitude;
  normal[2] /= magnitude;

  this->angle = 0;
  this->spokes = spokes;
  this->radius = radius;
  this->tire_width = tire_width;
  this->tire_color = new float[4];
  this->spoke_color = new float[4];
  vertexcopy(wheel_colors[0], this->tire_color);
  vertexcopy(wheel_colors[1], this->spoke_color);
}

void Wheel::render() {
  glPushMatrix();
  glTranslatef(center[0], center[1], center[2]);
  float rotation_angle = acos(normal[2])*180.0/PI;
  glRotatef(rotation_angle, -1*normal[1], normal[0], 0);

  // rotating wheel by current angle
  glRotatef(angle, 0, 0, 1);

  // rendering the tires
  glColor3fv(tire_color);
  glutSolidTorus(tire_width, radius+tire_width, 50, 50);

  // rendering the spokes
  glColor3fv(spoke_color);
  for (int i = 0; i < spokes; i++) {
    glPushMatrix();
    glRotatef(i*360/spokes, 0, 0, 1);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, 0.01f, 0.01f, radius, 32, 32);
    glPopMatrix();
  }
  glPopMatrix();
}

void Wheel::rotate(int rotate_x, int rotate_y, int rotate_z) {
  angle += rotate_z;
  while(angle > 360) angle -= 360;
  while(angle < 0) angle += 360;
}
