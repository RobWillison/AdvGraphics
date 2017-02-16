#include <math.h>

#include "vector.h"

Vector::Vector()
{
  x = 0.0;
  y = 0.0;
  z = 0.0;
}

void Vector::set(double px, double py, double pz)
{
  x = px;
  y = py;
  z = pz;
}

void Vector::normalise(void)
{
  double l;

  // get the length of the vector
  l = x*x+y*y+z*z;
  l = sqrt(l);

  // divide elements by length to transform vector to unit length
  x = x/l;
  y = y/l;
  z = z/l;
}

// compute the dot product of the vector.
double Vector::dot(const Vector &b)
{
  return (x*b.x)+(y*b.y)+(z*b.z);
}
