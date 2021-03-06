/*----------------------------------------------------------------------
  File:    texture.h
  Purpose: Stuff concerning textures
		   reads PPM files (only in P6 format, i.e., RGB binary)
----------------------------------------------------------------------*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <cmath>

class Texture {
public:
	int mWidth;
	int mHeight;
	unsigned char *mRGB; /* the size is 3*mWidth*mHeight */

	static inline float scale(float coord) {
		float nwrap;
		float ret = std::modf(coord, &nwrap);
		if (ret < 0.0) ret += 1.0;
		if (ret == 0.0 && (int)(nwrap) != 0) ret = 1.0;
		return ret;
	}
};

Texture *viReadPPM(char *filename);
int viWritePPM(char *filename, int width, int height, unsigned char *pixels);

#endif
