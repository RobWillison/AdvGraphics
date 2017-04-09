// Sphere object

#include <math.h>
#include <stdio.h>
#include <algorithm>

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
  //Ray X = px + dx*t
  //Axd^2 + Byd^2 + Czd^2 + Dxdyd + Exdzd + Fydzd
  double dx = ray.D.x;
  double dy = ray.D.y;
  double dz = ray.D.z;
  double px = ray.P.x;
  double py = ray.P.y;
  double pz = ray.P.z;

  //http://www.bmsc.washington.edu/people/merritt/graphics/quadrics.html
  //Apart = a*dx^2 + e*dy^2 + h*dz^2 + b*dx*dy + c*dx*dz + f*dy*dz
  //Bpart = 2*a*px*dx + 2*e*py*dy + 2*h*pz*dz + b * (px * dy + py * dx) + c * (px * dz + pz * dx) + f * (py * dz + pz * dy) + d * dx + g * dy + i * dz
  //Cpart = a*px^2 + e*py^2 + h*pz^2 + b*px*py + c*px*pz + f*py*pz + d*px + g*py + i*pz + j

  double aPart = term[0] * pow(dx, 2) + term[4] * 
  double bPart =
  double cPart =


  //Quadratic Equation -b +- sqrt(b^2 - 4 * a * c) / 2 * a
  double insideSqrt = pow(bPart, 2) - 4 * aPart * cPart;

  if (insideSqrt < 0) return false;

  double t = (-bPart + sqrt(insideSqrt)) / 2 * aPart;
  double t0 = (-bPart - sqrt(insideSqrt)) / 2 * aPart;

  if (t0 < t) t = t0;


  hit->obj = this;
  hit->t = t;

  hit->p.x = dx*t + px;
  hit->p.y = dy*t + py;
  hit->p.z = dz*t + pz;

  //http://marctenbosch.com/photon/mbosch_intersection.pdf
  Vector normal;
  normal.x = -(terms[0] * hit->p.x + terms[3] * hit->p.y + terms[4] * hit->p.z + terms[6]);
  normal.y = -(terms[3] * hit->p.x + terms[1] * hit->p.y + terms[5] * hit->p.z + terms[7]);
  normal.z = -(terms[4] * hit->p.x + terms[5] * hit->p.y + terms[2] * hit->p.z + terms[8]);

  normal.normalise();
  hit->n = normal;

  return true;
}
