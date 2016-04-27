/*
 * Sphere.h
 *
 *  Created on: Oct 21, 2015
 *      Author: Saylor Berman
 */

#ifndef SRC_SPHERE_H_
#define SRC_SPHERE_H_

#include <vector>

class Sphere {
public:
	Sphere(std::vector<float>& center, float radius);
	virtual ~Sphere();
	bool sphereIntersect(std::vector<float>& E, std::vector<float>& U);

	//Sphere* first;
	//Sphere* second;
	//int parent;

private:
	std::vector<float> centroid;
	float radius;
};

#endif /* SRC_SPHERE_H_ */
