#include "Assembly.hpp"

void rotate_vector(float* vec, float angle) {
  float a1 = cos(angle*PI/180)*vec[0] + sin(angle*PI/180)*vec[1];
  vec[1] = -1*sin(angle*PI/180)*vec[0] + cos(angle*PI/180)*vec[1];
  vec[0] = a1;
}

Cycle::Cycle(string file_name) {
  camera_mode = 2;
  phase = 0;
  // Make the cycle by reading from a file
  string line;
  ifstream myfile(file_name.c_str());
  istringstream* iss;

  // position of cycle and normal
  getline(myfile, line);
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);

  // Common cycle parameters
  float position[2][4];
  float rel_position[2][4] = {
    {0,0,0,1}, // centre position
    {0,0,1,1}, // normal direction, Z direction
  };

  // fetching centre of cycle
  *iss >> position[0][0] >> position[0][1] >> position[0][2] >> position[0][3];
  delete iss;
  // fetching normal of cycle
  getline(myfile, line);
  iss = new istringstream(line);
  *iss >> position[1][0] >> position[1][1] >> position[1][2] >> position[1][3];
  delete iss;

  // fetching cycle width
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float cycle_width;
  *iss >> cycle_width;
  delete iss;

  // fetching tube thickness
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float tube_thickness;
  *iss >> tube_thickness;
  delete iss;

  // fetching frame color
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float frame_color[4];
  *iss >> frame_color[0] >> frame_color[1] >> frame_color[2] >> frame_color[3];
  delete iss;

  // fetching cycle height
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float height;
  *iss >> height;
  delete iss;

  // fetching frame length
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float frame_length[2];
  *iss >> frame_length[0] >> frame_length[1];
  delete iss;

  // Frame parameters
  frame = new Frame(position, frame_color, height, frame_length, tube_thickness, cycle_width);


  // Pedal parameters
  getline(myfile, line);
  getline(myfile, line);
  // fetching pedal colors
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float pedal_colors[2][4];
  *iss >> pedal_colors[0][0] >> pedal_colors[0][1] >> pedal_colors[0][2] >> pedal_colors[0][3];
  delete iss;
  getline(myfile, line);
  iss = new istringstream(line);
  *iss >> pedal_colors[1][0] >> pedal_colors[1][1] >> pedal_colors[1][2] >> pedal_colors[1][3];
  delete iss;

  // fetching pedal length
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float pedal_length;
  *iss >> pedal_length;
  delete iss;

  // fetching pedal shaft width
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float pedal_width;
  *iss >> pedal_width;
  delete iss;

  // Constructing the pedal
  pedal = new Pedal(rel_position, pedal_colors, pedal_width, pedal_length, 2*cycle_width);
  frame->add_child(pedal);
  //   float tex_pos10[4] = {0, 1, 0, 1};
  // frame->map_texture("silver.bmp", tex_pos10);

  // Gear Parameters
  getline(myfile, line);
  getline(myfile, line);
  // Spokes in the gear
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float gear_spokes;
  *iss >> gear_spokes;
  delete iss;

  // Radius of the gear
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float gear_radius;
  *iss >> gear_radius;
  delete iss;

  float back_gear_position[2][4] = {
    {frame_length[1],0,0,1}, // centre position
    {0,0,1,1}, // normal direction, Z direction
  };

  // Thickness of the gear
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float gear_thickness;
  *iss >> gear_thickness;
  delete iss;

  // fetching gear colors
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float gear_colors[2][4];
  *iss >> gear_colors[0][0] >> gear_colors[0][1] >> gear_colors[0][2] >> gear_colors[0][3];
  delete iss;
  getline(myfile, line);
  iss = new istringstream(line);
  *iss >> gear_colors[1][0] >> gear_colors[1][1] >> gear_colors[1][2] >> gear_colors[1][3];
  delete iss;

  front_gear = new Wheel(rel_position, gear_colors, gear_spokes, gear_radius, gear_thickness);
  back_gear = new Wheel(back_gear_position, gear_colors, gear_spokes, gear_radius, gear_thickness);
  pedal->add_child(front_gear);
  frame->add_child(back_gear);

  // Chain Parameters
  getline(myfile, line);
  getline(myfile, line);
  // Spokes in the gear
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float chain_color[4];
  *iss >> chain_color[0] >> chain_color[1] >> chain_color[2] >> chain_color[3];
  delete iss;

  // Segment radius in chain
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float segment_radius;
  *iss >> segment_radius;
  delete iss;

  chain = new Chain(rel_position, chain_color, gear_radius, segment_radius, frame_length[1]);
  frame->add_child(chain);

  // Wheel Parameters
  getline(myfile, line);
  getline(myfile, line);
  // wheel colors
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float wheel_colors[2][4];
  *iss >> wheel_colors[0][0] >> wheel_colors[0][1] >> wheel_colors[0][2] >> wheel_colors[0][3];
  delete iss;
  getline(myfile, line);
  iss = new istringstream(line);
  *iss >> wheel_colors[1][0] >> wheel_colors[1][1] >> wheel_colors[1][2] >> wheel_colors[1][3];
  delete iss;

  // Spokes in the gear
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float wheel_spokes;
  *iss >> wheel_spokes;
  delete iss;

  float front_wheel_position[2][4] = {
    {0,-1*height,0,1}, // centre position
    {0,0,1,1}, // normal direction, Z direction
  };

  // Thickness of the tire
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float tire_thickness;
  *iss >> tire_thickness;
  delete iss;

  // Radius of the wheel
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float wheel_radius;
  *iss >> wheel_radius;
  delete iss;

  // Assembling the wheel
  front_wheel = new Wheel(front_wheel_position, wheel_colors, wheel_spokes, wheel_radius, tire_thickness);
  back_wheel = new Wheel(back_gear_position, wheel_colors, wheel_spokes, wheel_radius, tire_thickness);
  frame->add_child(back_wheel);

  // Handle Parameters
  getline(myfile, line);
  getline(myfile, line);

  float handle_center[4] = {-1*frame_length[0], height, 0.0 , 1.0}; // centre position

  // handle colors
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float handle_colors[3][4];
  *iss >> handle_colors[0][0] >> handle_colors[0][1] >> handle_colors[0][2] >> handle_colors[0][3];
  delete iss;
  getline(myfile, line);
  iss = new istringstream(line);
  *iss >> handle_colors[1][0] >> handle_colors[1][1] >> handle_colors[1][2] >> handle_colors[1][3];
  delete iss;
  getline(myfile, line);
  iss = new istringstream(line);
  *iss >> handle_colors[2][0] >> handle_colors[2][1] >> handle_colors[2][2] >> handle_colors[2][3];
  delete iss;

  // handle dimensions
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float handle_dimension[] = {
    cycle_width, // l_tyre_thick depends on thickness of tyre
    height, // l_frame_height depends on height of frame
    wheel_radius + tire_thickness, // l_tyre_radius depends on radius of tyre
    1.5*tube_thickness,
    0, // length of handle
    0, // length of handle grip
    0, // handle offset
    0, // handle angle
  };
  *iss >> handle_dimension[4] >> handle_dimension[5] >> handle_dimension[6] >> handle_dimension[7];
  delete iss;

  handle = new Handle(handle_center, handle_colors, handle_dimension);
  frame->add_child(handle);
  handle->add_child(front_wheel);

  // Brake Parameters
  getline(myfile, line);
  getline(myfile, line);

  float brake_center[4] = {-handle_dimension[6]*cos(PI*handle_dimension[7]/180) , handle_dimension[6]*sin(PI*handle_dimension[7]/180), 0, 1.0}; // centre position
   //float brake_center[4] = {0, 0.0, 0, 1.0}; // centre position

  // brake colors
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float brake_color[4];
  *iss >> brake_color[0] >> brake_color[1] >> brake_color[2] >> brake_color[3];
  delete iss;

  // brake dimensions
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float brake_dimension[4] = {0, 0, 0, 0}; //length of brake, angle of brake, brake thickness, handle length
  *iss >> brake_dimension[0] >> brake_dimension[1] >> brake_dimension[2] >> brake_dimension[3];
  delete iss;

  brake = new Brake(brake_center, brake_color, brake_dimension);
  handle->add_child(brake);

  // Seat Parameters
  getline(myfile, line);
  getline(myfile, line);
  // seat dimensions
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float seat_dimension[4] = {0, 0, 0, tube_thickness}; //height,length,width,radius
  *iss >> seat_dimension[0] >> seat_dimension[1] >> seat_dimension[2];
  delete iss;

  float seat_center[4] = {0.0, tube_thickness+ height + seat_dimension[0] + seat_dimension[2], 0, 1.0}; // seat centre position

  // seat color
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float seat_color[4];
  *iss >> seat_color[0] >> seat_color[1] >> seat_color[2] >> seat_color[3];
  delete iss;

  seat = new Seat(seat_center,  seat_color, seat_dimension);
  frame->add_child(seat);

  // Rider Parameters
  getline(myfile, line);
  getline(myfile, line);
  // rider position
  float rider_position[2][4] = {
    {0, height + seat_dimension[0] + 2*seat_dimension[2], 0, 1},
    {0, 0, 1, 1}
  };

  // rider color
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float rider_color[4];
  *iss >> rider_color[0] >> rider_color[1] >> rider_color[2] >> seat_color[3];
  delete iss;

  // width of the rider
  float rider_width = 2*seat_dimension[1];

  // thigh of the rider
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float rider_thigh;
  *iss >> rider_thigh;
  delete iss;

  // leg of the rider
  getline(myfile, line);
  getline(myfile, line);
  iss = new istringstream(line);
  float rider_leg;
  *iss >> rider_leg;
  delete iss;

  rider = new Rider(rider_position, rider_color, rider_width, rider_thigh, rider_leg);
  rider->bend_leg(0, pedal_length);
  frame->add_child(rider);

  generate_headlight();
}

