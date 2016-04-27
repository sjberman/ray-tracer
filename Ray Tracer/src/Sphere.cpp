/*
 * Sphere.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: Saylor Berman
 */

#include "../inc/Sphere.h"
#include "../utils/LinearAlgebra.h"
#include <iostream>

Sphere::Sphere(std::vector<float>& center, float radius){
	this->centroid = center;
	this->radius = radius;
	//parent = 0;
}

Sphere::~Sphere() {

}

//tests if ray intersects sphere
bool Sphere::sphereIntersect(std::vector<float>& E, std::vector<float>& U){
	std::vector<float> tmp = subtractVector(centroid, E);
	//bool var = false;
	float v = dotProduct(tmp, U);
	float c = sqrt(pow(tmp[0], 2) + pow(tmp[1], 2) + pow(tmp[2], 2));
	float d = sqrt(pow(this->radius, 2) - (pow(c, 2) - pow(v, 2)));
	/*if(d > 0 && this->parent > 0){
		var = this->first->sphereIntersect(E, U);
		if(var == false){
			var = this->second->sphereIntersect(E, U);
		}
		return var;
	}

	if(d > 0 && this->parent == 0){
		return true;
	}*/

	if(d > 0)
		return true;

	else
		return false;
}

