// Sphere object

#include <math.h>

#include "triangle.h"

// Triangle defined as 3 vertexs (in world space)

Triangle::Triangle(Vertex &v1, Vertex &v2, Vertex &v3)
{
  vertexs[0] = v1;
  vertexs[1] = v2;
  vertexs[3] = v3;
}

// Triangle intersection test

bool Triangle::intersect(Ray &ray, Hit *hit)
{
  Vector a;
  Vector b;
  Vector c;

  a.set(0.0, 0.0, 0.0, vertexs[0]);
  b.set(0.0, 0.0, 0.0, vertexs[1]);
  c.set(0.0, 0.0, 0.0, vertexs[2]);

  return true;
}
