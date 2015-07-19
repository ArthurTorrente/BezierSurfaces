#include "Common.h"
#include "include_glut.h"

extern void reshape3D(int, int);
extern void display3D();
extern void mouse3D(int, int, int, int);
extern void motion3D(int, int);

int menuIdBSpline = -1;

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


void keyboardBSpline(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
		editBSplinesBSpline(7);
		break;
	case 127:
		if (CURRENT_CURVE_EDITED >= 0 && CURRENT_CURVE_EDITED < bsplines.size())
		{
			if (CURRENT_VERTEX_EDITED >= 1)
			{
				bsplines[CURRENT_CURVE_EDITED]->remove(CURRENT_VERTEX_EDITED - 1);
				bsplines[CURRENT_CURVE_EDITED]->approximeSpline();

				if (CURRENT_VERTEX_EDITED > bsplines[CURRENT_CURVE_EDITED]->getControlPoints().size())
					CURRENT_VERTEX_EDITED = bsplines[CURRENT_CURVE_EDITED]->getControlPoints().size();
			}
		}
		glutPostRedisplay();
		break;
	case '+':
		if (CURRENT_CURVE_EDITED >= 0 && CURRENT_CURVE_EDITED < bsplines.size())
		{
			bsplines[CURRENT_CURVE_EDITED]->setLOD(bsplines[CURRENT_CURVE_EDITED]->getLOD() + 1);
			bsplines[CURRENT_CURVE_EDITED]->approximeSpline();
		}
		glutPostRedisplay();
		break;
	case '-':
		if (CURRENT_CURVE_EDITED >= 0 && CURRENT_CURVE_EDITED < bsplines.size())
		{
			bsplines[CURRENT_CURVE_EDITED]->setLOD(bsplines[CURRENT_CURVE_EDITED]->getLOD() - 1);
			bsplines[CURRENT_CURVE_EDITED]->approximeSpline();
		}
		glutPostRedisplay();
		break;
	}
}

void reshapeBSpline(int w, int h)
{
	bsplineWindow->setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void displayBSpline()
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

void mouseBSpline(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		float xF = x;
		float yF = y;

		// Clic gauche => Ajout d'un nouveau point dans le polygone en cours (soit à la fin soit insertion à l'espace en cours
		if (button == GLUT_LEFT_BUTTON)
		{
			if (CURRENT_CURVE_EDITED >= 0 && CURRENT_CURVE_EDITED < bsplines.size())
			{
				if (CURRENT_VERTEX_EDITED == bsplines[CURRENT_CURVE_EDITED]->getControlPoints().size())
					bsplines[CURRENT_CURVE_EDITED]->addControlPoint(Vector3(xF, yF, 0));
				else
					bsplines[CURRENT_CURVE_EDITED]->insert(CURRENT_VERTEX_EDITED, Vector3(xF, yF, 0));
				bsplines[CURRENT_CURVE_EDITED]->approximeSpline();
				CURRENT_VERTEX_EDITED++;
			}
		}

		else if (button == GLUT_MIDDLE_BUTTON)
		{
			MIDDLE_BUTTON_PRESSED = true;

			float distance = 1000.0;
			if (CURRENT_CURVE_EDITED >= 0 && CURRENT_CURVE_EDITED < bsplines.size())
			{
				unsigned int size = bsplines[CURRENT_CURVE_EDITED]->getControlPoints().size();
				for (unsigned int i = 0; i < size; ++i)
				{
					Vector3 currentPoint = bsplines[CURRENT_CURVE_EDITED]->getControlPoints()[i];
					float currentDistance = currentPoint.distance(Vector3(xF, yF, 0));
					if (currentDistance < distance)
					{
						distance = currentDistance;
						CURRENT_VERTEX_EDITED = (i + 1);
					}
				}
			}
		}

		glutPostRedisplay();
	}

	else if (state == GLUT_UP)
	{
		MIDDLE_BUTTON_PRESSED = false;
	}
}