void Cycle::generate_headlight() {
    // Headlight code -->
  GLfloat spotlight_cutoff[] = { 30.0 };
  GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
  GLfloat light_diffuse[] = { 1, 1, 0.2, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, spotlight_cutoff);
}

void Cycle::adjust_headlight() {
  float* cycle_direction = frame->get_direction();
  GLfloat light_position[] = {frame->center[0]+cycle_direction[0]*(frame->front_len), frame->center[1]+frame->height, frame->center[2]+cycle_direction[1]*frame->front_len, 1.0 };
  rotate_vector(cycle_direction, handle->get_angle());
  GLfloat spotlight_direction[] = {cycle_direction[0], -0, cycle_direction[1], 1.0 };
  glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlight_direction);
  delete[] cycle_direction;
}

void Cycle::render() {
  // Execute the rendering sequence for this assembly
  frame->render_tree();
}

void Cycle::rotate(int rx, int ry, int rz) {
  // Rotate the cycle
  frame->rotate(rx, ry, rz);
}

void Cycle::pedal_cycle(float angle) {
  phase += angle;
  if (phase >= 360) phase -=360;
  if (phase < 0) phase += 360;
  pedal->rotate(0,0,angle);
  chain->rotate(0,0,angle);
  back_gear->rotate(0,0,angle);
  front_wheel->rotate(0,0, angle);
  back_wheel->rotate(0,0,angle);
  rider->bend_leg(phase, pedal->length);

  float distance = front_wheel->radius*angle*PI/180;
  float length = frame->front_len + frame->back_len;
  float handle_angle = handle->get_angle();
  float* cycle_direction = frame->get_direction();
  float angle_rotated = handle_angle - (180.0/PI)*asin(sin(handle_angle*PI/180.0)*(length - distance) / length);
  frame->translate(cycle_direction[0]*distance, 0, cycle_direction[1]*distance);
  delete[] cycle_direction;
  frame->rotate(0, angle_rotated, 0);
  //turn(-1*angle_rotated);
  use_camera(camera_mode);
}

