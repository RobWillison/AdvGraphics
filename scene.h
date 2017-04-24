#ifndef _SCENE_H_
#define _SCENE_H_

#include "object.h"
#include "light.h"
#include "octree.h"

class Camera;

class Scene {
  Object *scache;
  Object *obj_list;
  Light  *light_list;
  Camera *camera;
  Octree *tree;
  int count;
public:
  Scene(void);
  void addObject(Object &obj);
  void createOctree(Vertex top, Vertex bottom);
  void setCamera(Camera &cam);
  void addLight(Light &light);
  Colour raytrace(Ray &ray, int level);
  bool shadowtrace(Ray &ray, double tlimit);
  int isShadowed(Vector xldir, Vertex position);
  float calculateFresnelKr(Object *closest, Ray &refractedRay, Ray &viewingRay, Vector &normal);
  Ray calculateRefractedRay(Object *closest, Vertex &position, Ray &ray, Vector &normal, Vector &view);
  Object *getObjectList();
};

#endif
