#include <math.h>
#include <stdio.h>
#include "octree.h"
#include "sphere.h"
#include <algorithm>

Octree::Octree(){
  root = new OctreeNode;
}

OctreeNode *Octree::build(Object *obj_list, Vertex sceneTop, Vertex sceneBottom)
{
  AABoundingBox *boundingBox = new AABoundingBox(sceneTop, sceneBottom);

  root->boundingBox = boundingBox;
  root->leaf = (OctreeLeaf*)0;

  return this->createTree(root, obj_list, 1);
}

std::vector<Object*> Octree::findObjects(Ray &ray)
{
  std::vector<Object*> objects = this->searchTree(ray, root);
  return objects;
}

std::vector<Object*> Octree::searchTree(Ray &ray, OctreeNode *node)
{

  if (node->leaf != (OctreeLeaf*)0)
  {
    return node->leaf->obj_list;
  }
  std::vector<Object*> objects;
  //Check the childer which the ray interesects
  for (int i = 0; i < 8; i++)
  {
    if (node->childern[i] == (OctreeNode*)0) continue;
    if (node->childern[i]->boundingBox->intersect(ray))
    {
      printf("Bounding Box\n");
      std::vector<Object*> childObjects = this->searchTree(ray, node->childern[i]);
      objects.insert(objects.end(), childObjects.begin(), childObjects.end());
    }
  }

  return objects;
}

bool Octree::containsObject(AABoundingBox *boundingBox, Object *obj_list)
{
  Object *obj = obj_list;

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

std::vector<Object*> Octree::getObjects(AABoundingBox *boundingBox, Object *obj_list)
{
  std::vector<Object*> objects;

  Object *obj = obj_list;

  while (obj != (Object *)0)
  {
    if (obj->boundingBoxIntersect(boundingBox))
    {
      objects.push_back(obj);
    }
    obj = obj->next();
  }

  return objects;
}


OctreeNode *Octree::createTree(OctreeNode *parentNode, Object *obj_list, int limit)
{
  if (limit <= 0)
  {
    OctreeLeaf *leaf = new OctreeLeaf;
    parentNode->leaf = leaf;
    leaf->obj_list = this->getObjects(parentNode->boundingBox, obj_list);
    return parentNode;
  }

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
        newBottom.set(x - sideLength, y - sideLength, z - sideLength, 1);

        AABoundingBox *childBox = new AABoundingBox(newTop, newBottom);
        OctreeNode *childNode = new OctreeNode;
        childNode->boundingBox = childBox;
        childNode->leaf = (OctreeLeaf*)0;

        if (this->containsObject(childBox, obj_list))
        {
          this->createTree(childNode, obj_list, limit - 1);
        } else {
          //Nothing in this area
          childNode = (OctreeNode*)0;
        }

        parentNode->childern[count++] = childNode;
      }
    }
  }

  return parentNode;
}