void Cycle::move_to(float x, float y, float z) {
  // Move cycle to specific point
  frame->move_to(x, y, z);
}

void Cycle::set_direction(float x, float y, float z) {
  frame->set_direction(x, y, z);
}

void Cycle::turn(float angle) {
  // Turn the cycle with respect to up vector
  if (handle->get_angle() + angle > 80 || handle->get_angle() + angle < -80) {
    // Don't turn the handle
  } else {
    handle->rotate(0, angle, 0);
  }
}

void Cycle::use_camera(int mode) {
  camera_mode = mode;
  float* cycle_direction = frame->get_direction();
  if (mode == 0) {
    // first person
    glLoadIdentity();
    gluLookAt(frame->center[0], frame->center[1] + frame->height*2.5, frame->center[2],
    frame->center[0] + cycle_direction[0]*frame->front_len, frame->center[1] + frame->height, frame->center[2] + cycle_direction[1]*frame->front_len,      // center is at (0,0,0)
    0.0, 1.0, 0.0);      // up is in positive Y direction
  } else if (mode == 1) {
    // first person
    glLoadIdentity();
    gluLookAt(frame->center[0] - 3*cycle_direction[0]*frame->back_len, frame->center[1] + frame->height*4, frame->center[2] - 3*cycle_direction[1]*frame->back_len,
    frame->center[0] + cycle_direction[0]*frame->front_len, frame->center[1] + frame->height, frame->center[2] + cycle_direction[1]*frame->front_len,      // center is at (0,0,0)
    0.0, 1.0, 0.0);      // up is in positive Y direction
  } else {
    // do nothing
  }
  delete[] cycle_direction;
}

