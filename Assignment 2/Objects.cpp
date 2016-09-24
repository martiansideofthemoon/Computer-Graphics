#include "Objects.hpp"

void vertexcopy(float *src,float *dest){
  dest[0]=src[0];
  dest[1]=src[1];
  dest[2]=src[2];
  dest[3]=src[3];
}

void draw_cuboid(float x, float y, float z) {
  glBegin(GL_QUADS);
  //Front
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-1*x, -1*y, z);
  glVertex3f(x, -1*y, z);
  glVertex3f(x, y, z);
  glVertex3f(-1*x, y, z);

  //Back
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(-1*x, -1*y, -1*z);
  glVertex3f(x, -1*y, -1*z);
  glVertex3f(x, y, -1*z);
  glVertex3f(-1*x, y, -1*z);

  //Right
  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f(x, -1*y, -1*z);
  glVertex3f(x, y, -1*z);
  glVertex3f(x, y, z);
  glVertex3f(x, -1*y, z);

  //Left
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(-1*x, -1*y, -1*z);
  glVertex3f(-1*x, -1*y, z);
  glVertex3f(-1*x, y, z);
  glVertex3f(-1*x, y, -1*z);

  //Top
  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-1*x, y, -1*z);
  glVertex3f(-1*x, y, z);
  glVertex3f(x, y, z);
  glVertex3f(x, y, -1*z);

  //Bottom
  glNormal3f(0.0f, -1.0f, 0.0f);
  glVertex3f(-1*x, -1*y, -1*z);
  glVertex3f(-1*x, -1*y, z);
  glVertex3f(x, -1*y, z);
  glVertex3f(x, -1*y, -1*z);
  glEnd();
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
}

void Wheel::rotate(int rotate_x, int rotate_y, int rotate_z) {
  angle += rotate_z;
  while(angle > 360) angle -= 360;
  while(angle < 0) angle += 360;
}

// Functions of the Pedal Class
Pedal::Pedal(float pedal_position[][4],
             float pedal_colors[][4],
             float width,
             float length,
             float pedal_separation) {
  this->center = new float[4];
  vertexcopy(pedal_position[0], this->center);
  this->normal = new float[4];
  vertexcopy(pedal_position[1], this->normal);
  // Normalizing the normal vector
  float magnitude = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
  normal[0] /= magnitude;
  normal[1] /= magnitude;
  normal[2] /= magnitude;

  this->angle = 0;
  this->pedal_separation = pedal_separation;
  this->pedal_height = 0.05;
  this->width = width;
  this->length = length;
  this->pedal_color = new float[4];
  this->shaft_color = new float[4];
  vertexcopy(pedal_colors[0], this->pedal_color);
  vertexcopy(pedal_colors[1], this->shaft_color);
}

void Pedal::render() {
  glTranslatef(center[0], center[1], center[2]);
  float rotation_angle = acos(normal[2])*180.0/PI;
  glRotatef(rotation_angle, -1*normal[1], normal[0], 0);

  // rotating wheel by current angle
  glRotatef(angle, 0, 0, 1);

  // rendering the tires
  glColor3fv(shaft_color);
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  glPushMatrix();
  glTranslatef(0, 0, -1*pedal_separation/2);
  gluCylinder(quadratic, 0.01f, 0.01f, pedal_separation, 32, 32);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0, pedal_separation/2);
  glRotatef(-90, 1.0f, 0.0f, 0.0f);
  gluCylinder(quadratic, 0.01f, 0.01f, length, 32, 32);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0, -1*pedal_separation/2);
  glRotatef(90, 1.0f, 0.0f, 0.0f);
  gluCylinder(quadratic, 0.01f, 0.01f, length, 32, 32);
  glPopMatrix();

  glColor3fv(pedal_color);
  glPushMatrix();
  glTranslatef(0, length, pedal_separation);
  glRotatef(-1*angle, 0, 0, 1);
  draw_cuboid(0.6*width/2, pedal_height/2, width/2);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, -1*length, -1*pedal_separation);
  glRotatef(-1*angle, 0, 0, 1);
  draw_cuboid(0.6*width/2, pedal_height/2, width/2);
  glPopMatrix();

}

void Pedal::rotate(int rotate_x, int rotate_y, int rotate_z) {
  angle += rotate_z;
  while(angle > 360) angle -= 360;
  while(angle < 0) angle += 360;
}

