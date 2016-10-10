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

void BaseObject::rotate(float rotate_x, float rotate_y, float rotate_z) {
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
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tire_color);
  //glColor3fv(tire_color);
  glutSolidTorus(tire_width, radius+tire_width, 50, 50);

  // rendering the spokes
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, spoke_color);
  //glColor3fv(spoke_color);
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

void Wheel::rotate(float rotate_x, float rotate_y, float rotate_z) {
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

  // rendering the shaft
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, shaft_color);
  //glColor3fv(shaft_color);
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

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pedal_color);
  //glColor3fv(pedal_color);
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

void Pedal::rotate(float rotate_x, float rotate_y, float rotate_z) {
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
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, chain_color);
  //glColor3fv(chain_color);
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

void Chain::rotate(float rotate_x, float rotate_y, float rotate_z) {
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
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, frame_color);
  //glColor3fv(frame_color);
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

void Frame::translate(float x, float y, float z) {
  center[0] += x;
  center[1] += y;
  center[2] += z;
}

void Frame::rotate(float rotate_x, float rotate_y, float rotate_z) {
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

float* Frame::get_direction() {
  float* direction = new float[2];
  direction[0] = -1*cos(ry*PI/180);
  direction[1] = sin(ry*PI/180);
  return direction;
}

// Functions of the Handle Class
Handle::Handle(float* Handle_center , float handle_colors[][4], float handle_dimension[8]) {
  this->center = new float[4];
  this->ry = 0;
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
    float radius_of_hand_grip = 1.001*l_frame_thick/2;
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

    glRotatef(ry, 0.0, 1.0, 0.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, handle_body_color);
    //glColor3fv(handle_body_color);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //main handle
    glPushMatrix();
    glTranslatef(handle_x,handle_y,-length_of_handle/2);
    gluCylinder(quadratic, radius_of_handle, radius_of_handle, length_of_handle, 32, 32 );
    glPopMatrix();

    //hand grip
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, handle_actual_color);
    //glColor3fv(handle_actual_color);
    glPushMatrix();
    glTranslatef(handle_x,handle_y, -length_of_handle/2);
    gluCylinder(quadratic, radius_of_hand_grip, radius_of_hand_grip, length_of_grip, 32, 32 );
    glPopMatrix();
    glPushMatrix();
    glTranslatef(handle_x,handle_y, length_of_handle/2-length_of_grip);
    gluCylinder(quadratic, radius_of_hand_grip , radius_of_hand_grip , length_of_grip , 32 , 32 );
    glPopMatrix();


    //45 degree pipe
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, handle_body_color);
    //glColor3fv(handle_body_color);
    glPushMatrix();
    glRotatef(-1*handle_angle, 0.0, 0.0, 1.0);
    glRotatef(-90, 0.0, 1.0, 0.0);
    gluCylinder(quadratic, l_frame_thick/2, l_frame_thick/2, handle_offset, 32, 32 );
    glPopMatrix();

    gluQuadricOrientation(quadratic, GLU_INSIDE);
    glPushMatrix();
    glRotatef(-90, 0.0, 1.0, 0.0);
    gluCylinder(quadratic, l_frame_thick/2, l_frame_thick*1.2, handle_offset/2, 32, 32 );
    glPopMatrix();

    gluQuadricOrientation(quadratic, GLU_OUTSIDE);
    //vertical pipe
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    gluCylinder(quadratic, l_frame_thick/2, l_frame_thick/2, l_frame_height - l_tyre_radius, 32, 32 );
    glPopMatrix();

    //red vertical pipe
    float red_prop = 0.8;
    float red_radius_prop = 1.2;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, handle_frame_color);
    //glColor3fv(handle_frame_color);
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
}

void Handle::rotate(float rotate_x, float rotate_y, float rotate_z) {
  ry += rotate_y;
  while (ry > 360) ry -= 360;
}

int Handle::get_angle() {
  return ry;
}


// Functions of the brake Class
Brake::Brake(float* brake_center , float* brake_color, float brake_dimension[4]) {
  this->brake_center = new float[4];
  vertexcopy(brake_center,this->brake_center);
  this->brake_color = new float[4];
  vertexcopy(brake_color, this->brake_color);
  this->length_of_brake = brake_dimension[0]; //depends on length_of_brake
  this->brake_angle = brake_dimension[1];          //depends on brake_angle
  this->brake_thick = brake_dimension[2];  //depends on brake_thick
  this->length_of_handle = brake_dimension[3];  //depends on length_of_handle

}

