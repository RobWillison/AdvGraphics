#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"
#include "light.h"

class Camera;

class Scene {
  Object *scache;
  Object *obj_list;
  Light  *light_list;
  Camera *camera;
public:
  Scene(void);
  void addObject(Object &obj);
  void setCamera(Camera &cam);
  void addLight(Light &light);
  Colour raytrace(Ray &ray, int level);
  bool shadowtrace(Ray &ray, double tlimit);
  int isShadowed(Vector xldir, Vertex position);
  Colour reflectedRay();
  Colour refractedRay(float closestN, Vertex &position, Ray &ray, Vector &normal, Vector &view, int level);
  Object *getObjectList();
};

#endif