// Functions of the Chain Class
Chain::Chain(float chain_position[][4], float chain_color[4], float radius, float segment_radius, float length) {
  this->center = new float[4];
  vertexcopy(chain_position[0], this->center);
  this->normal = new float[4];
  vertexcopy(chain_position[1], this->normal);
  // Normalizing the normal vector
  float magnitude = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
  normal[0] /= magnitude;
  normal[1] /= magnitude;
  normal[2] /= magnitude;

  this->angle = 0;
  this->radius = radius;
  this->length = length;
  this->segment_radius = segment_radius;
  this->thickness = 0.05;
  this->chain_color = new float[4];
  vertexcopy(chain_color, this->chain_color);
}

void Chain::render() {
  glTranslatef(center[0], center[1], center[2]);
  float rotation_angle = acos(normal[2])*180.0/PI;
  glRotatef(rotation_angle, -1*normal[1], normal[0], 0);

  // rendering the chain
  glColor3fv(chain_color);
  float distance_moved = angle*PI*radius/180;

  int circle_segments = (PI*radius)/(2*segment_radius);
  for (int i = 0; i <= circle_segments; i++) {
    glPushMatrix();
      float straight_distance = distance_moved - PI*radius + (i*PI/circle_segments)*radius;
      float circular_angle = straight_distance > 0 ? PI - i*PI/circle_segments : distance_moved/radius;
      float straight_offset = straight_distance > 0 ? straight_distance : 0;
      float y_coord = radius*cos(circular_angle + i*PI/circle_segments);
      float x_coord = -1*radius*sin(circular_angle + i*PI/circle_segments) + straight_offset;
      glTranslatef(x_coord, y_coord, 0);
      GLUquadricObj *quadratic;
      quadratic = gluNewQuadric();
      gluCylinder(quadratic, segment_radius, segment_radius, thickness, 32, 32);
      glPushMatrix();
        glTranslatef(0,0,thickness);
        gluDisk(quadratic, segment_radius/3, segment_radius, 32, 32);
      glPopMatrix();
    glPopMatrix();

    glPushMatrix();
      glTranslatef(length-x_coord, -1*y_coord, 0);
      //GLUquadricObj *quadratic;
      quadratic = gluNewQuadric();
      gluCylinder(quadratic, segment_radius, segment_radius, thickness, 32, 32);
      glPushMatrix();
        glTranslatef(0,0,thickness);
        gluDisk(quadratic, segment_radius/3, segment_radius, 32, 32);
      glPopMatrix();
    glPopMatrix();
  }

  for (int i = 0; i < length/(2*segment_radius); i++) {
    // Drawing upper part of chain
    glPushMatrix();
      float circular_distance = distance_moved - 2*i*segment_radius;
      float straight_offset = circular_distance > 0 ? -2*i*segment_radius : -1*distance_moved;
      float circle_x_offset = circular_distance > 0 ? -radius*sin(circular_distance/radius): 0;
      float circle_y_offset = circular_distance > 0 ? -1*radius+radius*cos(circular_distance/radius): 0;
      float y_coord = radius+circle_y_offset;
      float x_coord = 2*i*segment_radius + straight_offset + circle_x_offset;
      glTranslatef(x_coord, y_coord, 0);
      GLUquadricObj *quadratic;
      quadratic = gluNewQuadric();
      gluCylinder(quadratic, segment_radius, segment_radius, thickness, 32, 32);
      glPushMatrix();
        glTranslatef(0,0,thickness);
        gluDisk(quadratic, segment_radius/3, segment_radius, 32, 32);
      glPopMatrix();
    glPopMatrix();
    // Drawing lower part of chain
    glPushMatrix();
      // second point is mirror image of first about (length/2, 0, 0)
      glTranslatef(length - x_coord, -1*y_coord, 0);
      //GLUquadricObj *quadratic;
      quadratic = gluNewQuadric();
      gluCylinder(quadratic, segment_radius, segment_radius, thickness, 32, 32);
      glPushMatrix();
        glTranslatef(0,0,thickness);
        gluDisk(quadratic, segment_radius/3, segment_radius, 32, 32);
      glPopMatrix();
    glPopMatrix();
  }
}

void Chain::rotate(int rotate_x, int rotate_y, int rotate_z) {
  angle += rotate_z;
  while(angle > 180) angle -= 180;
  while(angle < 0) angle += 180;
}

// Functions for the frame
Frame::Frame(float frame_position[][4],
             float frame_color[4],
             float height,
             float length[2],
             float thickness,
             float cycle_width) {
  this->center = new float[4];
  vertexcopy(frame_position[0], this->center);
  this->normal = new float[4];
  vertexcopy(frame_position[1], this->normal);
  // Normalizing the normal vector
  float magnitude = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
  normal[0] /= magnitude;
  normal[1] /= magnitude;
  normal[2] /= magnitude;

  this->rx = 0;
  this->ry = 0;
  this->rz = 0;
  this->height = height;
  this->cycle_width = cycle_width;
  this->front_len = length[0];
  this->back_len = length[1];
  this->thickness = thickness;
  this->frame_color = new float[4];
  vertexcopy(frame_color, this->frame_color);
}

