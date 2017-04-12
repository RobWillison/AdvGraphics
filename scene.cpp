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

Colour Scene::reflectedRay()
{

}

Colour Scene::refractedRay(float closestN, Vertex &position, Ray &ray, Vector &normal, Vector &view, int level)
{
  normal.normalise();
  ray.D.normalise();
  float test = normal.dot(ray.D);

  Vector insident;
  insident.x = -ray.D.x;
  insident.y = -ray.D.y;
  insident.z = -ray.D.z;

  if (test > 0.0)
  {
    //Normal is pointing the wrong way
    normal.x = -normal.x;
    normal.y = -normal.y;
    normal.z = -normal.z;
  }
  //printf("Sphere Normal %f %f %f\n", normal.x, normal.y, normal.z);

  Colour col;
  col.clear();
  Ray T;

  float nRatio;

  if (ray.n == 1)
  {
    nRatio = closestN / ray.n;
    T.n = closestN;

  } else {
    nRatio = 1 / ray.n;
    T.n = 1.0f;
  }

  //printf("%f\n", nRatio);
  float thetaI = normal.dot(view);
  //printf("Theta I %f\n", acos(thetaI));
  float thetaT = 1.0 - (1.0 / pow(nRatio, 2)) * (1.0 - pow(thetaI, 2));
  //Check total Internal reflection

  if (thetaT < 0.0){
    return col;
  }

  thetaT = sqrt(thetaT);
  //printf("Theta T %f\n", acos(thetaT));
  T.D.x = 1/nRatio * - view.x - (thetaT - (1/nRatio) * thetaI) * normal.x;
  T.D.y = 1/nRatio * - view.y - (thetaT - (1/nRatio) * thetaI) * normal.y;
  T.D.z = 1/nRatio * - view.z + ((1/nRatio) * thetaT - thetaT) * normal.z;
  //printf("Test Test Test %f\n", T.D.z);
  T.D.normalise();

  T.P = position;
  T.P.x = T.P.x + 0.1 * T.D.x;
  T.P.y = T.P.y + 0.1 * T.D.y;
  T.P.z = T.P.z + 0.1 * T.D.z;
  //printf("Refracted Ray %f %f %f\n", T.D.x, T.D.y, T.D.z);
  col = this->raytrace(T, level - 1);
  //printf("Level %d Color %f %f %f\n", level, col.red, col.blue, col.green);

  return col;
}

int Scene::isShadowed(Vector xldir, Vertex position)
{
  Ray shadowRay;
  shadowRay.D.x = xldir.x;
  shadowRay.D.z = xldir.z;
  shadowRay.D.y = xldir.y;

  shadowRay.P = position;
  shadowRay.P.x = shadowRay.P.x + 0.01 * shadowRay.D.x;
  shadowRay.P.y = shadowRay.P.y + 0.01 * shadowRay.D.y;
  shadowRay.P.z = shadowRay.P.z + 0.01 * shadowRay.D.z;

  return tree->testForShadow(shadowRay);

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

  // while (obj != (Object *)0)
  // {
  //   if(obj->intersect(ray, &hit) == true)
  //   {
  //     if (hit.t < t)
  //     {
  //       closest = hit.obj;
  //       t = hit.t;
  //       normal = hit.n;
  //       position = hit.p;
  //     }
  //   }
  //
  //   obj = obj->next();
  // }

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

      // calculate specular component here
      Vector reflection;

      reflection.x = xldir.x - 2.0 * (xldir.dot(normal)) * normal.x;
      reflection.y = xldir.y - 2.0 * (xldir.dot(normal)) * normal.y;
      reflection.z = xldir.z - 2.0 * (xldir.dot(normal)) * normal.z;

      reflection.normalise();

      Vector view;
      Vertex cameraPos = camera->getPosition();
      view.x = cameraPos.x - position.x;
      view.y = cameraPos.y - position.y;
      view.z = cameraPos.z - position.z;

      view.normalise();
      double reflectionDiff = reflection.dot(view);

      Ray viewReflection;
      viewReflection.P = position;

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
      float objectN = closest->obj_mat->n;

      Colour refractedColour = this->refractedRay(objectN, position, ray, normal, view, level);

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