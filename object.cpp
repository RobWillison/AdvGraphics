// Object - The base class for objects.

#include <math.h>

#include "object.h"

Object::Object(void)
{
}

void Object::link(Object *obj)
{
  obj_next = obj;
}

Object *Object::next(void)
{
  return obj_next;
}

void Object::setMaterial(Material *m)
{
  obj_mat = m;
}


Colour Object::getColour()
{
  Colour a;
  return a;
}

bool Object::sintersect(Ray &ray, Hit *h, double tl)
{
  bool res;
  Hit nh;

  if (this->intersect(ray, &nh) == true) // does the object intersect
  {
    if (nh.t > tl) return false; // is the intersection beyond the light position. 

    *h = nh;

    return true; // then return the hit
  }

  return false;
}