void motionBSpline(int x, int y)
{
	float xF = x;
	float yF = y;

	if (MIDDLE_BUTTON_PRESSED == true)
	{
		if (CURRENT_CURVE_EDITED >= 0 && CURRENT_CURVE_EDITED < bsplines.size())
		{
			if (CURRENT_VERTEX_EDITED >= 0 && CURRENT_VERTEX_EDITED <= bsplines[CURRENT_CURVE_EDITED]->getControlPoints().size())
			{
				bsplines[CURRENT_CURVE_EDITED]->getVectorAt(CURRENT_VERTEX_EDITED - 1).x = xF;
				bsplines[CURRENT_CURVE_EDITED]->getVectorAt(CURRENT_VERTEX_EDITED - 1).y = yF;
				bsplines[CURRENT_CURVE_EDITED]->approximeSpline();
			}
		}
	}

	glutPostRedisplay();
}

void initMenuBSpline()
{
	refreshUIBSpline();
}


void refreshUIBSpline()
{
	if (menuIdBSpline != -1)
		glutDestroyMenu(menuIdBSpline);

	switch (currentMode)
	{
		case MAIN:
		{
			menuIdBSpline = glutCreateMenu(selectModeBSpline);
			glutAddMenuEntry("B-Splines", 1);
			glutAddMenuEntry("Bezier Surfaces", 2);
			glutAddMenuEntry("Quit", 3);
			break;
		}

		case BSPLINES_MAIN:
		{
			menuIdBSpline = glutCreateMenu(selectBSplinesBSpline);
			glutAddMenuEntry("New", 100);

			std::string name;
			for (uint i = 0; i < bsplines.size(); ++i)
			{
				name = "B-Spline ";
				name += std::to_string(i + 1);
				glutAddMenuEntry(name.c_str(), 101 + i);
			}

			glutAddMenuEntry("Back", 101 + bsplines.size());
			break;
		}
		case BSPLINES_EDITION:
		{
			int colorMenu = glutCreateMenu(selectColorBSpline);
			glutAddMenuEntry("Blue", 300);
			glutAddMenuEntry("Green", 301);
			glutAddMenuEntry("Cyan", 302);
			glutAddMenuEntry("Red", 303);
			glutAddMenuEntry("Purple", 304);
			glutAddMenuEntry("Yellow", 305);
			glutAddMenuEntry("Toogle sub curves", 306);

			int extrudeMenu = glutCreateMenu(selectExtrudeBSpline);
			glutAddMenuEntry("Linear", 400);
			glutAddMenuEntry("Revolution", 401);
			glutAddMenuEntry("General", 402);

			menuIdBSpline = glutCreateMenu(editBSplinesBSpline);
			glutAddSubMenu("Color", colorMenu);
			glutAddSubMenu("Extrude", extrudeMenu);
			glutAddMenuEntry("Toggle Approx. bounds", 202);
			glutAddMenuEntry("Toggle Close spline", 203);
			glutAddMenuEntry("Order up", 204);
			glutAddMenuEntry("Order down", 205);
			glutAddMenuEntry("Remove", 206);
			glutAddMenuEntry("Back", 207);

			break;
		}
		case BEZIERSURFACES:
		{
			// menu = glutCreateMenu(selectBezierSurfaces);
			break;
		}

	}

	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void selectModeBSpline(int mode)
{
	switch (mode)
	{
	case 1:
		currentMode = BSPLINES_MAIN;
		break;

	case 2:
		currentMode = BEZIERSURFACES;
		break;

	case 3:
		exit(0);
		break;
	}

	refreshUIBSpline();
}

void selectBSplinesBSpline(int selection)
{
	uint id = selection % 100;

	if (id == 0)
	{
		bsplines.push_back(new BSpline());
		CURRENT_CURVE_EDITED = bsplines.size() - 1;
		currentMode = BSPLINES_EDITION;
	}
	else if (id == bsplines.size() + 1)
	{
		currentMode = MAIN;
		CURRENT_CURVE_EDITED = -1;
	}
	else
	{
		currentMode = BSPLINES_EDITION;
		CURRENT_CURVE_EDITED = id - 1;
		CURRENT_VERTEX_EDITED = bsplines[CURRENT_CURVE_EDITED]->getControlPoints().size();
		glutPostRedisplay();
	}

	refreshUIBSpline();

}

void editBSplinesBSpline(int selection)
{
	int id = selection % 100;

	switch (id)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		bsplines[CURRENT_CURVE_EDITED]->approximeBounds(!bsplines[CURRENT_CURVE_EDITED]->isApproximed());
		bsplines[CURRENT_CURVE_EDITED]->approximeSpline();
		glutPostRedisplay();
		break;
	case 3:
		bsplines[CURRENT_CURVE_EDITED]->closeSpline(!bsplines[CURRENT_CURVE_EDITED]->isClosed());
		bsplines[CURRENT_CURVE_EDITED]->approximeSpline();
		glutPostRedisplay();
		break;
	case 4:
		bsplines[CURRENT_CURVE_EDITED]->orderUp();
		bsplines[CURRENT_CURVE_EDITED]->approximeSpline();
		glutPostRedisplay();
		break;
	case 5:
		bsplines[CURRENT_CURVE_EDITED]->orderDown();
		bsplines[CURRENT_CURVE_EDITED]->approximeSpline();
		glutPostRedisplay();
		break;
	case 6:
		bsplines.erase(bsplines.begin() + CURRENT_CURVE_EDITED);
		CURRENT_CURVE_EDITED = -1;
		glutPostRedisplay();
	case 7:
		currentMode = BSPLINES_MAIN;
		CURRENT_CURVE_EDITED = -1;
		CURRENT_VERTEX_EDITED = 0;
		glutPostRedisplay();
		refreshUIBSpline();
		break;
	default:
		break;
	}
}

