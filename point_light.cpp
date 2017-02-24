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
  // the direction is always the same (light is infinitely far away)
  ldir->x = position.x - pos.x;
  ldir->y = position.y - pos.y;
  ldir->z = position.z - pos.z;

  // the intensity is always the same (not dependent on where it's going
  i->set(intensity.getRed(),intensity.getGreen(),intensity.getBlue(),intensity.getAlpha());

  if (direction.length() == 0.0) return;

  double dotProduct = ldir->dot(direction);
  i->red = i->red * pow(dotProduct, 0.1);
  i->green = i->green * pow(dotProduct, 0.1);
  i->blue = i->blue * pow(dotProduct, 0.1);

}
