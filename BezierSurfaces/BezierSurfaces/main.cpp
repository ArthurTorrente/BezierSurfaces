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

	GLenum error = glewInit();
	if (error != GL_NO_ERROR)
	{
		// Erreur
	}

	// Initialisation des différents éléments
	r.setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);
	r.setClearColor(Vector3(0.5, 0.5, 0.5));

	c.setPosition(Vector3(0, 0, -30));

	ShaderLoader::loadShader("simple", "simple.vs", "simple.fs");

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