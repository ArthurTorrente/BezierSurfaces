#include "include_glut.h"
#include "Common.h"

GlutWindow* bsplineWindow = NULL;
GlutWindow* spaceWindow = NULL;

void reshapeBSpline(int, int);
void displayBSpline();
void mouseBSpline(int, int, int, int);
void motionBSpline(int, int);
void keyboardBSpline(unsigned char, int, int);
void initMenuBSpline();
void refreshUIBSpline();
void selectModeBSpline(int mode);
void selectBSplinesBSpline(int selection);
void editBSplinesBSpline(int selection);
void selectColorBSpline(int selection);
void selectExtrudeBSpline(int selection);

static const float g_cubeVertices[] = {
	-1.f, -1.f, 1.0f,		// 0
	1.f, -1.f, 1.0f,  		// 1	
	1.f, 1.f, 1.0f,			// 2
	-1.f, 1.f, 1.0f,		// 3	
	-1.f, -1.f, -1.0f,		// 4
	1.f, -1.f, -1.0f,		// 5
	1.f, 1.f, -1.0f,		// 6
	-1.f, 1.f, -1.0f		// 7
};

static const unsigned int g_cubeIndices[] = {
	0, 1, 2, // avant
	2, 3, 0,
	3, 2, 6, // haut
	6, 7, 3,
	7, 6, 5, // arriere
	5, 4, 7,
	1, 5, 6, // droite
	6, 2, 1,
	4, 0, 3, // gauche
	3, 7, 4,
	4, 5, 1, // bas
	1, 0, 4
};


int main(int argc, char** argv)
{
	// Initialisation de la fenêtre principale
	glutInit(&argc, argv);

	bsplineWindow = new GlutWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 10, 10, "B-Splines and Bezier Surfaces", GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
	bsplineWindow->enable();
	bsplineWindow->display(displayBSpline);
	bsplineWindow->reshape(reshapeBSpline);
	bsplineWindow->mouse(mouseBSpline);
	bsplineWindow->motion(motionBSpline);
	bsplineWindow->keyboardFunc(keyboardBSpline);

	// Création du menu
	initMenuBSpline();

	// Boucle de rendu
	glutMainLoop();

	// Destruction
	if (bsplineWindow != NULL) delete bsplineWindow;
	if (spaceWindow != NULL) delete spaceWindow;

	return 0;



	/*
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("B-Splines and Bezier Surfaces");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLenum error = glewInit();
	if (error != GL_NO_ERROR)
	{
		return 0;
	}

	initMenu();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	//glutSpecialFunc(specialKey);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();
	return 0;
	*/
}