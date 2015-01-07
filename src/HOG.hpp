//=======================================================================
// Copyright Ahmed Magdi Osman 2015.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*
 * HOG.hpp
 *
 *  Created on: Oct 27, 2014
 *      Author: Ahmed M. Osman
 */

#ifndef HOG_HPP_
#define HOG_HPP_

#include <algorithm>
#include "Image3.hpp"
#include "Vec2.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include  "consts.h"

using namespace std;

typedef struct {
	float bin[NBINS * BLOCKSIZEX * BLOCKSIZEY] = { 0 };
} HOGDescriptor;

class HOG {

public:
	HOG(const Image3& _img);

	void computeHOG();

	~HOG();

private:
	Image3 img;
	Vec2 imgGradient;
	float lut[256]; 	//  Sqrt gamma compression LUT table
	HOGDescriptor* hogDsc; // Final vector for default config is 7 blocks*15 blocks* 2x2 cells*9 bins = 3780 elements
	unsigned int numWindowsX;
	unsigned int numWindowsY;

	void computeGradient();
	void computeHistogram(unsigned int _x, unsigned int _y);
	void normalizeBlock(unsigned int _windowX, unsigned int _windowY);
	void writeFeatureVector(unsigned int _windowX, unsigned int _windowY);
	void writeGradient();
};
#endif /* HOG_HPP_ */
