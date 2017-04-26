#ifndef _PLANE_H_
#define _PLANE_H_

#include "object.h"

// The triangle object class, extends Object class.

class Plane: public Object {
 public:
   Vertex vertexs [3];
  Plane(Vertex &v1, Vertex &v2, Vertex &v3);

  void set(Vertex &v1, Vertex &v2, Vertex &v3);
  bool intersect(Ray &ray, Hit *h);
  bool boundingBoxIntersect(AABoundingBox *boundingBox);
};

#endif
