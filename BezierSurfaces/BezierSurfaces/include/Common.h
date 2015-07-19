#ifndef _COMMON_H_
#define _COMMON_H_

#include "BSpline.h"
#include "BezierSurface.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

enum MODE
{
	MAIN,
	BSPLINES_MAIN,
	BSPLINES_EDITION,
	BEZIERSURFACES
};

extern std::vector<BSpline*> bsplines;
extern std::vector<BezierSurface*> beziersurfaces;
extern MODE currentMode;

extern uint CURRENT_CURVE_EDITED;
extern uint CURRENT_VERTEX_EDITED;

extern bool MIDDLE_BUTTON_PRESSED;

#endif