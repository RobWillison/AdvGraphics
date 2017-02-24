#ifndef _QUADRATIC_H_
#define _QUADRATIC_H_

#include "object.h"

// The triangle object class, extends Object class.

class Quadratic: public Object {
  double *terms;
 public:
  Quadratic(double equationTerms[9]);
  bool intersect(Ray &ray, Hit *h);
};

#endif
