// Sphere object

#include <math.h>
#include <stdio.h>

#include "triangle.h"

// Triangle defined as 3 vertexs (in world space)

Triangle::Triangle(Vertex &v1, Vertex &v2, Vertex &v3)
{
  vertexs[0] = v1;
  vertexs[1] = v2;
  vertexs[2] = v3;
}

Triangle::Triangle()
{

}

void Triangle::set(Vertex &v1, Vertex &v2, Vertex &v3)
{
  vertexs[0] = v1;
  vertexs[1] = v2;
  vertexs[2] = v3;
}

bool Triangle::boundingBoxIntersect(AABoundingBox *boundingBox)
{
  //Check intersection with X planes
  double minX = boundingBox->bottomCorner.x;
  double maxX = boundingBox->topCorner.x;

  if ((minX > vertexs[0].x) && (minX > vertexs[1].x) && (minX > vertexs[2].x)) return false;
  if ((maxX < vertexs[0].x) && (maxX < vertexs[1].x) && (maxX < vertexs[2].x)) return false;

  //Check intersection with X planes
  minX = boundingBox->bottomCorner.y;
  maxX = boundingBox->topCorner.y;

  if ((minX > vertexs[0].y) && (minX > vertexs[1].y) && (minX > vertexs[2].y)) return false;
  if ((maxX < vertexs[0].y) && (maxX < vertexs[1].y) && (maxX < vertexs[2].y)) return false;

  //Check intersection with X planes
  minX = boundingBox->bottomCorner.z;
  maxX = boundingBox->topCorner.z;

  if ((minX > vertexs[0].z) && (minX > vertexs[1].z) && (minX > vertexs[2].z)) return false;
  if ((maxX < vertexs[0].z) && (maxX < vertexs[1].z) && (maxX < vertexs[2].z)) return false;

  return true;
}

// Triangle intersection test

bool Triangle::intersect(Ray &ray, Hit *hit)
{
  Vector a;
  Vector b;
  Vector c;

  a.set(vertexs[1].x - vertexs[0].x, vertexs[1].y - vertexs[0].y, vertexs[1].z - vertexs[0].z);
  b.set(vertexs[2].x - vertexs[1].x, vertexs[2].y - vertexs[1].y, vertexs[2].z - vertexs[1].z);
  c.set(vertexs[0].x - vertexs[2].x, vertexs[0].y - vertexs[2].y, vertexs[0].z - vertexs[2].z);

  Vector normal = a.cross(b);
  normal.normalise();

  double D =  normal.x*vertexs[0].x + normal.y*vertexs[0].y + normal.z*vertexs[0].z;

  double divisor = normal.dot(ray.D);

  if (divisor == 0.0){
    return false;
  }

  hit->obj = this;

  double intersectPoint = (-(normal.x*ray.P.x + normal.y*ray.P.y + normal.z*ray.P.z) + D) / divisor;
  hit->t = intersectPoint;

  if (hit->t < 0.0) return false;

  hit->p.x = ray.D.x*intersectPoint + ray.P.x;
  hit->p.y = ray.D.y*intersectPoint + ray.P.y;
  hit->p.z = ray.D.z*intersectPoint + ray.P.z;

  Vector tempVec;
  tempVec.set(hit->p.x - vertexs[0].x, hit->p.y - vertexs[0].y, hit->p.z - vertexs[0].z);
  Vector crossProduct = a.cross(tempVec);

  double length = normal.dot(crossProduct);

  if (length < -0.0){
    return false;
  }

  tempVec.set(hit->p.x - vertexs[1].x, hit->p.y - vertexs[1].y, hit->p.z - vertexs[1].z);
  crossProduct = b.cross(tempVec);

  length = normal.dot(crossProduct);

  if (length < -0.0){
    return false;
  }

  tempVec.set(hit->p.x - vertexs[2].x, hit->p.y - vertexs[2].y, hit->p.z - vertexs[2].z);
  crossProduct = c.cross(tempVec);

  length = normal.dot(crossProduct);

  if (length < -0.0){
    return false;
  }

  hit->n = normal;
  hit->n.normalise();

  return true;
}
