// Sphere object

#include <math.h>
#include <stdio.h>
#include <algorithm>
#include "AABoundingBox.h"

AABoundingBox::AABoundingBox(Vertex &max, Vertex &min)
{
  topCorner = max;
  bottomCorner = min;
}

// Sphere intersection test

bool AABoundingBox::intersect(Ray &ray)
{
  //https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
  //Find Min and Max X axis intersections
  double tMinX = (bottomCorner.x - ray.P.x) / ray.D.x;
  double tMaxX = (topCorner.x - ray.P.x) / ray.D.x;
  if (tMinX > tMaxX) std::swap(tMinX, tMaxX);

  //Find Min and Max Y axis intersections
  double tMinY = (bottomCorner.y - ray.P.y) / ray.D.y;
  double tMaxY = (topCorner.y - ray.P.y) / ray.D.y;
  if (tMinY > tMaxY) std::swap(tMinY, tMaxY);

  //Check ray intersets X and Y axis inside bounding box
  if ((tMinX > tMaxY) || (tMinY > tMaxX))
  {
    return false;
  }

  double maxT = tMaxX;
  double minT = tMinX;

  if (maxT > tMaxY) maxT = tMaxY;
  if (minT < tMinY) minT = tMinY;


  //Find Min and Max Z axis intersections
  double tMinZ = (bottomCorner.z - ray.P.z) / ray.D.z;
  double tMaxZ = (topCorner.z - ray.P.z) / ray.D.z;
  if (tMinZ > tMaxZ) std::swap(tMinZ, tMaxZ);

  if ((minT > tMaxZ) || (tMinZ > maxT))
  {
    return false;
  }

  if (maxT > tMaxZ) maxT = tMaxZ;
  if (minT < tMinZ) minT = tMinZ;

  if (maxT <= -0) return false;

  return true;
}
