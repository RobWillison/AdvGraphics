#include <math.h>

#include "vector.h"
#include "vertex.h"

Vector::Vector()
{
  x = 0.0;
  y = 0.0;
  z = 0.0;
  Vertex position;
}

void Vector::set(double px, double py, double pz)
{
  x = px;
  y = py;
  z = pz;
}

void Vector::set(double px, double py, double pz, Vertex v)
{
  x = px;
  y = py;
  z = pz;
  position = v;
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

Vector Vector::cross(const Vector &b)
{
  Vector crossProduct;
  crossProduct.set(0.0, 0.0, 0.0);

  crossProduct.x = y*b.z - z*b.y;
  crossProduct.y = z*b.x - x*b.z;
  crossProduct.z = x*b.y - y*b.x;

  return crossProduct;
}
