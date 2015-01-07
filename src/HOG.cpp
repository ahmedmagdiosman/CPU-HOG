//=======================================================================
// Copyright Ahmed Magdi Osman 2015.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*
 * HOG.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: Ahmed M. Osman
 */

#include "HOG.hpp"
#include "time.h"
HOG::HOG(const Image3& _img) :
img(_img), imgGradient(img.width() - 2 * PADTHICKNESS,
		img.height() - 2 * PADTHICKNESS)

{

	// Build LUT table
	for (int i = 0; i < 256; i++) {
		lut[i] = sqrtf(i);
	}

	numWindowsX = imgGradient.width() / WINDOWSTRIDEX - 1;
	numWindowsY = imgGradient.height() / WINDOWSTRIDEY - 1;
	hogDsc = new HOGDescriptor[numWindowsX * numWindowsY * NUMBLOCKSX
	                           * NUMBLOCKSY]; // total number of blocks. By default (7x15 blocks per window) * (number of windows in the image)

}

void HOG::computeGradient() {
	//		 Gradient computation -1 0 1 in x and y
	float2* imgGradientPtr;
	uchar3 *prevImgPtr = img.getConstPtr(1, 0), *imgPtr = img.getConstPtr(1, 1),
			*nextImgPtr;
	float gxR, gxG, gxB, gyR, gyG, gyB, mag, mR, mG, mB;
	int y = 0, x;

	for (; y < imgGradient.height(); y++) {

		imgGradientPtr = imgGradient.getPtr(0, y);		//row pointer
		nextImgPtr = img.getConstPtr(1, y + 2); // +1 for border. +1 for nextImgPtr.
		for (x = 0; x < imgGradient.width(); x++) {

			//			//x gradient
			gxR = lut[(int) imgPtr[x + 1].x] - lut[(int) imgPtr[x - 1].x];
			gxG = lut[(int) imgPtr[x + 1].y] - lut[(int) imgPtr[x - 1].y];
			gxB = lut[(int) imgPtr[x + 1].z] - lut[(int) imgPtr[x - 1].z];

			//			//y gradient
			gyR = lut[(int) nextImgPtr[x].x] - lut[(int) prevImgPtr[x].x];
			gyG = lut[(int) nextImgPtr[x].y] - lut[(int) prevImgPtr[x].y];
			gyB = lut[(int) nextImgPtr[x].z] - lut[(int) prevImgPtr[x].z];

			//magnitude
			mR = sqrtf(gxR * gxR + gyR * gyR);
			mG = sqrtf(gxG * gxG + gyG * gyG);
			mB = sqrtf(gxB * gxB + gyB * gyB);
			mag = max(max(mR, mG), mB);

			if (mag == mR) {
				imgGradientPtr[x].x = mR;
				imgGradientPtr[x].y = atan2f(fabsf(gyR), (gxR + EPSILON))
								* 180/ PI; //atan2(y,x) returns value in radians.
			} else if (mag == mG) {
				imgGradientPtr[x].x = mG;
				imgGradientPtr[x].y = atan2f(fabsf(gyG), (gxG + EPSILON))
								* 180/ PI;
			} else	//mag == mB
			{
				imgGradientPtr[x].x = mB;
				imgGradientPtr[x].y = atan2f(fabsf(gyB), (gxB + EPSILON))
								* 180/ PI;
			}

		}
		prevImgPtr = imgPtr;
		imgPtr = nextImgPtr;

	}

	//	for(i=0;i<imgGradient.height();i++)
	//		for (int q = 0; q<imgGradient.width();q++)
	//			printf("%f\t%f\n",imgGradient.get(q,i,0),imgGradient.get(q,i,1));
	return;
}

