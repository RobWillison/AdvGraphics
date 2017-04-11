#include <math.h>


#include <stdio.h>
#include "octree.h"
#include "sphere.h"

Octree::Octree(Scene &scene)
{
  root = new OctreeNode;
  Vertex t1;
  t1.set(3, 1, 3, 1);
  Vertex t2;
  t2.set(1, -1, 1, 1);
  AABoundingBox *boundingBox = new AABoundingBox(t1, t2);

  root->boundingBox = boundingBox;

  this->createTree(root, 1);
}

OctreeNode *Octree::createTree(OctreeNode *parentNode, int limit)
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
        this->createTree(childNode, limit - 1);

        parentNode->childern[count++] = childNode;
      }
    }
  }

  return parentNode;
}