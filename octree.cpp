#include <math.h>


#include <stdio.h>
#include "octree.h"
#include "sphere.h"

Octree::Octree(Scene &scene, Vertex sceneTop, Vertex sceneBottom)
{
  root = new OctreeNode;

  AABoundingBox *boundingBox = new AABoundingBox(sceneTop, sceneBottom);

  root->boundingBox = boundingBox;

  this->createTree(root, scene, 1);
}

bool Octree::containsObject(AABoundingBox *boundingBox, Scene &scene)
{
  Object *obj = scene.getObjectList();

  while (obj != (Object *)0)
  {
    if (obj->boundingBoxIntersect(boundingBox))
    {
      return true;
    }
    obj = obj->next();
  }

  return false;
}

OctreeNode *Octree::createTree(OctreeNode *parentNode, Scene &scene, int limit)
{
  printf("%d\n", limit);
  if (limit <= 0) return parentNode;

  Vertex top = parentNode->boundingBox->topCorner;
  Vertex bottom = parentNode->boundingBox->bottomCorner;

  //Get side length, its a cube of all sides the same
  double sideLength = (top.x - bottom.x) / 2;

  int count = 0;
  for (int i = 1; i < 3; i++)
  {
    double x = (top.x - bottom.x)/i + bottom.x;
    for (int j = 1; j < 3; j++)
    {
      double y = (top.y - bottom.y)/j + bottom.y;
      for (int k = 1; k < 3; k++)
      {
        double z = (top.z - bottom.z)/k + bottom.z;
        Vertex newTop;
        newTop.set(x, y, z, 1);
        Vertex newBottom;
        newTop.set(x - sideLength, y - sideLength, z - sideLength, 1);

        AABoundingBox *childBox = new AABoundingBox(newTop, newBottom);
        OctreeNode *childNode = new OctreeNode;
        childNode->boundingBox = childBox;

        if (this->containsObject(childBox, scene)) this->createTree(childNode, scene, limit - 1);

        parentNode->childern[count++] = childNode;
      }
    }
  }

  return parentNode;
}