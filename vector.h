#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "vertex.h"

class Vector {
 public:
  double x;
  double y;
  double z;
  Vector(void);
  void set(double x, double y, double z);
  void normalise(void);
  double dot(const Vector &b);
  Vector cross(const Vector &b);
  double length();
};


#endif
