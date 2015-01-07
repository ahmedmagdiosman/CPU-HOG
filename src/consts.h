//=======================================================================
// Copyright Ahmed Magdi Osman 2015.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*
 * consts.h
 *
 *  Created on: Nov 2, 2014
 *      Author: Ahmed M. Osman
 */

#ifndef CONSTS_H_
#define CONSTS_H_

#define PI 3.14159265
const int PADTHICKNESS = 1;
const float EPSILON = 0.000001;
const unsigned int RGBCHANNELS = 3;
const int DRVCHANNELS = 2;
const unsigned int NBINS = 9;
const unsigned int ANGLERANGE = 180;
const unsigned int CELLSIZEX = 8;
const unsigned int CELLSIZEY = 8;
const unsigned int BLOCKSIZEX = 2;
const unsigned int BLOCKSIZEY = 2;
const unsigned int BLOCKSTRIDEX = CELLSIZEX;
const unsigned int BLOCKSTRIDEY = CELLSIZEY;
const unsigned int WINDOWSIZEX = 64;
const unsigned int WINDOWSIZEY = 128;
const unsigned int WINDOWSTRIDEX = WINDOWSIZEX / 2;
const unsigned int WINDOWSTRIDEY = WINDOWSIZEY / 2;
const unsigned int NUMCELLSX = WINDOWSIZEX / CELLSIZEX;
const unsigned int NUMCELLSY = WINDOWSIZEY / CELLSIZEY;
const unsigned int NUMBLOCKSX = NUMCELLSX - 1;
const unsigned int NUMBLOCKSY = NUMCELLSY - 1;
const unsigned int NUMHIST = WINDOWSIZEX / CELLSIZEX * WINDOWSIZEY / CELLSIZEY;
const float L2THRESHOLD = 0.2;
static const int BORDERTHICKNESS = 1;

#endif /* CONSTS_H_ */
