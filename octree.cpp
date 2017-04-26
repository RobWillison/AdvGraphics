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
  //Create root box
  root->boundingBox = boundingBox;
  root->leaf = (OctreeLeaf*)0;
  //Build octree, depth 3
  return this->createTree(root, obj_list, 3);
}

void Octree::findObjects(Ray &ray, Hit *hit)
{
  //Seach the tree for a hit
  this->searchTree(ray, root, hit);
}

bool Octree::testForShadow(Ray &ray)
{
  //Test for a shadow hit
  bool result = testObjectIntersection(ray, root);

  return result;
}

bool Octree::testObjectIntersection(Ray &ray, OctreeNode *node)
{
  //Return true if the ray intersects an object, otherwise false
  Hit *testHit = new Hit();

  //If its a leaf node
  if (node->leaf != (OctreeLeaf*)0)
  {
    for (int i = 0; i < node->leaf->obj_list.size(); i++)
    {
      if(node->leaf->obj_list[i]->intersect(ray, testHit) == true)
      {
        //Is an intersection
        return true;
      }
    }
    return false;
  }
  //If not a leaf
  //Check the childern which the ray interesects
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
  //If its a leaf node
  if (node->leaf != (OctreeLeaf*)0)
  {
    //Check all objects in leaf
    for (int i = 0; i < node->leaf->obj_list.size(); i++)
    {
      //Has the object been checked allready
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

  //Check the childern which the ray interesects
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
  //Check bounding box for objects which are inside, return true if there are
  Object *obj = obj_list;

  while (obj != (Object *)0)
  {
    if (obj->boundingBoxIntersect(boundingBox))
    {
      //Object found
      return true;
    }
    obj = obj->next();
  }
  //No objects
  return false;
}

std::vector<Object*> Octree::getObjects(AABoundingBox *boundingBox, Object *obj_list)
{
  std::vector<Object*> objects;

  Object *obj = obj_list;
  //For each objects build list of those inside the box
  while (obj != (Object *)0)
  {
    if (obj->boundingBoxIntersect(boundingBox))
    {
      //Object in box, add to list
      objects.push_back(obj);
    }
    obj = obj->next();
  }

  return objects;
}


OctreeNode *Octree::createTree(OctreeNode *parentNode, Object *obj_list, int limit)
{
  //If limit reaches define leaf node
  if (limit <= 0)
  {
    OctreeLeaf *leaf = new OctreeLeaf;
    parentNode->leaf = leaf;
    //Get list of objects inside
    leaf->obj_list = this->getObjects(parentNode->boundingBox, obj_list);
    return parentNode;
  }

  Vertex top = parentNode->boundingBox->topCorner;
  Vertex bottom = parentNode->boundingBox->bottomCorner;

  //Get side length, its a cube of all sides the same
  double sideLength = (top.x - bottom.x) / 2;

  int count = 0;
  //Create 8 child bounding boxes
  for (int i = 1; i < 3; i++)
  {
    double x = (top.x - bottom.x)/i + bottom.x;
    for (int j = 1; j < 3; j++)
    {
      double y = (top.y - bottom.y)/j + bottom.y;
      for (int k = 1; k < 3; k++)
      {
        double z = (top.z - bottom.z)/k + bottom.z;
        //Create new bounding box
        Vertex newTop;
        newTop.set(x, y, z, 1);
        Vertex newBottom;
        newBottom.set(x - sideLength, y - sideLength, z - sideLength, 1);

        AABoundingBox *childBox = new AABoundingBox(newTop, newBottom);
        OctreeNode *childNode = new OctreeNode;
        childNode->boundingBox = childBox;
        childNode->leaf = (OctreeLeaf*)0;
        //Check if there are objects inside
        if (this->containsObject(childBox, obj_list))
        {
          //If so, continue recursion
          this->createTree(childNode, obj_list, limit - 1);
        } else {
          //Nothing in this area, return
          childNode = (OctreeNode*)0;
        }

        parentNode->childern[count++] = childNode;
      }
    }
  }

  return parentNode;
}