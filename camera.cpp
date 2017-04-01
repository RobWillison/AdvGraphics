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
  double fieldOfView = (double) (45 / 2.0f) * (M_PI / 180);
  double d = 0.5f;

  double angleX = fieldOfView * ((x - width/2) / width/2);
  double angleY = fieldOfView * ((y - height/2) / height/2);

  double distanceU = atan(angleX) * d;
  double distanceV = atan(angleY) * d;

  Vertex center = position;
  center.x = center.x - w.x * d;
  center.y = center.y - w.y * d;
  center.z = center.z - w.z * d;
  Vertex pointOnScreen = center;

  pointOnScreen.x = center.x + (u.x * distanceU) + (v.x * distanceV);
  pointOnScreen.y = center.y + (u.y * distanceU) + (v.y * distanceV);
  pointOnScreen.z = center.z + (u.z * distanceU) + (v.z * distanceV);

  //printf("%f %f %f\n", pointOnScreen.x, pointOnScreen.y, pointOnScreen.z);

  viewingRay.D.x = - position.x + pointOnScreen.x;
  viewingRay.D.y = - position.y + pointOnScreen.y;
  viewingRay.D.z = - position.z + pointOnScreen.z;

  viewingRay.D.normalise();

  viewingRay.n = 1;
  //printf("%f %f %f\n", viewingRay.D.x, viewingRay.D.y, viewingRay.D.z);
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
      Colour col = scene->raytrace(ray,2);
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