void Brake::render() {
    //brake lever
    glTranslatef(brake_center[0],brake_center[1],brake_center[2]);
    //glRotatef(ry, 0.0, 1.0, 0.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, brake_color);
    //glColor3fv(brake_color);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    glPushMatrix();
    glTranslatef( -length_of_brake*sin(PI*brake_angle/180), 0, -length_of_handle/4 );
    glRotatef(  brake_angle, 0.0, 1.0, 0.0);
    gluCylinder(quadratic, brake_thick , brake_thick , length_of_brake , 32 , 32 );
    glRotatef( 180 - brake_angle, 0.0, 1.0, 0.0);
    glTranslatef(0,0,-0.01);
    gluCylinder(quadratic, brake_thick , brake_thick , 2*length_of_brake , 32 , 32 );
    glPopMatrix();

    glPushMatrix();
    glTranslatef( -length_of_brake*sin(PI*brake_angle/180), 0, length_of_handle/4 );
    glRotatef(180 - brake_angle, 0.0, 1.0, 0.0);
    gluCylinder(quadratic, brake_thick , brake_thick , length_of_brake , 32 , 32 );
    glRotatef(180 + brake_angle, 0.0, 1.0, 0.0);
    glTranslatef(0,0,-0.01);
    gluCylinder(quadratic, brake_thick , brake_thick , 2*length_of_brake , 32 , 32 );
    glPopMatrix();
}



void Brake::rotate(float rotate_x, float rotate_y, float rotate_z) {
  ry += 2*rotate_y;
  while (ry > 360) ry -= 360;
}

Seat::Seat(float* seat_center, float* seat_color, float seat_dimension[4]) {
  this->center = new float[4];
  vertexcopy(seat_center,this->center);
  this->seat_color = new float[4];
  vertexcopy(seat_color,this->seat_color);
  this->seat_height = seat_dimension[0];
  this->seat_length = seat_dimension[1];
  this->seat_width = seat_dimension[2];
  this->r_seat = seat_dimension[3];
}

