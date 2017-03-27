// Sphere object

#include <math.h>
#include <stdio.h>

#include "camera.h"

Camera::Camera(Vertex &camPosition, Vector &vUp, Vector &vLookat)
{
  position = camPosition;
  upVector = vUp;
  lookatVector = vLookat;
  upVector.normalise();
  lookatVector.normalise();

  //Get plane tangential to lookat given distance aways
  //Calculate windows points
}

Ray Camera::produceRay(int width, int height, double x, double y)
{
  Vector w;
  w.x = position.x - lookatVector.x;
  w.y = position.y - lookatVector.y;
  w.z = position.z - lookatVector.z;
  w.normalise();

  Vector u = upVector.cross(w);
  u.normalise();

  Vector v = w.cross(u);
  v.normalise();

  Ray viewingRay;
  viewingRay.P = position;

  //45 degrees
  double fieldOfView = (double) (45 / 2.0f) * (M_PI / 180)

  Vertex center = position - w * d;
  Vertex leftBottom = center - u * width/2 - v * height/2;

  

  viewingRay.D.normalise();
  //printf("%f %f %f\n",viewingRay.D.x, viewingRay.D.y, viewingRay.D.z);
  return viewingRay;
}

Colour Camera::antiAliasTrace(int width, int height, int x, int y, Scene *scene)
{
  double xDouble = (double) x;
  double yDouble = (double) y;
  int samples = 0;
  Colour averageCol;
  averageCol.clear();

  for (int i = 0; i < 4; i++)
  {
    xDouble = xDouble + 0.25f;
    for (int j = 0; j < 4; j++)
    {
      yDouble = yDouble + 0.25f;
      Ray ray = produceRay(width, height, xDouble, yDouble);
      // Trace primary ray
      Colour col = scene->raytrace(ray,6);
      averageCol.red = averageCol.red + col.red;
      averageCol.green = averageCol.green + col.green;
      averageCol.blue = averageCol.blue + col.blue;
      averageCol.alpha = averageCol.alpha + col.alpha;
      samples++;
    }
  }
  averageCol.red = averageCol.red / samples;
  averageCol.green = averageCol.green / samples;
  averageCol.blue = averageCol.blue / samples;
  averageCol.alpha = averageCol.alpha / samples;

  return averageCol;
}



Colour Camera::traceRay(int width, int height, int x, int y, Scene *scene)
{
  double xFloat = (double) x;
  double yFloat = (double) y;

  Colour col;

  for (xFloat; xFloat < x + 1; xFloat = xFloat + 0.1f )
  {
    for (yFloat; yFloat < y + 1; yFloat = yFloat + 0.1f )
    {
      col = antiAliasTrace(width, height, xFloat, yFloat, scene);
    }
  }


  return col;
}

Vertex Camera::getPosition()
{
  return position;
}