float Cycle::get_handle_angle() {
  return handle->get_angle();
}

void Cycle::set_handle_angle(float angle) {
  handle->set_angle(angle);
}

void Cycle::set_phase(float angle) {
  pedal->angle = angle;
  chain->angle = angle;
  back_gear->angle = angle;
  front_wheel->angle = angle;
  back_wheel->angle = angle;
  rider->bend_leg(phase, pedal->length);
  phase = angle;
}

float* Cycle::get_direction() {
  return frame->get_direction();
}

float* Cycle::get_center() {
  return frame->center;
}



Room::Room(string file_name) {
  room = new BaseObject();
  int detail = 50;
  room_height = 4;
  room_width = 8;
  room_depth = 8;
  float position = -0.57;

  float surface_color[4] = {1, 1, 1, 1};
  float floor_color[4] = {139.0/256, 69.0/256, 19.0/256, 1};

  float floor_pos[2][4] = {
    {0, position, 0, 1},
    {0, 1, 0, 1}
  };
  float bottom_width = room_width;
  float bottom_height = room_depth;
  int bottom_detail = 200;
  bottom = new Surface(floor_pos, surface_color, bottom_width, bottom_height, detail);
  room->add_child(bottom);
  float tex_pos0[4] = {0, 1, 0, 1};
  bottom->map_texture("wood.bmp", tex_pos0);

  float ceil_pos[2][4] = {
    {0, room_height + position, 0, 1},
    {0, -1, 0, 1}
  };
  float ceil_width = room_width;
  float ceil_height = room_depth;
  ceiling = new Surface(ceil_pos, surface_color, ceil_width, ceil_height, detail);
  room->add_child(ceiling);

  float left_pos[2][4] = {
    {-1*room_width / 2, room_height/2 + position, 0, 1},
    {1, 0, 0, 1}
  };
  float left_width = room_depth;
  float left_height = room_height;
  left_wall = new Surface(left_pos, surface_color, left_width, left_height, detail);
  room->add_child(left_wall);

  float right_pos[2][4] = {
    {room_width / 2, room_height/2 + position, 0, 1},
    {-1, 0, 0, 1}
  };
  float right_width = room_depth;
  float right_height = room_height;
  right_wall = new Surface(right_pos, surface_color, right_width, right_height, detail);
  room->add_child(right_wall);
  float tex_pos1[4] = {0.4, 0.6, 0.3, 0.7};
  //right_wall->map_texture("wall.bmp", tex_pos1);

  float back_pos[2][4] = {
    {0, room_height/2 + position, -1*room_depth/2, 1},
    {0, 0, 1, 1}
  };
  float back_width = room_width;
  float back_height = room_height;
  back_wall = new Surface(back_pos, surface_color, back_width, back_height, detail);
  room->add_child(back_wall);
  float tex_pos2[4] = {0.3, 0.7, 0.1, 0.9};
  back_wall->map_texture("mona.bmp", tex_pos2);

  float front_pos[2][4] = {
    {0, room_height/2 + position, room_depth/2, 1},
    {0, 0, -1, 1}
  };
  float front_width = room_width;
  float front_height = room_height;
  front_wall = new Surface(front_pos, surface_color, front_width, front_height, detail);
  room->add_child(front_wall);

  generate_light();

  float lamp_radius = 0.3;
  float lamp_position[4] = {0.0, 3.43-lamp_radius , 0.0, 1.0};
  float lamp_color[4] = {0.4, 0.4 , 0.0, 1.0};
  lamp = new Lamp(lamp_position, lamp_color, lamp_radius);
  room->add_child(lamp);


}

void Room::render() {
  room->render_tree();
}

void Room::use_camera() {
  glLoadIdentity();
  gluLookAt(0.0, 0.7*room_height, 1*room_depth/2,  // eye is at (0,0,5)
  0.0, 0.0, 0.0,      // center is at (0,0,0)
  0.0, 1.0, 0.0);      // up is in positive Y direction
}

void Room::generate_light() {
  GLfloat spotlight_cutoff[] = { 180.0 };
  GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, spotlight_cutoff);

  glEnable(GL_LIGHT0);
}

void Room::adjust_light() {
  GLfloat light_position[] = { 0.0, 3.0 , 0.7, 1.0 };
  GLfloat spotlight_direction[] = { 0.0, -1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotlight_direction);
}