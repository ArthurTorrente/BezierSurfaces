#include "Common.h"

std::vector<BSpline*> bsplines;
extern std::vector<BezierSurface*> beziersurfaces;

MODE currentMode = MAIN;

uint CURRENT_CURVE_EDITED = -1;
uint CURRENT_VERTEX_EDITED = 0;

bool MIDDLE_BUTTON_PRESSED = false;