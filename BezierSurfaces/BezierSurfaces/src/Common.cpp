#include "Common.h"

#include "Render/Renderer.h"
#include "Objects/Scene.h"
#include "Cameras/PerspectiveCamera.h"

Renderer* r = new Renderer();
Scene* s = new Scene();
PerspectiveCamera* c = new PerspectiveCamera();

std::vector<BSpline*> bsplines;
BezierSurface* beziersurface;

MODE currentMode = MAIN;

uint CURRENT_CURVE_EDITED = -1;
uint CURRENT_VERTEX_EDITED = 0;

bool MIDDLE_BUTTON_PRESSED = false;