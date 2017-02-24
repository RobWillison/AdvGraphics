#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vertex.h"
#include "vector.h"
#include "ray.h"
#include "scene.h"

class Camera {
  Vertex position;
  Vector upVector;
  Vector lookatVector;
  Vertex widowTopLeft;
  Vertex windowBottomRight;
 public:
  Camera(Vertex &pos, Vector &vup, Vector &vlookat, Vertex topLeft, Vertex bottomRight);
  Ray produceRay(int width, int height, double x, double y);
  Colour traceRay(int width, int height, int x, int y, Scene *scene);
  Colour antiAliasTrace(int width, int height, int x, int y, Scene *scene);
};

#endif
