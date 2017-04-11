#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "vertex.h"
#include "vector.h"
#include "colour.h"
#include "ray.h"
#include "hit.h"
#include "colour.h"
#include "material.h"

class AABoundingBox;

class Object {
  Object *obj_next;
 public:
  Material *obj_mat;
  Object(void);
  void link(Object *obj);
  Object *next(void);
  void setMaterial(Material *m);
  Colour getColour();
  virtual bool intersect(Ray &ray, Hit *h) { return false; } // primary and secondary ray intersections
  virtual bool sintersect(Ray &ray, Hit *h, double tl); // shadow ray intersections
  virtual bool boundingBoxIntersect(AABoundingBox *boundingBox) { return false; };
};

#endif
