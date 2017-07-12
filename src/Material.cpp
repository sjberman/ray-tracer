/*
 * Material.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: Saylor Berman
 */

#include "../inc/Material.h"

Material::Material(int objNum, int firstFace, int lastFace, float diff1,
                   float diff2, float diff3, float k, float alpha,
                   float opacity, std::vector<Model> &models) {
  this->objNum = objNum;
  this->firstFace = ((models[objNum].fcount - 2) * firstFace);
  this->lastFace = ((models[objNum].fcount - 2) * lastFace);
  this->k = k;
  this->alpha = alpha;
  this->opacity = opacity;

  // set up diffuse matrix
  diffuseMatrix.resize(3);
  for (int i = 0; i < 3; i++)
    diffuseMatrix[i].resize(3);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++)
      diffuseMatrix[i][j] = 0;
  }
  diffuseMatrix[0][0] = diff1;
  diffuseMatrix[1][1] = diff2;
  diffuseMatrix[2][2] = diff3;
}

Material::~Material() {}
