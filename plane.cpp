// Sphere object

#include <math.h>
#include <stdio.h>

#include "plane.h"

// Plane defined as 3 vertexs (in world space)

Plane::Plane(Vertex &v1, Vertex &v2, Vertex &v3)
{
  vertexs[0] = v1;
  vertexs[1] = v2;
  vertexs[2] = v3;
}

// Plane intersection test
bool Plane::intersect(Ray &ray, Hit *hit)
{
  Vector a;
  Vector b;

  //Find vectors
  a.set(vertexs[1].x - vertexs[0].x, vertexs[1].y - vertexs[0].y, vertexs[1].z - vertexs[0].z);
  b.set(vertexs[2].x - vertexs[1].x, vertexs[2].y - vertexs[1].y, vertexs[2].z - vertexs[1].z);

  //Get normal
  Vector normal = a.cross(b);
  normal.normalise();

  double D =  normal.x*vertexs[0].x + normal.y*vertexs[0].y + normal.z*vertexs[0].z;

  double divisor = normal.dot(ray.D);
  //Cannot divide by 0, so if < 0 no intersection
  if (divisor == 0.0){
    return false;
  }

  hit->obj = this;
  //Find intersection point
  double intersectPoint = (-(normal.x*ray.P.x + normal.y*ray.P.y + normal.z*ray.P.z) + D) / divisor;
  hit->t = intersectPoint;
  //If behind ray
  if (hit->t < 0.0) return false;

  hit->p.x = ray.D.x*intersectPoint + ray.P.x;
  hit->p.y = ray.D.y*intersectPoint + ray.P.y;
  hit->p.z = ray.D.z*intersectPoint + ray.P.z;

  hit->n = normal;
  hit->n.normalise();

  return true;
}

bool Plane::boundingBoxIntersect(AABoundingBox *boundingBox)
{
  //Needs implementing
  return true;
}

