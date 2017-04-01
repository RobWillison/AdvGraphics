#ifndef _RAY_H_
#define _RAY_H_

#include "vertex.h"
#include "vector.h"

class Ray {
 public:
  int n;
  Vertex P;
  Vector D;
  Vertex position(double t);
};


#endif
