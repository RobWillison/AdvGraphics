#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object.h"

// The sphere object class, extends Object class.

class Sphere: public Object {
  Vertex sp;
  float  r;
 public:
  Sphere(Vertex &psp, float pr);
  bool intersect(Ray &ray, Hit *h);
};

#endif
