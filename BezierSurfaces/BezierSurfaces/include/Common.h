#ifndef _COMMON_H_
#define _COMMON_H_

#include "BSpline.h"
#include "BezierSurface.h"
#include "GlutWindow.h"

#include "Render/Renderer.h"
#include "Objects/Scene.h"
#include "Cameras/PerspectiveCamera.h"
#include "Materials/ShaderLoader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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

/*
extern void menuExtru(int);
extern void subMenuExtru(int);
extern void keyboardExtru(unsigned char, int, int);
*/

// ** Fonctions décrites dans Interface3D.cpp **
extern void reshape3D(int, int);
extern void display3D();
extern void mouse3D(int, int, int, int);
extern void motion3D(int, int);
/*
extern void menuExtru(int);
extern void subMenuExtru(int);
extern void keyboardExtru(unsigned char, int, int);
*/

extern Renderer r;
extern Scene s;
extern PerspectiveCamera c;

extern std::vector<BSpline*> bsplines;
extern std::vector<BezierSurface*> beziersurfaces;
extern MODE currentMode;

extern uint CURRENT_CURVE_EDITED;
extern uint CURRENT_VERTEX_EDITED;

extern bool MIDDLE_BUTTON_PRESSED;

#endif