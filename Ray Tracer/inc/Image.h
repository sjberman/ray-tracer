/*
 * Image.h
 *
 *  Created on: Oct 3, 2015
 *      Author: Saylor Berman
 */

#ifndef SRC_IMAGE_H_
#define SRC_IMAGE_H_

#include "Camera.h"
#include <vector>

class Image {
public:
	Image(const Camera& camera, int rows, int cols);
	virtual ~Image();
	void generate();
	std::vector<std::vector<std::vector<float> > > getPixels() const;

private:
	void scalePixels();

	Camera camera;
	std::vector<std::vector<std::vector<float> > > pixels;
};

#endif /* SRC_IMAGE_H_ */
