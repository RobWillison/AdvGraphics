#include <math.h>

#include "scene.h"
#include <stdio.h>
#include "camera.h"
#include <vector>
#include <algorithm>

Scene::Scene()
{
  scache = 0;
  obj_list = (Object *)0;
  light_list = (Light *)0;
  count = 0;
}

void Scene::createOctree(Vertex top, Vertex bottom)
{
  tree = new Octree();
  tree->build(obj_list, top, bottom);
}

void Scene::setCamera(Camera &cam)
{
  camera = &cam;
}

Object * Scene::getObjectList()
{
  return obj_list;
}

void Scene::addObject(Object &obj)
{
	// add object to list of objects in scene
  obj.link(obj_list);
  obj_list = &obj;
}

void Scene::addLight(Light &lt)
{
	// add light to list of objects in scene
  lt.link(light_list);
  light_list = &lt;
}

float Scene::calculateFresnelKr(Object *closest, Ray &refractedRay, Ray &viewingRay, Vector &normal)
{

  float test = normal.dot(viewingRay.D);
  float nRatio;
  Vector adjustedNorm;

  if (test > 0.0)
  {
    adjustedNorm.x = -normal.x;
    adjustedNorm.y = -normal.y;
    adjustedNorm.z = -normal.z;
    //Normal is pointing the wrong way, leaving object
    nRatio = 1 / closest->obj_mat->n;
  } else {
    adjustedNorm.x = normal.x;
    adjustedNorm.y = normal.y;
    adjustedNorm.z = normal.z;
    //Entering object
    nRatio = closest->obj_mat->n / viewingRay.n;
  }

  Vector insident;
  insident.x = -viewingRay.D.x;
  insident.y = -viewingRay.D.y;
  insident.z = -viewingRay.D.z;

  float theataI = adjustedNorm.dot(insident);
  float theataT = sqrt(pow(adjustedNorm.dot(refractedRay.D), 2));

  float rPar = (nRatio * theataI - theataT) / (nRatio * theataI + theataT);
  float rPer = (theataI - nRatio * theataT) / (theataI + nRatio * theataT);

  float kr = 0.5 * (pow(rPar, 2) + pow(rPer, 2));

  return kr;
}

Ray Scene::calculateRefractedRay(Object *closest, Vertex &position, Ray &ray, Vector &normal, Vector &view)
{

  normal.normalise();
  ray.D.normalise();
  ray.D.normalise();
  Vector adjustedNorm;
  float test = normal.dot(ray.D);

  Ray T;
  float nRatio;

  if (test > 0.0)
  {
    adjustedNorm.x = -normal.x;
    adjustedNorm.y = -normal.y;
    adjustedNorm.z = -normal.z;
    //Normal is pointing the wrong way, leaving object
    nRatio = 1 / closest->obj_mat->n;
    T.n = 1.0f;
  } else {
    adjustedNorm.x = normal.x;
    adjustedNorm.y = normal.y;
    adjustedNorm.z = normal.z;
    //Entering object
    nRatio = closest->obj_mat->n / ray.n;
    T.n = closest->obj_mat->n;
  }

  float thetaI = normal.dot(ray.D);
  //printf("Theta I %f\n", acos(thetaI));
  float thetaT = 1.0 - (1.0 / pow(nRatio, 2)) * (1.0 - pow(thetaI, 2));

  //Check total Internal reflection
  if (thetaT < 0.0){
    T.D.x = 0;
    T.D.y = 0;
    T.D.z = 0;
    return T;
  }

  thetaT = sqrt(thetaT);

  T.D.x = 1/nRatio * ray.D.x - (thetaT - (1/nRatio) * thetaI) * adjustedNorm.x;
  T.D.y = 1/nRatio * ray.D.y - (thetaT - (1/nRatio) * thetaI) * adjustedNorm.y;
  T.D.z = 1/nRatio * ray.D.z - (thetaT - (1/nRatio) * thetaI) * adjustedNorm.z;

  T.D.normalise();
  //Move little way off
  T.P = position;
  T.P.x = T.P.x + 0.001 * T.D.x;
  T.P.y = T.P.y + 0.001 * T.D.y;
  T.P.z = T.P.z + 0.001 * T.D.z;

  T.number = clock();
  return T;
}

int Scene::isShadowed(Vector xldir, Vertex position)
{
  Ray shadowRay;
  shadowRay.number = clock();
  shadowRay.D.x = xldir.x;
  shadowRay.D.z = xldir.z;
  shadowRay.D.y = xldir.y;
  //Move little way off
  shadowRay.P = position;
  shadowRay.P.x = shadowRay.P.x + 0.01 * shadowRay.D.x;
  shadowRay.P.y = shadowRay.P.y + 0.01 * shadowRay.D.y;
  shadowRay.P.z = shadowRay.P.z + 0.01 * shadowRay.D.z;

  Hit objHit;
  return tree->testForShadow(shadowRay);

  return 0;

}

