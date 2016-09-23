#include "Assembly.hpp"

Cycle::Cycle(string file_name) {
  // Make the cycle by reading from a file

  // Common cycle parameters
  float position[2][4] = {
    {0,0,0,1}, // centre position
    {0,0,1,1}, // normal direction, Z direction
  };
  float cycle_width = 0.1;
  float tube_thickness = 0.05;

  // Frame parameters
  float frame_color[4] = {1.0, 0.0, 0.0, 1.0};
  float height = 1;
  float frame_length[2] = {1.5, 1};
  frame = new Frame(position, frame_color, height, frame_length, tube_thickness, cycle_width);

  // Pedal parameters
  float pedal_colors[2][4] = {
    {0, 0, 0, 1.0}, // pedal color
    {0.4, 0.4, 0.4, 1} // shaft color
  };
  float pedal_length = 0.5;
  float pedal_width = 0.3;
  pedal = new Pedal(position, pedal_colors, pedal_width, pedal_length, 2*cycle_width);

  frame->add_child(pedal);

  // Gear Parameters
  float gear_spokes = 5;
  float gear_radius = 0.2;
  float back_gear_position[2][4] = {
    {frame_length[1],0,0,1}, // centre position
    {0,0,1,1}, // normal direction, Z direction
  };
  float gear_thickness = 0.007;
  float gear_colors[2][4] = {
    {0.4, 0.4, 0.4, 1.0}, // rim color
    {0.4, 0.4, 0.4, 1} // spoke color
  };
  front_gear = new Wheel(position, gear_colors, gear_spokes, gear_radius, gear_thickness);
  back_gear = new Wheel(back_gear_position, gear_colors, gear_spokes, gear_radius, gear_thickness);

  pedal->add_child(front_gear);
  frame->add_child(back_gear);

  // Chain Parameters
  float chain_color[4] = {0, 0, 0, 1};
  float segment_radius = 0.01;
  chain = new Chain(position, chain_color, gear_radius, segment_radius, frame_length[1]);

  frame->add_child(chain);

  // Wheel Parameters
  float wheel_colors[2][4] = {
    {0, 0, 0, 1.0}, // tire color
    {1,1,1, 1} // spoke color
  };
  float wheel_spokes = 30;
  float front_wheel_position[2][4] = {
    {0,-1*height,0,1}, // centre position
    {0,0,1,1}, // normal direction, Z direction
  };
  float tire_thickness = 0.04;
  float wheel_radius = 0.5;
  front_wheel = new Wheel(front_wheel_position, wheel_colors, wheel_spokes, wheel_radius, tire_thickness);
  back_wheel = new Wheel(back_gear_position, wheel_colors, wheel_spokes, wheel_radius, tire_thickness);
  frame->add_child(back_wheel);

  // Handle parameters
  float handle_center[4] = {-1*frame_length[0], height, 0.0 , 1.0}; // centre position
  float handle_dimension[3] = {
    cycle_width, // l_tyre_thick depends on thickness of tyre
    height-wheel_radius, // l_frame_height depends on height of frame
    wheel_radius, // l_tyre_radius depends on radius of tyre
  };
  float handle_colors[3][4] = {
    {0.75, 0.75, 0.75, 1.0}, // handle_body_color
    {0, 0, 0, 1.0}, // handle_actual_color
    {1, 0, 0, 1} // handle_frame_color
  };
  handle = new Handle(handle_center, handle_colors, handle_dimension);
  frame->add_child(handle);
  handle->add_child(front_wheel);
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
  pedal->rotate(0,0,angle);
  chain->rotate(0,0,angle);
  back_gear->rotate(0,0,angle);
  front_wheel->rotate(0,0,angle);
  back_wheel->rotate(0,0,angle);
}

void Cycle::move_to(float x, float y, float z) {
  // Move cycle to specific point
}

void Cycle::turn(int angle) {
  // Turn the cycle with respect to up vector
  handle->rotate(0,angle,0);
}