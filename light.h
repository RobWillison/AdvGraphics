#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vertex.h"
#include "vector.h"
#include "colour.h"

// The base class for lights.

class Light {
  Light *lt_next;
 public:
  Light(void);
  void link(Light *light);
  Light *next(void);
  virtual void getLightProperties(Vertex &pos, Vector *ldir, Colour *i);
};

#endif