void Seat::render() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, seat_color);
    //glColor3fv(seat_color);
    glTranslatef(center[0],center[1],center[2]);

    float seat_end = -seat_length;
    glPushMatrix();
    glTranslatef(-seat_length/2,0,0);
    glBegin(GL_POLYGON);
      glVertex3f(0.1*seat_length, seat_height, -0.5*seat_width);
      glVertex3f( -seat_length, seat_height, -0.3*seat_width);
      glVertex3f( -seat_length, seat_height,  0.3*seat_width);
      glVertex3f(0.1*seat_length, seat_height,  0.5*seat_width);
      glVertex3f(0.5*seat_length, seat_height,  seat_width);
      glVertex3f(1.0*seat_length, seat_height,  seat_width);
      glVertex3f(1.0*seat_length, seat_height, -seat_width);
      glVertex3f(0.5*seat_length, seat_height, -seat_width);
    glEnd();


    glBegin(GL_POLYGON);
      glVertex3f(0.1*seat_length, -seat_height, -0.5*seat_width);
      glVertex3f( -seat_length, -seat_height, -0.3*seat_width);
      glVertex3f( -seat_length, -seat_height,  0.3*seat_width);
      glVertex3f(0.1*seat_length, -seat_height,  0.5*seat_width);
      glVertex3f(0.5*seat_length, -seat_height,  seat_width);
      glVertex3f(seat_length, -seat_height,  seat_width);
      glVertex3f(seat_length, -seat_height, -seat_width);
      glVertex3f(0.5*seat_length, -seat_height, -seat_width);
    glEnd();


   glBegin(GL_QUADS);
      //1
      glVertex3f( -seat_length,  seat_height, -0.3*seat_width);
      glVertex3f( -seat_length,  seat_height,  0.3*seat_width);
      glVertex3f( -seat_length, -seat_height,  0.3*seat_width);
      glVertex3f( -seat_length, -seat_height, -0.3*seat_width);

      //2
      glVertex3f(  -seat_length,  seat_height, 0.3*seat_width);
      glVertex3f( 0.1*seat_length,  seat_height, 0.5*seat_width);
      glVertex3f( 0.1*seat_length, -seat_height, 0.5*seat_width);
      glVertex3f(  -seat_length, -seat_height, 0.3*seat_width);

      //3
      glVertex3f( 0.1*seat_length,  seat_height, 0.5*seat_width);
      glVertex3f( 0.5*seat_length,  seat_height, seat_width);
      glVertex3f( 0.5*seat_length, -seat_height, seat_width);
      glVertex3f( 0.1*seat_length, -seat_height, 0.5*seat_width);

      //4
      glVertex3f( 0.5*seat_length,  seat_height, seat_width);
      glVertex3f( -seat_end,  seat_height, seat_width);
      glVertex3f( -seat_end, -seat_height, seat_width);
      glVertex3f( 0.5*seat_length, -seat_height, seat_width);

      //5
      glVertex3f(  -seat_end,  seat_height,  seat_width);
      glVertex3f(  -seat_end,  seat_height, -seat_width);
      glVertex3f(  -seat_end, -seat_height, -seat_width);
      glVertex3f(  -seat_end, -seat_height,  seat_width);

      //6
      glVertex3f( 0.5*seat_length,  seat_height, -seat_width);
      glVertex3f( -seat_end,  seat_height, -seat_width);
      glVertex3f( -seat_end, -seat_height, -seat_width);
      glVertex3f( 0.5f*seat_length, -seat_height, -seat_width);

      //7
      glVertex3f( 0.1*seat_length,  seat_height, -0.5*seat_width);
      glVertex3f( 0.5*seat_length,  seat_height, -seat_width);
      glVertex3f( 0.5*seat_length, -seat_height, -seat_width);
      glVertex3f( 0.1*seat_length, -seat_height, -0.5*seat_width);

      //8
      glVertex3f( -seat_length,  seat_height, -0.3*seat_width);
      glVertex3f( 0.1*seat_length,  seat_height, -0.5*seat_width);
      glVertex3f( 0.1*seat_length, -seat_height, -0.5*seat_width);
      glVertex3f( -seat_length, -seat_height, -0.3*seat_width);


   glEnd();
   glPopMatrix();

    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

    //support frame
    glPushMatrix();
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(quadratic, r_seat , r_seat , 4*seat_height , 32 , 32 );
    glPopMatrix();
}

void Seat::rotate(float rotate_x, float rotate_y, float rotate_z) {
  // Nothing to be done here
}

Rider::Rider(float rider_position[][4], float rider_color[4], float width, float thigh, float leg) {
  this->center = new float[4];
  vertexcopy(rider_position[0], this->center);
  this->normal = new float[4];
  vertexcopy(rider_position[1], this->normal);
  // Normalizing the normal vector
  float magnitude = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
  normal[0] /= magnitude;
  normal[1] /= magnitude;
  normal[2] /= magnitude;

  this->thigh_angle = 0;
  this->leg_angle = 0;
  this->thigh_angle2 = 0;
  this->leg_angle2 = 0;
  this->width = width;
  this->thigh = thigh;
  this->leg = leg;
  this->rider_color = new float[4];
  vertexcopy(rider_color, this->rider_color);
}

void Rider::render() {
  glTranslatef(center[0], center[1], center[2]);
  float rotation_angle = acos(normal[2])*180.0/PI;
  glRotatef(rotation_angle, -1*normal[1], normal[0], 0);

  // rendering the tires
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, rider_color);
  //glColor3fv(rider_color);
  float person_thickness = 0.1;

  // rendering the mid_body
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  glPushMatrix();
  glTranslatef(0, 0, -1*width/2);
  gluCylinder(quadratic, person_thickness/2, person_thickness/2, width, 32, 32);
  glPopMatrix();

  // rendering thigh
  glPushMatrix();
  glTranslatef(0, 0, width/2);
  glRotatef(thigh_angle, 0, 0, 1);
  glRotatef(90, 1, 0, 0);
  gluCylinder(quadratic, person_thickness/2, person_thickness/2, thigh, 32, 32);
  glPopMatrix();

  // rendering leg
  glPushMatrix();
  glTranslatef(thigh*sin(thigh_angle*PI/180), -1*thigh*cos(thigh_angle*PI/180), width/2);
  glRotatef(leg_angle, 0, 0, 1);
  glRotatef(90, 1, 0, 0);
  gluCylinder(quadratic, person_thickness/2, person_thickness/2, leg, 32, 32);
  glPopMatrix();

  // rendering thigh
  glPushMatrix();
  glTranslatef(0, 0, -1*width/2);
  glRotatef(thigh_angle2, 0, 0, 1);
  glRotatef(90, 1, 0, 0);
  gluCylinder(quadratic, person_thickness/2, person_thickness/2, thigh, 32, 32);
  glPopMatrix();

  // rendering leg
  glPushMatrix();
  glTranslatef(thigh*sin(thigh_angle2*PI/180), -1*thigh*cos(thigh_angle2*PI/180), -1*width/2);
  glRotatef(leg_angle2, 0, 0, 1);
  glRotatef(90, 1, 0, 0);
  gluCylinder(quadratic, person_thickness/2, person_thickness/2, leg, 32, 32);
  glPopMatrix();
}