void Frame::render() {
  glTranslatef(center[0], center[1], center[2]);
  float rotation_angle = acos(normal[2])*180.0/PI;
  glRotatef(rotation_angle, -1*normal[1], normal[0], 0);

  glRotatef(rx, 1.0, 0.0, 0.0);
  glRotatef(ry, 0.0, 1.0, 0.0);
  glRotatef(rz, 0.0, 0.0, 1.0);

  // rendering the frame
  glColor3fv(frame_color);
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  glPushMatrix();
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic, thickness/2, thickness/2, height, 32, 32);
  glPopMatrix();
  glPushMatrix();
    glRotatef(-1*atan(height/front_len)*180/PI,0,0,1);
    glRotatef(-90, 0.0f, 1.0f, 0.0f);
    float diag_len = sqrt(height*height + front_len*front_len);
    gluCylinder(quadratic, thickness/2, thickness/2, diag_len, 32, 32);
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0, height, 0);
    glRotatef(-90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadratic, thickness/2, thickness/2, front_len, 32, 32);
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0, 0, cycle_width/2);
    glTranslatef(0, height, 0);
    glRotatef(-1*atan(height/back_len)*180/PI,0,0,1);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    diag_len = sqrt(height*height + back_len*back_len);
    gluCylinder(quadratic, thickness/2, thickness/2, diag_len, 32, 32);
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0, 0, -1*cycle_width/2);
    glTranslatef(0, height, 0);
    glRotatef(-1*atan(height/back_len)*180/PI,0,0,1);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    diag_len = sqrt(height*height + back_len*back_len);
    gluCylinder(quadratic, thickness/2, thickness/2, diag_len, 32, 32);
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0, 0, cycle_width/2);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadratic, thickness/2, thickness/2, back_len, 32, 32);
  glPopMatrix();
  glPushMatrix();
    glTranslatef(0, 0, -1*cycle_width/2);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadratic, thickness/2, thickness/2, back_len, 32, 32);
  glPopMatrix();
}

void Frame::translate(int x, int y, int z) {
  center[0] += x;
  center[1] += y;
  center[2] += z;
}