void selectColorBSpline(int selection)
{
	int id = selection % 100;

	switch (id)
	{
	case 0:		// Blue						
		bsplines[CURRENT_CURVE_EDITED]->setColor(Vector3(0, 0, 1));
		break;
	case 1:		// Green									
		bsplines[CURRENT_CURVE_EDITED]->setColor(Vector3(0, 1, 0));
		break;
	case 2:		// Cyan
		bsplines[CURRENT_CURVE_EDITED]->setColor(Vector3(0, 1, 1));
		break;
	case 3:		// Red
		bsplines[CURRENT_CURVE_EDITED]->setColor(Vector3(1, 0, 0));
		break;
	case 4:		// Purple
		bsplines[CURRENT_CURVE_EDITED]->setColor(Vector3(1, 0, 1));
		break;
	case 5:		// Yellow
		bsplines[CURRENT_CURVE_EDITED]->setColor(Vector3(1, 1, 0));
		break;
	case 6:
		bsplines[CURRENT_CURVE_EDITED]->showSubCurve(!bsplines[CURRENT_CURVE_EDITED]->subCurveVisible());
	}

	glutPostRedisplay();
}

void selectExtrudeBSpline(int selection)
{
	int id = selection % 100;

	switch (id)
	{
	case 0:
		/*bsplines[CURRENT_CURVE_EDITED]->extrudeLinear(0, 0);
		MaterialSPtr mat(new BasicMaterial("simple", Vector3(1, 1, 0)));
		MeshSPtr mesh(new Mesh(bsplines[CURRENT_CURVE_EDITED]->getGeometry(), mat));
		Object3D* obj = new Object3D();
		obj->setMesh(mesh);
		s.add(obj);*/

		// Ajout d'un cube pour voir ce qu'il se passe
		Geometry geo = Geometry(
			std::vector<float>(g_cubeVertices, g_cubeVertices + sizeof(g_cubeVertices) / sizeof(float)),
			std::vector<int>(g_cubeIndices, g_cubeIndices + sizeof(g_cubeIndices) / sizeof(int))
		);

		MaterialSPtr mat_2(new BasicMaterial("simple", Vector3(1, 1, 0)));
		MeshSPtr cube(new Mesh(geo, mat_2));
		Object3D* obj_2 = new Object3D();
		obj_2->setMesh(cube);
		s.add(obj_2);

		break;
	}

	if (spaceWindow == NULL || !spaceWindow->isActive())
	{
		spaceWindow = new GlutWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 10, 10, "B-Splines and Bezier Surfaces", GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
		spaceWindow->enable();
		spaceWindow->display(display3D);
		spaceWindow->reshape(reshape3D);
		spaceWindow->mouse(mouse3D);
		spaceWindow->motion(motion3D);
		// spaceWindow->keyboardFunc(keyboardBSpline);

	}


}