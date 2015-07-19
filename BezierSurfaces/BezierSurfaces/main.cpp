#include "BSpline.h"
#include "BezierSurface.h"

#define GLEW_STATIC 1
#include "GL\glew.h"

#ifdef _WIN32
#	include <Windows.h>
#	define FREEGLUT_LIB_PRAGMAS 0
#	pragma comment(lib, "freeglut.lib")
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glew32s.lib")
#endif

#include "GL/freeglut.h"

#include "Render/Renderer.h"

Renderer r;
Scene s;
Camera c;

void reshape(int w, int l)
{
	glutReshapeWindow(800, 600);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 600, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	r.render(s, c);
}

int main(int argc, char** argv)
{
	r.setClearColor(Vector3(1.0, 0.0, 0.0));
	r.setViewport(800, 600);

	c.setPosition(Vector3(0, 0, -10));



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Test B-Spline");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);


	glutMainLoop();
	return 0;
}