void HOG::computeHistogram(unsigned int _windowX, unsigned int _windowY) {
	// 		Orientation binning: magnitude added to orientation bin. Orientation = atan(y/x) INTERPOLATION
	float mag, angle, a1, a2, b1, b2, c1, c2;

	int blkIdx, blkPosX, blkPosY, cellLowCenX, cellLowCenY, cellHighCenX,
	cellHighCenY, posX, posY, lowBin, z1, z2, xy11, xy12, xy21, xy22;
	const int halfCellX = CELLSIZEX / 2;
	const int halfCellY = CELLSIZEY / 2;

	int bx = WINDOWSIZEX / NUMCELLSX, by = WINDOWSIZEY / NUMCELLSY, bz =
			ANGLERANGE / NBINS;
	const int halfBin = bz / 2.f;
	float2* imgGradientPtr;
	unsigned int windowPosX = _windowX * WINDOWSTRIDEX;
	unsigned int windowPosY = _windowY * WINDOWSTRIDEY;
	unsigned int wIdx = _windowY * numWindowsX + _windowX;
	const unsigned int NUMBLOCKS = NUMBLOCKSX * NUMBLOCKSY;
	int y, x, bX, bY;
	for (bY = 0; bY < NUMBLOCKSY; bY++) {

		for (bX = 0; bX < NUMBLOCKSX; bX++) {
			blkPosX = windowPosX + bX * BLOCKSTRIDEX;
			blkPosY = windowPosY + bY * BLOCKSTRIDEY;
			cellLowCenX = blkPosX + halfCellX;
			cellLowCenY = blkPosY + halfCellY;
			cellHighCenX = cellLowCenX + CELLSIZEX;
			cellHighCenY = cellLowCenY + CELLSIZEY;

			blkIdx = (wIdx) * NUMBLOCKS + bY * NUMBLOCKSX + bX;
			for (y = 0; y < BLOCKSIZEY * CELLSIZEY; y++) {
				imgGradientPtr = imgGradient.getConstPtr(blkPosX, blkPosY + y);
				for (x = 0; x < BLOCKSIZEX * CELLSIZEX; x++) {

					mag = imgGradientPtr[x].x;
					angle = imgGradientPtr[x].y;

					z1 = (angle) / (bz + EPSILON);
					lowBin = z1 * bz + halfBin;
					if (lowBin > angle) {
						lowBin -= bz;
					}

					//precompute weights
					posX = x + blkPosX;
					posY = y + blkPosY;
					a1 = posX > cellLowCenX ?
							(float) (posX - cellLowCenX) / bx : 0;
					b1 = posY > cellLowCenY ?
							(float) (posY - cellLowCenY) / by : 0;
					c1 = (angle - lowBin) / bz;
					a2 = (posX) < cellHighCenX ? 1 - a1 : 0;
					b2 = (posY) < cellHighCenY ? 1 - b1 : 0;
					c2 = 1 - c1;

					xy11 = 0;
					xy21 = NBINS * 1;
					xy12 = NBINS * 2;
					xy22 = NBINS * 3;

					lowBin = (lowBin % ANGLERANGE + ANGLERANGE) % ANGLERANGE; // to handle negative modulo e.g. -10%180 = 170
					z1 = lowBin / bz;
					z2 = (z1 + 1) % NBINS;
					// storing trilinear interpolation and binning

					hogDsc[blkIdx].bin[xy11 + z1] += mag * a2 * b2 * c2;
					hogDsc[blkIdx].bin[xy11 + z2] += mag * a2 * b2 * c1;
					hogDsc[blkIdx].bin[xy12 + z1] += mag * a2 * b1 * c2;
					hogDsc[blkIdx].bin[xy21 + z1] += mag * a1 * b2 * c2;
					hogDsc[blkIdx].bin[xy12 + z2] += mag * a2 * b1 * c1;
					hogDsc[blkIdx].bin[xy21 + z2] += mag * a1 * b2 * c1;
					hogDsc[blkIdx].bin[xy22 + z1] += mag * a1 * b1 * c2;
					hogDsc[blkIdx].bin[xy22 + z2] += mag * a1 * b1 * c1;

				}
			}
		}
	}
	return;
}

