// Sphere object

#include <math.h>
#include <stdio.h>
#include "sphere.h"

// Sphere defined as vertex (in world space) and radius

Sphere::Sphere(Vertex &psp, float pr)
{
  sp = psp;
  r  = pr;
}

// Sphere intersection test

bool Sphere::boundingBoxIntersect(AABoundingBox *boundingBox)
{
  //Check X planes
  double minX = boundingBox->bottomCorner.x;
  double maxX = boundingBox->topCorner.x;

  if ((minX > sp.x + r) || (maxX < sp.x - r)) return false;

  double minY = boundingBox->bottomCorner.y;
  double maxY = boundingBox->topCorner.y;

  if ((minY > sp.y + r) || (maxY < sp.y - r)) return false;

  double minZ = boundingBox->bottomCorner.z;
  double maxZ = boundingBox->topCorner.z;

  if ((minZ > sp.z + r) || (maxZ < sp.z - r)) return false;

  return true;
}

bool Sphere::intersect(Ray &ray, Hit *hit)
{
  Vector ro;

  // offset ray by sphere position
  // equivalent to transforming ray into local sphere space

  ro.set(ray.P.x-sp.x,ray.P.y-sp.y,ray.P.z-sp.z);

  float a = (float)ray.D.dot(ray.D);
  float b = (float)(2.0 * ray.D.dot(ro));
  float c = (float)ro.dot(ro) - r*r;

  float disc = b*b - 4*a*c;

  if (disc < 0.0)
  {
    return false; // a negative value indicates no intersection.
  }

  float ds = sqrtf(disc);
  float q;

  if (b < 0.0)
  {
    q = (-b - ds)/2.0f;
  } else
  {
    q = (-b + ds)/2.0f;
  }

  float t0 = q/a;
  float t1 = c/q;

  if (t0>t1)
  {
    float temp = t0;
    t0 = t1;
    t1 = temp;
  }

  if (t1 < 0.0)
  {
    return false;
  }

  // if an intersection has been found, record details in hit object

  hit->obj = this;

  if (t0 < 0.0)
  {
    hit->t = t1;

    hit->p.x = ray.P.x  + t1 * ray.D.x;
    hit->p.y = ray.P.y  + t1 * ray.D.y;
    hit->p.z = ray.P.z  + t1 * ray.D.z;
    hit->p.w = 1.0;
    hit->n.x = hit->p.x - sp.x;
    hit->n.y = hit->p.y - sp.y;
    hit->n.z = hit->p.z - sp.z;
    hit->n.normalise();

    return true;
  }

  hit->t = t0;

  hit->p.x = ray.P.x  + t0 * ray.D.x;
  hit->p.y = ray.P.y  + t0 * ray.D.y;
  hit->p.z = ray.P.z  + t0 * ray.D.z;
  hit->p.w = 1.0;

  hit->n.x = hit->p.x - sp.x;
  hit->n.y = hit->p.y - sp.y;
  hit->n.z = hit->p.z - sp.z;
  hit->n.normalise();

  //printf("SPhere - %f %f %f\n", hit->n.x, hit->n.y, hit->n.z);

  return true;
}
