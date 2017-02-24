#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "light.h"

class PointLight: public Light {
  Vector direction;
  Vertex position;
  Colour intensity;
 public:
  PointLight(Vertex &p, Colour &c);
  PointLight(Vertex &p, Vector &d, Colour &c);
  virtual void getLightProperties(Vertex &pos, Vector *ldir, Colour *i);
};

#endif
