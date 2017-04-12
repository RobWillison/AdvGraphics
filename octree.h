#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "vertex.h"
#include "AABoundingBox.h"
#include <string>
#include <vector>

struct OctreeLeaf
{
  AABoundingBox *boundingBox;
  std::vector<Object*> obj_list;
};


struct OctreeNode
{
  AABoundingBox *boundingBox;
  OctreeLeaf *leaf;
  OctreeNode *childern[8];
};


class Octree {
  OctreeNode *root;
  int count;
 public:
  Octree();
  OctreeNode *build(Object *obj_list, Vertex sceneTop, Vertex sceneBottom);
  OctreeNode *createTree(OctreeNode *parentNode, Object *object, int limit);
  bool containsObject(AABoundingBox *boundingBox, Object *object);
  std::vector<Object*> getObjects(AABoundingBox *boundingBox, Object *object);
  void findObjects(Ray &ray, Hit *hit);
  void searchTree(Ray &ray, OctreeNode *node, Hit *hit);
  bool testForShadow(Ray &ray);
  bool testObjectIntersection(Ray &ray, OctreeNode *node);
};

#endif
