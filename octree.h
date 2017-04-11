#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "vertex.h"
#include "scene.h"
#include "AABoundingBox.h"
#include <string>

struct OctreeNode
{
  AABoundingBox *boundingBox;
  OctreeNode *childern[8];
};

class Octree {
  OctreeNode *root;
 public:
  Octree(Scene &scene);
  OctreeNode *createTree(OctreeNode *parentNode, int limit);
};

#endif
