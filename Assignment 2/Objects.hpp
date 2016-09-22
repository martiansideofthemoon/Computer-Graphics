#include <iostream>
#include <GL/glut.h>
#include <vector>

#ifndef _OBJECTS_HPP_
#define _OBJECTS_HPP_

void vertexcopy(float *src,float *dest);

// A simple class that represents a node in the hierarchy tree
class BaseObject {
private:
  std::vector<BaseObject*> children;
  BaseObject* parent;
  int rx, ry, rz;

public:
  BaseObject();
  void add_child(BaseObject*);
  virtual void render();
  void render_tree();
  virtual void rotate(int rotate_x, int rotate_y, int rotate_z);
};

#endif