void Frame::rotate(int rotate_x, int rotate_y, int rotate_z) {
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

// Functions of the Handle Class
Handle::Handle(float* Handle_center , float handle_colors[][4], float handle_dimension[4]) {
  this->center = new float[4];
  vertexcopy(Handle_center,this->center);
  this->l_tyre_thick = handle_dimension[0]; //depends on thickness of tyre
  this->l_frame_height = handle_dimension[1];          //depends on height of frame
  this->l_tyre_radius = handle_dimension[2];  //depends on radius of tyre
  this->l_frame_thick = handle_dimension[3];  //depends on thickness of frame
  this->length_of_handle = handle_dimension[4];
  this->length_of_grip = handle_dimension[5];
  this->handle_offset = handle_dimension[6];
  this->handle_angle = handle_dimension[7];
  this->handle_body_color = new float[4];
  this->handle_actual_color = new float[4];
  this->handle_frame_color = new float[4];
  vertexcopy(handle_colors[0], this->handle_body_color);
  vertexcopy(handle_colors[1], this->handle_actual_color);
  vertexcopy(handle_colors[2], this->handle_frame_color);
}

void Handle::render() {
    float length_of_brake = 0.6f;
    float length_of_brake_support = 0.5f;
    float radius_of_handle = l_frame_thick/2;
    float radius_of_hand_grip = 1.5*l_frame_thick/2;
    float radius_of_brake = 0.05f;
    float radius_of_brake_support = 0.07f;

    float length_of_brake_tyre = 0.75f;

    float radius_of_wheel_support = l_frame_thick/2;
    float radius_of_brake_tyre = 0.05f;
    float radius_of_brake_tyre_support = 0.05f;

    float handle_x = -1*handle_offset*cos(handle_angle*PI/180);
    float handle_y = handle_offset*sin(handle_angle*PI/180);
    float brake_support_x = 0.3;

    glTranslatef(center[0],center[1],center[2]);
    glRotatef(rx, 1.0, 0.0, 0.0);
    glRotatef(ry, 0.0, 1.0, 0.0);
    glRotatef(rz, 0.0, 0.0, 1.0);
    glColor3fv(handle_body_color);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //main handle
    glPushMatrix();
    glTranslatef(handle_x,handle_y,-length_of_handle/2);
    gluCylinder(quadratic, radius_of_handle, radius_of_handle, length_of_handle, 32, 32 );
    glPopMatrix();

    //hand grip
    glColor3fv(handle_actual_color);
    glPushMatrix();
    glTranslatef(handle_x,handle_y, -length_of_handle/2);
    gluCylinder(quadratic, radius_of_hand_grip, radius_of_hand_grip, length_of_grip, 32, 32 );
    glPopMatrix();
    glPushMatrix();
    glTranslatef(handle_x,handle_y, length_of_handle/2-length_of_grip);
    gluCylinder(quadratic, radius_of_hand_grip , radius_of_hand_grip , length_of_grip , 32 , 32 );
    glPopMatrix();

    //brake lever
    glPushMatrix();
    glTranslatef(handle_x,handle_y,1.2);
    glRotatef(-30, 0.0, 1.0, 0.0);
    glTranslatef(-0.1,0.1,0.0);
    gluCylinder(quadratic, radius_of_brake_support , radius_of_brake_support , length_of_brake_support , 32 , 32 );
    glRotatef(30, 0.0, 1.0, 0.0);
    glTranslatef(-0.25,0.015,0.4);
    gluCylinder(quadratic, radius_of_brake , radius_of_brake , length_of_brake , 32 , 32 );
    glPopMatrix();

    glPushMatrix();
    glTranslatef(handle_x-0.2,handle_y,-1.1);
    glRotatef(210, 0.0, 1.0, 0.0);
    glTranslatef(-0.1,0.1,0.0);
    gluCylinder(quadratic, radius_of_brake_support , radius_of_brake_support , length_of_brake_support , 32 , 32 );
    glRotatef(-210, 0.0, 1.0, 0.0);
    glTranslatef(-0.25,0.015,-1.0);
    gluCylinder(quadratic, radius_of_brake , radius_of_brake , length_of_brake , 32 , 32 );
    glPopMatrix();

    //45 degree pipe
    glColor3fv(handle_body_color);
    glPushMatrix();
    glRotatef(-1*handle_angle, 0.0, 0.0, 1.0);
    glRotatef(-90, 0.0, 1.0, 0.0);
    gluCylinder(quadratic, l_frame_thick/2, l_frame_thick/2, handle_offset, 32, 32 );
    glPopMatrix();

    //vertical pipe
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quadratic, l_frame_thick/2, l_frame_thick/2, l_frame_height - l_tyre_radius, 32, 32 );
    glPopMatrix();

    //red vertical pipe
    float red_prop = 0.8;
    float red_radius_prop = 1.2;
    glColor3fv(handle_frame_color);
    glPushMatrix();
    glTranslatef(0,-1*((1-red_prop)*(l_frame_height-l_tyre_radius)),0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quadratic, red_radius_prop*l_frame_thick/2, red_radius_prop*l_frame_thick/2, red_prop*(l_frame_height-l_tyre_radius), 32, 32 );
    glPopMatrix();

    //red horizontal pipe
    glPushMatrix();
    glTranslatef(0, -1*(l_frame_height - l_tyre_radius - l_frame_thick/2),-l_tyre_thick/2);
    gluCylinder(quadratic, l_frame_thick/2, l_frame_thick/2, l_tyre_thick, 32, 32 );
    glPopMatrix();

    //red vertical wheel attachment
    glPushMatrix();
    glTranslatef(0,-l_frame_height+l_tyre_radius,-l_tyre_thick/2);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quadratic, l_frame_thick/2, l_frame_thick/2, l_tyre_radius, 32, 32);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-l_frame_height+l_tyre_radius,l_tyre_thick/2);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quadratic, l_frame_thick/2, l_frame_thick/2, l_tyre_radius, 32, 32);
    glPopMatrix();

    // glPushMatrix();
    // glTranslatef(0, -l_frame_height,-l_tyre_thick/2);
    // gluCylinder(quadratic, radius_of_wheel_support, radius_of_wheel_support, l_tyre_thick, 32, 32);
    // glPopMatrix();

    //brakes
    glColor3fv(handle_actual_color);

    glPushMatrix();
    glTranslatef(0, l_frame_height,-l_tyre_thick/2);
    gluCylinder(quadratic, radius_of_brake_tyre_support , radius_of_brake_tyre_support , length_of_brake_tyre_support , 32 , 32 );
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-3*l_frame_height,-l_tyre_thick/2);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quadratic, radius_of_brake_tyre , radius_of_brake_tyre , length_of_brake_tyre , 32 , 32 );
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-3*l_frame_height,l_tyre_thick/2);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quadratic, radius_of_brake_tyre , radius_of_brake_tyre , length_of_brake_tyre , 32 , 32 );
    glPopMatrix();

}

void Handle::rotate(int rotate_x, int rotate_y, int rotate_z) {
  ry += 2*rotate_y;
  while (ry > 360) ry -= 360;
}