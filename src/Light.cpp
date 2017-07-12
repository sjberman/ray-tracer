/*
 * Light.cpp
 *
 *  Created on: Oct 24, 2015
 *      Author: Saylor
 */

#include "../inc/Light.h"

// normal light
Light::Light(float r, float g, float b, float x, float y, float z,
             bool ambient) {
  red = r;
  green = g;
  blue = b;
  this->x = x;
  this->y = y;
  this->z = z;
  this->ambient = ambient;
}

Light::~Light() {}
