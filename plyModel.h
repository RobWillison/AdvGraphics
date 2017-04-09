#ifndef _MODEL_H_
#define _MODEL_H_

#include "vertex.h"
#include <string>
#include <vector>
#include "triangle.h"
#include "scene.h"
#include "material.h"

struct Model {
  int vertexNum;
  int faceNum;
  std::vector<Vertex> vertexs;
  std::vector<Triangle> faces;
};

class PlyModel {
  Vertex vertexs[];
  Model model;
 public:
  PlyModel(char fileName[]);
  Model ParseFile(char fileName[]);
  double * ParseLine(std::string line);
  void AddToScene(Scene *scene, Material *m, Vertex position, int scale);
};

#endif
