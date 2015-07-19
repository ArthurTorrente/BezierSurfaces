#include "include_glut.h"
#include "Common.h"
#include "Interface.h"

void reshape(int w, int l)
{
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{	

	// Clear de la zone de dessin
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Dessin des courbes
	for (unsigned int i = 0; i < bsplines.size(); ++i)
	{
		bsplines[i]->draw();
	}

	// Dessin du carré d'édition
	if (CURRENT_CURVE_EDITED != -1)
	{
		Vector3 vector_tmp;
		if ((CURRENT_CURVE_EDITED >= 0 && CURRENT_CURVE_EDITED < bsplines.size()) && (CURRENT_VERTEX_EDITED >= 1 && CURRENT_VERTEX_EDITED <= bsplines[CURRENT_CURVE_EDITED]->getControlPoints().size()))
			vector_tmp = bsplines[CURRENT_CURVE_EDITED]->getControlPoints()[CURRENT_VERTEX_EDITED - 1];

		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
		glVertex2f(vector_tmp.x - 5, vector_tmp.y - 5);
		glVertex2f(vector_tmp.x + 5, vector_tmp.y - 5);
		glVertex2f(vector_tmp.x + 5, vector_tmp.y + 5);
		glVertex2f(vector_tmp.x - 5, vector_tmp.y + 5);
		glEnd();
	}

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
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
}