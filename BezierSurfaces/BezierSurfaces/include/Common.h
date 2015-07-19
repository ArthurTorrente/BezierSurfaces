#ifndef _COMMON_H_
#define _COMMON_H_

#include "BSpline.h"
#include "BezierSurface.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

enum MODE
{
	MAIN,
	BSPLINES,
	BEZIERSURFACES
};

extern std::vector<BSpline*> bsplines;
extern std::vector<BezierSurface*> beziersurfaces;
extern MODE currentMode = MAIN;

#endif