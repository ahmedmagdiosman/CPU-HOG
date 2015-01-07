//=======================================================================
// Copyright Ahmed Magdi Osman 2015.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*
 * Vec2.hpp
 *
 *  Created on: Oct 16, 2014
 *      Author: Ahmed Magdi Osman
 */

#ifndef VEC2_HPP_
#define VEC2_HPP_

#include "CImg.h"
#include "types.h"
using namespace cimg_library;

class Vec2 {
public:

	inline int width() const;
	inline int height() const;
	inline size_t size() const;
	inline float2* getConstPtr() const;
	inline float2* getPtr();
	inline float2* getPtr(unsigned int x, unsigned int y);
	inline float2* getConstPtr(unsigned int x, unsigned int y) const;
	inline float2 get(unsigned int x, unsigned int y) const;
	Vec2(const Vec2& _vec);
	//Vec2(CImg<float> _img, bool border, int numchannels = 3);
	Vec2(int width, int height);
	Vec2();
	~Vec2();

private:
	float2* data;
	int imWidth;
	int imHeight;
	size_t imSize;

};

inline float2 Vec2::get(unsigned int x, unsigned int y) const {
	//TODO check for out of bounds without overhead
	// c: channel -> R,G,B = 0,1,2 respectively
	//WARNING: IF BORDER IS ADDED SHIFT both x and y by 1
	return data[y * imWidth + x];
}

inline int Vec2::width() const {
	return imWidth;
}
inline int Vec2::height() const {
	return imHeight;
}
inline size_t Vec2::size() const {
	return imSize;
}

inline float2* Vec2::getConstPtr() const {
	return data;
}

inline float2* Vec2::getPtr() {
	return data;
}

inline float2* Vec2::getPtr(unsigned int x, unsigned int y) {
	return &data[y * imWidth + x];
}
inline float2* Vec2::getConstPtr(unsigned int x, unsigned int y) const {
	return &data[y * imWidth + x];
}

#endif /* VEC2_HPP_ */
