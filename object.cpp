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
