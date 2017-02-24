// Sphere object

#include <math.h>
#include <stdio.h>
#include <algorithm>

#include "quadratic.h"

// Triangle defined as 3 vertexs (in world space)

Quadratic::Quadratic(double equationTerms[9])
{
  terms = equationTerms;
}

// Triangle intersection test

bool Quadratic::intersect(Ray &ray, Hit *hit)
{
  //Ax^2 + By^2 + Cz^2 + Dxy + Exz + Fyx + Gx + Hy + Iz + J = 0
  //Ray X = px + dx*t
  //Axd^2 + Byd^2 + Czd^2 + Dxdyd + Exdzd + Fydzd
  double dx = ray.D.x;
  double dy = ray.D.y;
  double dz = ray.D.z;
  double px = ray.P.x;
  double py = ray.P.y;
  double pz = ray.P.z;

  double Apart = (terms[0] * (dx*dx)) + (terms[1] * (dy*dy)) + (terms[2] * (dz*dz)) + terms[3]*dx*dy + terms[4]*dx*dz + terms[5]*dy*dz;
  if (Apart == 0.0) return false;

  double Bpart = 2*terms[0]*px*dx + 2*terms[1]*py*dy + 2*terms[2]*pz*dz + terms[3]*(px*dy + pz*dx) + terms[4]*(px*dz + pz*dx) + terms[5]*(py*dz + pz*dy) + terms[6]*dx + terms[7]*dy + terms[8]*dz;

  double Cpart = terms[0]*px*px + terms[1]*py*py + terms[2]*pz*pz + terms[3]*px*py + terms[4]*px*pz + terms[5]*py*pz + terms[6]*px + terms[7]*py + terms[8]*pz + terms[9];

  double t0 = ((Bpart * -1.0) - (sqrt(Bpart*Bpart - 4*Apart*Cpart)) / 2*Apart);
  double t = ((Bpart * -1.0) + sqrt(Bpart*Bpart - 4*Apart*Cpart)) / 2*Apart;

  if (t0 < t) t = t0;

  hit->obj = this;
  hit->t = t;
  hit->p.x = dx*t + px;
  hit->p.y = dy*t + py;
  hit->p.z = dz*t + pz;

  return true;
}
