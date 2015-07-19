#include "Interface.h"

#include "include_glut.h"

int menuId = -1;

void specialKey(int, int, int)
{

}

// Fonction appelée par le clavier
void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
		editBSplines(7);
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

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
		mouseDown(button, x, y);

	else if (state == GLUT_UP)
		mouseUp(button, x, y);
}

void motion(int x, int y)
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

void mouseDown(int button, int x, int y)
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

void mouseUp(int button, int x, int y)
{
	if (button == GLUT_MIDDLE_BUTTON)
	{
		MIDDLE_BUTTON_PRESSED = false;
	}
}

void initMenu()
{
	refreshUI();
}


void refreshUI()
{
	if (menuId != -1)
		glutDestroyMenu(menuId);

	switch (currentMode)
	{
		case MAIN:
		{
			menuId = glutCreateMenu(selectMode);
			glutAddMenuEntry("B-Splines", 1);
			glutAddMenuEntry("Bezier Surfaces", 2);
			glutAddMenuEntry("Quit", 3);
			break;
		}
	
		case BSPLINES_MAIN:
		{
			menuId = glutCreateMenu(selectBSplines);
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
			int colorMenu = glutCreateMenu(selectColor);
			glutAddMenuEntry("Blue", 300);
			glutAddMenuEntry("Green", 301);
			glutAddMenuEntry("Cyan", 302);
			glutAddMenuEntry("Red", 303);
			glutAddMenuEntry("Purple", 304);
			glutAddMenuEntry("Yellow", 305);
			glutAddMenuEntry("Toogle sub curves", 306);

			int extrudeMenu = glutCreateMenu(selectExtrude);
			glutAddMenuEntry("Linear", 400);
			glutAddMenuEntry("Revolution", 401);
			glutAddMenuEntry("General", 402);

			menuId = glutCreateMenu(editBSplines);
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

void selectMode(int mode)
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

	refreshUI();
}

void selectBSplines(int selection)
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

	refreshUI();

}

void editBSplines(int selection)
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
			refreshUI();
			break;
		default:
			break;
	}
}

void selectColor(int selection)
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

void selectExtrude(int selection)
{

}