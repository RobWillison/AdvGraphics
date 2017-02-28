// Sphere object

#include <math.h>
#include <stdio.h>

#include "camera.h"

Camera::Camera(Vertex &camPosition, Vector &vUp, Vector &vLookat)
{
  position = camPosition;
  upVector = vUp;
  lookatVector = vLookat;
  widowTopLeft = topLeft;
  windowBottomRight = bottomRight;

  //Get plane tangential to lookat given distance aways
  //Calculate windows points
}

Ray Camera::produceRay(int width, int height, double x, double y)
{
  Ray viewingRay;
  viewingRay.P = position;
  double w = widowTopLeft.z;
  double u = windowBottomRight.x + (widowTopLeft.x - windowBottomRight.x)*((x + 0.5) / width);
  double v = windowBottomRight.y + (widowTopLeft.y - windowBottomRight.y)*((y + 0.5) / height);
  Vector uVec = lookatVector.cross(upVector);

  viewingRay.D.z = (w * lookatVector.z) + (v * upVector.z) + (u * uVec.z) - position.z;
  viewingRay.D.y = (w * lookatVector.y) + (v * upVector.y) + (u * uVec.y) - position.y;
  viewingRay.D.x = (w * lookatVector.x) + (v * upVector.x) + (u * uVec.x) - position.x;


  viewingRay.D.normalise();


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
    xDouble = xDouble + 1/4;
    for (int j = 0; j < 4; j++)
    {
      yDouble = yDouble + 1/4;
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

