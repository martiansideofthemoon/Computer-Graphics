#include "Assembly.hpp"

Cycle::Cycle(string file_name) {
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
  this->pedal_shaft = pedal_length;
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
}

void Cycle::render() {
  // Execute the rendering sequence for this assembly
  frame->render_tree();
}

void Cycle::rotate(int rx, int ry, int rz) {
  // Rotate the cycle
  frame->rotate(rx, ry, rz);
}

void Cycle::pedal_cycle(int angle) {
  phase += angle;
  if (phase >= 360) phase -=360;
  if (phase < 0) phase += 360;
  pedal->rotate(0,0,angle);
  chain->rotate(0,0,angle);
  back_gear->rotate(0,0,angle);
  front_wheel->rotate(0,0,angle);
  back_wheel->rotate(0,0,angle);
  rider->bend_leg(phase, this->pedal_shaft);
}

void Cycle::move_to(float x, float y, float z) {
  // Move cycle to specific point
}

void Cycle::turn(int angle) {
  // Turn the cycle with respect to up vector
  handle->rotate(0,angle,0);
}