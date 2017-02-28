// a simple directional light source

#include <math.h>
#include <stdio.h>

#include "point_light.h"

PointLight::PointLight(Vertex &p, Colour &i)
{
  position.x = -p.x;
  position.y = -p.y;
  position.z = -p.z;
  direction.set(0.0, 0.0, 0.0);
  intensity.set(i.getRed(),i.getGreen(),i.getBlue(),i.getAlpha());
}

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
  ldir->x = pos.x - position.x;
  ldir->y = pos.y - position.y;
  ldir->z = pos.z - position.z;

  // the intensity is always the same (not dependent on where it's going
  i->set(intensity.getRed(),intensity.getGreen(),intensity.getBlue(),intensity.getAlpha());

  if (direction.length() == 0.0) return;

  Vector directionToPoint;
  directionToPoint.x = position.x - pos.x;
  directionToPoint.y = position.y - pos.y;
  directionToPoint.z = position.z - pos.z;

  double dotProduct = directionToPoint.dot(direction);

  i->red = i->red * dotProduct;
  i->green = i->green * dotProduct;
  i->blue = i->blue * dotProduct;
}
