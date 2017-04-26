// a simple directional light source

#include <math.h>
#include <stdio.h>

#include "point_light.h"

//Define point and colour
PointLight::PointLight(Vertex &p, Colour &i)
{
  position.x = -p.x;
  position.y = -p.y;
  position.z = -p.z;
  direction.set(0.0, 0.0, 0.0);
  intensity.set(i.getRed(),i.getGreen(),i.getBlue(),i.getAlpha());
}

//Define point, vector and colour
PointLight::PointLight(Vertex &p, Vector &d, Colour &i)
{
  position.x = -p.x;
  position.y = -p.y;
  position.z = -p.z;
  direction.set(d.x, d.y, d.z);
  direction.normalise();

  intensity.set(i.getRed(),i.getGreen(),i.getBlue(),i.getAlpha());
}

// provide the intensity and direction from which light arrives at given vertex
void PointLight::getLightProperties(Vertex &pos, Vector *ldir, Colour *i)
{
  //Get direction to intersection
  ldir->x = position.x - pos.x;
  ldir->y = position.y - pos.y;
  ldir->z = position.z - pos.z;

  // the intensity is always the same (not dependent on where it's going
  i->set(intensity.getRed(),intensity.getGreen(),intensity.getBlue(),intensity.getAlpha());
  //No direction set
  if (direction.length() == 0.0) return;

  //Get vector pointing at light from intersection
  Vector directionToPoint;
  directionToPoint.x = pos.x - position.x;
  directionToPoint.y = pos.y - position.y;
  directionToPoint.z = pos.z - position.z;

  directionToPoint.normalise();
  direction.normalise();

  double dotProduct = directionToPoint.dot(direction);

  if (dotProduct <= -0.0) {
    i->clear();
    return;
  }
  //Scale by angle
  i->red = i->red * pow(dotProduct, 0.5);
  i->green = i->green * pow(dotProduct, 0.5);
  i->blue = i->blue * pow(dotProduct, 0.5);
}
