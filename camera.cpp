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
}

Ray Camera::produceRay(int width, int height, double x, double y)
{
  //Create orthogonal basis w, u, v
  Vector w;
  w.x = position.x - lookatVector.x;
  w.y = position.y - lookatVector.y;
  w.z = position.z - lookatVector.z;
  w.normalise();

  Vector u = upVector.cross(w);
  u.normalise();

  Vector v = w.cross(u);
  v.normalise();
  // Create viewing ray starting at camera position
  Ray viewingRay;
  viewingRay.P = position;
  //Set ray number, used in the octree
  viewingRay.number = clock();

  //45 degrees
  double fieldOfView = (double) (45 / 2.0f) * (M_PI / 180);
  double d = 0.5f;
  //Calculate the x and y angles of the viewing ray
  double angleX = fieldOfView * ((x - width/2) / width/2);
  double angleY = fieldOfView * ((y - height/2) / height/2);
  //Get the distance between center of image plane and plane intersection point
  double distanceU = atan(angleX) * d;
  double distanceV = atan(angleY) * d;
  //Get center point
  Vertex center = position;
  center.x = center.x - w.x * d;
  center.y = center.y - w.y * d;
  center.z = center.z - w.z * d;
  Vertex pointOnScreen = center;
  //Find point on screen
  pointOnScreen.x = center.x + (u.x * distanceU) + (v.x * distanceV);
  pointOnScreen.y = center.y + (u.y * distanceU) + (v.y * distanceV);
  pointOnScreen.z = center.z + (u.z * distanceU) + (v.z * distanceV);
  //Set viewing ray
  viewingRay.D.x = - position.x + pointOnScreen.x;
  viewingRay.D.y = - position.y + pointOnScreen.y;
  viewingRay.D.z = - position.z + pointOnScreen.z;
  viewingRay.n = 1.0f;

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

  //For 0 to 1 in 0.25 steps in x and y direction
  for (int i = 0; i < 4; i++)
  {
    xDouble = xDouble + 0.25f;
    for (int j = 0; j < 4; j++)
    {
      yDouble = yDouble + 0.25f;
      //Produce ray
      Ray ray = produceRay(width, height, xDouble, yDouble);
      // Trace primary ray
      Colour col = scene->raytrace(ray, 3);
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

  Colour col = antiAliasTrace(width, height, x, y, scene);

  return col;
}

Vertex Camera::getPosition()
{
  return position;
}
