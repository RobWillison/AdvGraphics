#include <math.h>
#include <stdio.h>
#include "octree.h"
#include "sphere.h"
#include <algorithm>

Octree::Octree(){
  root = new OctreeNode;
  count = 0;
}

OctreeNode *Octree::build(Object *obj_list, Vertex sceneTop, Vertex sceneBottom)
{
  AABoundingBox *boundingBox = new AABoundingBox(sceneTop, sceneBottom);

  root->boundingBox = boundingBox;
  root->leaf = (OctreeLeaf*)0;

  return this->createTree(root, obj_list, 3);
}

void Octree::findObjects(Ray &ray, Hit *hit)
{
  this->searchTree(ray, root, hit);
}

bool Octree::testForShadow(Ray &ray)
{
  bool result = testObjectIntersection(ray, root);

  return result;
}

bool Octree::testObjectIntersection(Ray &ray, OctreeNode *node)
{
  Hit *testHit = new Hit();
  if (node->leaf != (OctreeLeaf*)0)
  {
    for (int i = 0; i < node->leaf->obj_list.size(); i++)
    {
      if(node->leaf->obj_list[i]->intersect(ray, testHit) == true)
      {
        return true;
      }
    }
    return false;
  }

  //Check the childer which the ray interesects
  for (int i = 0; i < 8; i++)
  {
    if (node->childern[i] == (OctreeNode*)0) continue;
    if (node->childern[i]->boundingBox->intersect(ray))
    {
      if(this->testObjectIntersection(ray, node->childern[i]))
      {
        return true;
      }
    }
  }

  return false;
}

void Octree::searchTree(Ray &ray, OctreeNode *node, Hit *hit)
{
  Hit *testHit = new Hit();
  if (node->leaf != (OctreeLeaf*)0)
  {
    for (int i = 0; i < node->leaf->obj_list.size(); i++)
    {
      if (node->leaf->obj_list[i]->lastRay == ray.number) continue;

      if (node->leaf->obj_list[i]->intersect(ray, testHit) == true)
      {
        node->leaf->obj_list[i]->lastRay = ray.number;
        if (testHit->t < hit->t)
        {
        	hit->obj = testHit->obj;
        	hit->t = testHit->t;
        	hit->n = testHit->n;
        	hit->p = testHit->p;
        }
      }
    }

    return;
  }

  //Check the childer which the ray interesects
  for (int i = 0; i < 8; i++)
  {
    if (node->childern[i] == (OctreeNode*)0) continue;
    if (node->childern[i]->boundingBox->intersect(ray))
    {
      this->searchTree(ray, node->childern[i], hit);
    }
  }
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