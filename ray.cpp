#include "ray.h"

// return the position on the ray given the parametric value t.
Vertex Ray::position(double t)
{
  Vertex v;

  v.x = P.x + t * D.x;
  v.y = P.y + t * D.y;
  v.z = P.z + t * D.z;
  v.w = P.w;

  return v;
}
