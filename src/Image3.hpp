//=======================================================================
// Copyright Ahmed Magdi Osman 2015.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*
 * Image3.hpp
 *
 *  Created on: Oct 16, 2014
 *      Author: Ahmed M. Osman
 */

#ifndef IMAGE3_HPP_
#define IMAGE3_HPP_

#include "CImg.h"
#include  "consts.h"
#include  "types.h"
using namespace cimg_library;

class Image3 {
public:

	inline int width() const;
	inline int height() const;
	inline size_t size() const;
	inline uchar3* getConstPtr() const;
	inline uchar3* getPtr();
	inline uchar3* getPtr(unsigned int x, unsigned int y);
	inline uchar3* getConstPtr(unsigned int x, unsigned int y) const;
	inline uchar3 get(unsigned int x, unsigned int y) const;
	//	inline float getROI(int x, int y, int c);
	Image3(const Image3& _img);
	Image3(CImg<unsigned char> _img, bool border, int numchannels = 3);
	Image3(int width, int height);
	Image3();
	~Image3();

private:
	uchar3* data;
	int imWidth;
	int imHeight;
	size_t imSize;

};

inline uchar3 Image3::get(unsigned int x, unsigned int y) const {
	//TODO check for out of bounds without overhead
	// c: channel -> R,G,B = 0,1,2 respectively
	//WARNING: IF BORDER IS ADDED SHIFT both x and y by 1
	return data[y * imWidth + x];
}

//inline float Image3::getROI(int x, int y, int c)
//{
//return this->data[]
//}

inline int Image3::width() const {
	return imWidth;
}
inline int Image3::height() const {
	return imHeight;
}
inline size_t Image3::size() const {
	return imSize;
}

inline uchar3* Image3::getConstPtr() const {
	return data;
}

inline uchar3* Image3::getPtr() {
	return data;
}

inline uchar3* Image3::getPtr(unsigned int x, unsigned int y) {
	return &data[y * imWidth + x];
}
inline uchar3* Image3::getConstPtr(unsigned int x, unsigned int y) const {
	return &data[y * imWidth + x];
}

#endif /* IMAGE3_HPP_ */