void Rider::rotate(float rotate_x, float rotate_y, float rotate_z) {

}

void Rider::bend_leg(int pedal_angle, float pedal_shaft) {
  float pedal_x = 0 - pedal_shaft*sin(pedal_angle*PI/180);
  float pedal_y = -1*center[1] + pedal_shaft*cos(pedal_angle*PI/180);
  float length = sqrt(pedal_x*pedal_x + pedal_y*pedal_y);
  float alpha = (180/PI)*acos((thigh*thigh + leg*leg - length*length) / (2*thigh*leg));
  float beta = (180/PI)*acos((thigh*thigh - leg*leg + length*length) / (2*thigh*length));
  float phi = 180*atan(pedal_x/pedal_y)/PI;
  thigh_angle = -1*(phi + beta);
  leg_angle = 180 - alpha - beta - phi;

  pedal_x = pedal_shaft*sin(pedal_angle*PI/180);
  pedal_y = -1*center[1] - pedal_shaft*cos(pedal_angle*PI/180);
  length = sqrt(pedal_x*pedal_x + pedal_y*pedal_y);
  alpha = (180/PI)*acos((thigh*thigh + leg*leg - length*length) / (2*thigh*leg));
  beta = (180/PI)*acos((thigh*thigh - leg*leg + length*length) / (2*thigh*length));
  phi = 180*atan(pedal_x/pedal_y)/PI;
  thigh_angle2 = -1*(phi + beta);
  leg_angle2 = 180 - alpha - beta - phi;
}

// Functions of the Surface Class
Surface::Surface(float surface_position[][4], float surface_color[4], float width, float height, int detail) {
  this->center = new float[4];
  vertexcopy(surface_position[0], this->center);
  this->normal = new float[4];
  vertexcopy(surface_position[1], this->normal);
  // Normalizing the normal vector
  float magnitude = sqrt(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
  normal[0] /= magnitude;
  normal[1] /= magnitude;
  normal[2] /= magnitude;

  this->width = width;
  this->height = height;
  this->detail = detail;
  this->surface_color = new float[4];
  vertexcopy(surface_color, this->surface_color);
}

void Surface::render() {
  glTranslatef(center[0], center[1], center[2]);
  float rotation_angle = acos(normal[2])*180.0/PI;
  if (normal[0] == 0 && normal[1] == 0 && normal[2] < 0) {
    glRotatef(180, 1, 0, 0);
  } else {
    glRotatef(rotation_angle, -1*normal[1], normal[0], 0);
  }

  // rendering the tires
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, surface_color);
  //glColor3fv(surface_color);
  float quad_width = width / detail;
  float quad_height = height / detail;
  glPushMatrix();
    glBegin(GL_QUADS);
    for (int j = 0; j < detail; j++) {
      for (int i = 0; i < detail; i++) {
        glNormal3f(0, 0, 1);
        glVertex3f(-1*width/2 + i*quad_width, -1*height/2 + j*quad_height, 0);
        glVertex3f(-1*width/2 + (i+1)*quad_width, -1*height/2 + j*quad_height, 0);
        glVertex3f(-1*width/2 + (i+1)*quad_width, -1*height/2 + (j+1)*quad_height, 0);
        glVertex3f(-1*width/2 + i*quad_width, -1*height/2 + (j+1)*quad_height, 0);
      }
    }
    glEnd();
  glPopMatrix();

}

void Surface::rotate(float rotate_x, float rotate_y, float rotate_z) {
  // Can't be rotated independently
}
