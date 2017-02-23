#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object.h"

// The triangle object class, extends Object class.

class Triangle: public Object {
  Vertex vertexs [3];
 public:
  Triangle(Vertex &v1, Vertex &v2, Vertex &v3);
  bool intersect(Ray &ray, Hit *h);
};

#endif
