/*
 * Light.h
 *
 *  Created on: Oct 24, 2015
 *      Author: Saylor
 */

#ifndef SRC_LIGHT_H_
#define SRC_LIGHT_H_

#include "Model.h"
#include <vector>

class Light {
public:
  Light(float r, float g, float b, float x, float y, float z, bool ambient);
  virtual ~Light();

  float red, green, blue, x, y, z;
  bool ambient;

private:
};

#endif /* SRC_LIGHT_H_ */
