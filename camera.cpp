// Sphere object

#include <math.h>
#include <stdio.h>

#include "camera.h"

Camera::Camera(Vertex &camPosition, Vector &vUp, Vector &vLookat, Vertex topLeft, Vertex bottomRight)
{
  position = camPosition;
  upVector = vUp;
  lookatVector = vLookat;
  widowTopLeft = topLeft;
  windowBottomRight = bottomRight;
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

Colour Camera::traceRay(int width, int height, int x, int y, Scene *scene)
{
  double xFloat = (double) x;
  double yFloat = (double) y;

  Colour col;

  for (xFloat; xFloat < x + 1; xFloat = xFloat + 0.1f )
  {
    for (yFloat; yFloat < y + 1; yFloat = yFloat + 0.1f )
    {
      Ray ray = produceRay(width, height, xFloat, yFloat);
      // Trace primary ray
      col = scene->raytrace(ray,6);
    }
  }


  return col;
}

