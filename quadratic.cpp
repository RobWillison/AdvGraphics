// Sphere object

#include <math.h>
#include <stdio.h>

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
  double dx;
  double Apart = (terms[0] * dx)


  return false;
}
