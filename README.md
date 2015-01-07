# CPU HOGv1.8

An CPU implementation of Histogram of Oriented Gradients [Dalal 2005] with the default parameters. The algorithm uses a sliding window on the image to generate the histograms. The feature vectors is saved under "CPU_features" and the gradient magnitude-and-angle calculation step is saved in "imgradient.txt", it's not pretty though :P

## Installation
* CImg is needed to read images from disk. It is already included.
* Make sure pthreads and X11 libraries are installed. This is needed for CImg to compile.
* Either run the included makefiles (in Release/Debug) or compile with gcc and link (-lpthread) and (-lX11).

## Usage
Run:
* ./Debug/HOGv1.8 [image name]

 or
* ./Release/HOGv1.8 [image name]

## Bugs
* Gaussian weights for the block edges are not implemented.

## Environment
* Compiled with gcc 4.8 in Debian Jessie (testing)

## License

Distributed under the MIT License.
See accompanying file LICENSE or copy at
http://opensource.org/licenses/MIT
