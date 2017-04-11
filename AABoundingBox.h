#ifndef _AABoundingBox_H_
#define _AABoundingBox_H_

#include "object.h"

// The sphere object class, extends Object class.

class AABoundingBox {
 public:
  Vertex topCorner;
  Vertex bottomCorner;
  AABoundingBox(Vertex &max, Vertex &min);
  bool intersect(Ray &ray);
};

#endif
