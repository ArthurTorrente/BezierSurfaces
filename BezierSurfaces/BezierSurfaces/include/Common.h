#ifndef _COMMON_H_
#define _COMMON_H_

#include "GlutWindow.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class BezierSurface;
class BSpline;
class Renderer;
class Scene;
class PerspectiveCamera;

enum MODE
{
	MAIN,
	BSPLINES_MAIN,
	BSPLINES_EDITION,
	BEZIERSURFACES
};

extern GlutWindow* bsplineWindow;
extern GlutWindow* spaceWindow;

// ** Fonctions décrites dans InterfaceBSpline.cpp **
extern void reshapeBSpline(int, int);
extern void displayBSpline();
extern void mouseBSpline(int, int, int, int);
extern void motionBSpline(int, int);
extern void keyboardBSpline(unsigned char, int, int);
extern void initMenuBSpline();
extern void refreshUIBSpline();
extern void selectModeBSpline(int);
extern void selectBSplinesBSpline(int);
extern void editBSplinesBSpline(int);
extern void selectColorBSpline(int);
extern void selectExtrudeBSpline(int);

// ** Fonctions décrites dans Interface3D.cpp **
extern void reshape3D(int, int);
extern void display3D();
extern void mouse3D(int, int, int, int);
extern void motion3D(int, int);
extern void keyboard3D(unsigned char, int, int);
extern void wheel3D(int, int, int, int);
extern void initMenu3D();
extern void selectMode3D(int);

extern Renderer* r;
extern Scene* s;
extern PerspectiveCamera* c;

extern std::vector<BSpline*> bsplines;
extern BezierSurface* beziersurface;
extern MODE currentMode;

extern uint CURRENT_CURVE_EDITED;
extern uint CURRENT_VERTEX_EDITED;

extern bool MIDDLE_BUTTON_PRESSED;

#endif