void HOG::normalizeBlock(unsigned int _windowX, unsigned int _windowY) {

	//		Normalization of blocks

	unsigned int wIdx = _windowY * numWindowsX + _windowX;
	const unsigned int NUMBLOCKS = NUMBLOCKSX * NUMBLOCKSY;

	float sumofsqr, newSumofSqr[NUMBLOCKSX * NUMBLOCKSY] = { 0 };
	unsigned int blkIdx, y, x, i;
	unsigned int blkSize = NBINS * BLOCKSIZEX * BLOCKSIZEY;
	//L2 norm with clipping
	for (y = 0, i = 0; y < NUMBLOCKSY; y++) {

		for (x = 0; x < NUMBLOCKSX; x++) {

			blkIdx = (wIdx) * NUMBLOCKS + y * NUMBLOCKSX + x;
			sumofsqr = 0;
			//compute sum of squares

			for (i = 0; i < blkSize; i++) {
				sumofsqr += hogDsc[blkIdx].bin[i] * hogDsc[blkIdx].bin[i];
			}

			//normalize block

			for (i = 0; i < blkSize; i++) {
				hogDsc[blkIdx].bin[i] = min(
						(hogDsc[blkIdx].bin[i]
						                    / sqrtf(sumofsqr + EPSILON * EPSILON)),
						                    L2THRESHOLD);

				newSumofSqr[y * NUMBLOCKSX + x] += hogDsc[blkIdx].bin[i]
				                                                      * hogDsc[blkIdx].bin[i];

			}

			//L2-Hys re-normalizing after clipping
			for (i = 0; i < blkSize; i++) {
				hogDsc[blkIdx].bin[i] = hogDsc[blkIdx].bin[i]
				                                           / sqrtf(
				                                        		   newSumofSqr[y * NUMBLOCKSX + x]
				                                        		               + EPSILON * EPSILON);

			}

		}
	}

}

void HOG::writeFeatureVector(unsigned int _windowX, unsigned int _windowY) {
	struct stat st;
	if (stat("CPU_features", &st) == -1) {
		mkdir("CPU_features", 0777);
	}
	std::ofstream outfile;
	std::stringstream ss1, ss2;
	ss1 << _windowX;
	ss2 << _windowY;
	unsigned int windowIdx = (_windowY * numWindowsX + _windowX) * NUMBLOCKSX
			* NUMBLOCKSY;
	std::string name = "CPU_features/vector_" + ss1.str() + "_" + ss2.str()
					+ ".txt";
	outfile.open(name.c_str());
	for (unsigned int i = 0; i < (NUMBLOCKSX * NUMBLOCKSY); i++) {
#pragma unroll 36
		for (unsigned int j = 0; j < 36; j++) {
			outfile << hogDsc[windowIdx + i].bin[j] << "\n";
		}

		//outfile << "\n==========BLOCK END ==========\n";
	}
	outfile.close();
}

void HOG::writeGradient() {

	std::ofstream outfile;

	std::string name = "imgradient.txt";
	outfile.open(name.c_str());
	float2* grdPtr;
	for (unsigned int y = 0; y < imgGradient.height(); y++) {
		grdPtr = imgGradient.getConstPtr(0, y);
		for (unsigned int x = 0; x < imgGradient.width(); x++) {
			outfile << "M: " << grdPtr[x].x << "A: " << grdPtr[x].y << "\t";
		}
		outfile << "\n";
	}
	outfile.close();
}
void HOG::computeHOG() {
	clock_t start = clock();
	computeGradient();
	clock_t end = clock();
	float milliseconds = (end - start) / (CLOCKS_PER_SEC / 1000.0);
	float histTime = 0, normTime = 0;
	printf("Time for gradient: %f ms\n", milliseconds);
	writeGradient();
	unsigned int x, y;
	for (y = 0; y < numWindowsY; y++) {
		for (x = 0; x < numWindowsX; x++) {
			start = clock();
			computeHistogram(x, y);
			end = clock();
			histTime += (end - start) / (CLOCKS_PER_SEC / 1000.0);
			start = clock();
			normalizeBlock(x, y);
			end = clock();
			normTime += (end - start) / (CLOCKS_PER_SEC / 1000.0);

			writeFeatureVector(x, y);

		}
	}
	printf("Time for histogram computation: %f ms\n", histTime);
	printf("Time for block normalization: %f ms\n", normTime);

}
HOG::~HOG() {

	delete[] hogDsc;
	hogDsc = 0;
}
