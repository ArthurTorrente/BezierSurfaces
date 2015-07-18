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

#include <time.h>

BSpline b;

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
	// Clear de la zone de dessin
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	std::vector<Vector3> pointsC = b.getControlPoints();
	std::vector<std::vector<Vector3>> splines = b.getApproximedSplines();
	
	Vector3 v;
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (uint i = 0; i < pointsC.size(); i++)
	{
		v = pointsC[i];
		glVertex2f(v.x, v.y);
	}
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	for (uint i = 0; i < splines.size(); ++i)
	{
		
		glColor3f(
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
		);
		for (uint j = 0; j < splines[i].size(); ++j)
		{
			v = splines[i][j];
			glVertex2f(v.x, v.y);
		}

	}
	glEnd();
	

	glFlush();

}

int main(int argc, char** argv)
{
	/* initialize random seed: */
	srand(time(NULL));

	// Création d'une b-spline
	b = BSpline(30, 4);
	// b.approximeBounds(true);						==> Pour approximer les extrémités
	// b.closeSpline(true);							==> Pour fermer la bspline
	b.addControlPoint(Vector3(100, 100, 0));
	b.addControlPoint(Vector3(150, 400, 0));
	b.addControlPoint(Vector3(200, 100, 0));
	b.addControlPoint(Vector3(250, 400, 0));
	b.addControlPoint(Vector3(300, 100, 0));
	b.addControlPoint(Vector3(350, 400, 0));
	b.addControlPoint(Vector3(400, 100, 0));
	b.addControlPoint(Vector3(450, 400, 0));
	b.addControlPoint(Vector3(500, 100, 0));

	//b.useUniformNodal(false);
	//b.nodalVectorCustom(std::vector<float>{0, 1, 1, 1, 2, 3, 4, 5, 6, 6, 6, 7, 7, 8, 9, 10, 11});

	b.approximeSpline();

	// b.extrudeLinear(0, 50);						==> Extrusion linéaire avec un facteur de réduction
	// b.extrudeRevolution();						==> Extrusion de révolution
	// b.extrudeGeneral(b);							==> Extrusion généralisée selon une autre bspline

	BezierSurface bs(5, 10);

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