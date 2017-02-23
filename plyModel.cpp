// Sphere object

#include <stdio.h>
#include <fstream>
#include <string>

#include "plyModel.h"

PlyModel::PlyModel(char fileName[])
{
  std::ifstream file(fileName);
  std::string str;

  int vertexNum;
  int faceNum;

  while (std::getline(file, str))
  {

    if (str.substr(0, 15) == "element vertex")
    {
      vertexNum = std::stoi(str.substr(15));
    }

    if (str.substr(0, 13) == "element face")
    {

    }

  }
}
