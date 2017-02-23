#ifndef _MODEL_H_
#define _MODEL_H_

#include "vertex.h"

class PlyModel {
  Vertex vertexs[];
 public:
  PlyModel(char fileName[]);
};

#endif