Colour Scene::raytrace(Ray &ray, int level)
{

  float ta,t;
  Colour col;
  Object *obj;
  Object *closest;
  Light *lt;
  Hit   hit;
  Vertex position;
  Vector normal;


  if (level == 0)
  {
    col.clear();
    return col; // stop if recursed deep enough
  }

  // TEST EACH OBJECT AGAINST RAY, FIND THE CLOSEST

  t = 1000000000.0; // a long way aways
  closest = (Object *)0;
  obj = obj_list;
  hit.t = 1000000000.0;

  tree->findObjects(ray, &hit);

  if (hit.t < 1000000000.0)
  {
    closest = hit.obj;
    t = hit.t;
    normal = hit.n;
    position = hit.p;
  }

  col.clear();

  if (closest != (Object *)0)
  {
    lt = light_list;

    Colour ocol;

    Colour ka = closest->obj_mat->ka;
    Colour kd = closest->obj_mat->kd;
    Colour ks = closest->obj_mat->ks;
    Colour kr = closest->obj_mat->kr;
    Colour kt = closest->obj_mat->kt;
    Ray sray;

    while (lt != (Light *)0)
    {
      Vector ldir;
      Vector xldir;
      Colour lcol;

      // calculate diffuse component
      lt->getLightProperties(position, &xldir, &lcol);

      xldir.normalise();

      int shadow = this->isShadowed(xldir, position);

      float dlc = xldir.dot(normal);

      if (dlc < 0.0)
      {
	       dlc = 0.0;
      }

      // calculate reflection
      Vector reflection;

      reflection.x = xldir.x - 2.0 * (xldir.dot(normal)) * normal.x;
      reflection.y = xldir.y - 2.0 * (xldir.dot(normal)) * normal.y;
      reflection.z = xldir.z - 2.0 * (xldir.dot(normal)) * normal.z;

      reflection.normalise();

      Vector view;
      view.x = - ray.D.x;
      view.y = - ray.D.y;
      view.z = - ray.D.z;

      view.normalise();
      double reflectionDiff = reflection.dot(view);

      Ray viewReflection;
      viewReflection.P = position;
      viewReflection.number = clock();
      viewReflection.D.x = - (view.x - 2.0 * (view.dot(normal)) * normal.x);
      viewReflection.D.y = - (view.y - 2.0 * (view.dot(normal)) * normal.y);
      viewReflection.D.z = - (view.z - 2.0 * (view.dot(normal)) * normal.z);

      viewReflection.P.x = viewReflection.P.x + 0.1 * viewReflection.D.x;
      viewReflection.P.y = viewReflection.P.y + 0.1 * viewReflection.D.y;
      viewReflection.P.z = viewReflection.P.z + 0.1 * viewReflection.D.z;
      viewReflection.n = 1.0f;

      Colour reflectedColour = this->raytrace(viewReflection, level - 1);

      float slc = 0.0;

      if (reflectionDiff <= -0.0)
      {
        slc = pow(reflectionDiff, 20);
      }
      //refraction
      Ray refractedRay = this->calculateRefractedRay(closest, position, ray, normal, view);

      Colour refractedColour;
      if (refractedRay.D.length() == 0)
      {
        refractedColour.clear();
      } else {
        refractedColour = this->raytrace(refractedRay, level - 1);
      }
      
      //If not using simple kr kt
      if (kr.red > 1.0f)
      {
        float krValue = this->calculateFresnelKr(closest, refractedRay, ray, normal);
        float ktValue = 1 - krValue;

        kr.red = krValue;
        kr.blue = krValue;
        kr.green = krValue;
        kt.red = ktValue;
        kt.green = ktValue;
        kt.blue = ktValue;
      }

      if (shadow) {
        dlc = 0.0;
        slc = 0.0;
      }

      // combine components
      col.red += ka.red + lcol.red*(dlc * kd.red + slc * ks.red) + (kr.red * reflectedColour.red) + (kt.red * refractedColour.red);
      col.green += ka.green + lcol.green*(dlc * kd.green + slc * ks.green) + (kr.green * reflectedColour.green) + (kt.green * refractedColour.green);
      col.blue += ka.blue + lcol.blue*(dlc * kd.blue + slc * ks.blue) + (kr.blue * reflectedColour.blue) + (kt.blue * refractedColour.blue);

      lt = lt->next(); // next light
    }
  }

  return col;
}