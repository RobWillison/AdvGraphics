// Sphere object

#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "quadratic.h"

// Triangle defined as 3 vertexs (in world space)

Quadratic::Quadratic(double equationTerms[10])
{
  terms = equationTerms;
}

// Triangle intersection test

bool Quadratic::intersect(Ray &ray, Hit *hit)
{
  //Ax^2 + 2Bxy + 2Cxz + 2Dx + Ey^2 + 2Fyz + 2Gy + Hz^2 + Iz + J = 0

  //http://www.bmsc.washington.edu/people/merritt/graphics/quadrics.html

  double dx = ray.D.x;
  double dy = ray.D.y;
  double dz = ray.D.z;
  double px = ray.P.x;
  double py = ray.P.y;
  double pz = ray.P.z;

  double a = terms[0];
  double b = terms[1];
  double c = terms[2];
  double d = terms[3];
  double e = terms[4];
  double f = terms[5];
  double g = terms[6];
  double h = terms[7];
  double i = terms[8];
  double j = terms[9];

  double aPart = a*pow(dx, 2) + e*pow(dy, 2) + h*pow(dz, 2) + b*dx*dy + c*dx*dz + f*dy*dz;
  double bPart = 2*a*px*dx + 2*e*py*dy + 2*h*pz*dz + b*(px*dy + py*dx) + c*(px*dz + pz*dx) + f*(pz*dy + py*dz) + d*dx + g*dy + i*dz;
  double cPart = a*pow(px, 2) + e*pow(py, 2) + h*pow(pz, 2) + b*px*py + c*px*pz + f*py*pz + d*px + g*py + i*pz + j;

  //Quadratic Equation -b +- sqrt(b^2 - 4 * a * c) / 2 * a
  double insideSqrt = pow(bPart, 2) - 4 * aPart * cPart;

  if (insideSqrt < 0) return false;

  double t = (-bPart + sqrt(insideSqrt)) / 2 * aPart;
  double t0 = (-bPart - sqrt(insideSqrt)) / 2 * aPart;


  if (t0 < t) t = t0;

  if (aPart == 0)
  {
    t = -cPart/bPart;
  }

  hit->obj = this;
  hit->t = t;

  hit->p.x = dx*t + px;
  hit->p.y = dy*t + py;
  hit->p.z = dz*t + pz;

  //http://marctenbosch.com/photon/mbosch_intersection.pdf
  Vector normal;
  normal.x = a * hit->p.x + b * hit->p.y + c * hit->p.z + d;
  normal.y = b * hit->p.x + e * hit->p.y + f * hit->p.z + g;
  normal.z = c * hit->p.x + f * hit->p.y + h * hit->p.z + i;

  //printf("Quadra - %f %f %f\n", normal.x, normal.y, normal.z);

  normal.normalise();
  hit->n = normal;

  return true;
}
