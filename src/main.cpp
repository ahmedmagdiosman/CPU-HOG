//=======================================================================
// Copyright Ahmed Magdi Osman 2015.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*
 * main.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: Ahmed M. Osman
 */

#include <stdio.h>
#include <stdlib.h>
#include "CImg.h"
#include "Image3.hpp"
#include "Vec2.hpp"
#include  "HOG.hpp"
#include <sstream>
using namespace cimg_library;

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << "[IMAGE NAME]" << std::endl;

		return 1;
	}

	//Reads image
	CImg<unsigned char> srcImg(argv[1]);

	//Convert from CImg class to a simple handmade class.
	Image3 img = Image3(srcImg, true, RGBCHANNELS); // 'true' pads the image
	//Initialize HOG instance
	HOG hog = HOG(img);
	//Compute a sliding window HOG on the default image scale with default parameters of [Dalal 2005]
	hog.computeHOG();

	return 0;
}
