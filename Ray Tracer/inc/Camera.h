/*
 * Camera.h
 *
 *  Created on: Oct 3, 2015
 *      Author: Saylor Berman
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

#include <vector>
#include <string>
#include "Model.h"
#include "Sphere.h"
#include "Light.h"
#include "Material.h"

class Camera {
public:
	Camera(std::string& filename, std::vector<Model>& models, std::vector<Light>& lights, std::vector<Material>& materials);
	virtual ~Camera();
	std::vector<float> solveLinearSystem(int j, std::vector<float>& L, std::vector<float>& U);
	std::vector<float> throwRay(std::vector<float>& L, std::vector<float> U);
	std::vector<float> lightIntersect(std::vector<float> S, int faceIndex, int modelIndex, std::vector<float> V);

	std::vector<float> focalPoint, lookAtPoint, VUP, n, u, v;
	std::vector<int> boundingBox;
	float focalLength;
	int reflectCount, refractCount;

private:
	std::vector<float> Av, Bv, Cv;
	std::vector<std::vector<float> > triangles, equation, vertices;
	std::vector<std::vector<std::vector<float> > > modelVerticesList, modelFaceList;
	std::vector<std::vector<Sphere> > modelSpheres;
	std::vector<Model> models;
	std::vector<Light> lights;
	std::vector<Material> materials;
	std::vector<Sphere> spheres;
};

#endif /* SRC_CAMERA_H_ */
