#include "Common.h"
#include "include_glut.h"

#include "BSpline.h"
#include "BezierSurface.h"

#include "Render/Renderer.h"
#include "Objects/Scene.h"
#include "Cameras/PerspectiveCamera.h"

int menuId3D = -1;
Vector2 originClick;

void reshape3D(int w, int h)
{

}

void display3D()
{
	r->render(*s, *c);

	if (r->autoUpdate())
		glutPostRedisplay();
}

void mouse3D(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{

			//std::cout << "new click" << std::endl;
			originClick.x = x;
			originClick.y = y;

			/*Vector3 mouse(x, y, 0);

			int distance = 10;
			const std::vector<std::vector<Vector3>>& points = beziersurface->getControlPoints();
			Vector3 screenSpace;
			for (uint i = 0; i < points.size(); ++i)
			{
				for (uint j = 0; j < points[i].size(); ++j)
				{
					screenSpace = c->getProjectionMatrix() * c->getWorldMatrix().transpose() * points[i][j];
					std::cout << mouse.distance(screenSpace) << std::endl;
				}
			}*/


		}
		else if (state == GLUT_UP)
		{

		}

	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{

	}
}

void motion3D(int x, int y)
{
	int xF = x;
	int yF = y;

	if (originClick.x < xF)
	{
		c->rotateY(0.01);
	}
	else
	{
		c->rotateY(-0.01);
	}

	if (originClick.y < yF)
	{
		c->rotateX(0.01);
	}
	else
	{
		c->rotateX(-0.01);
	}

	originClick.x = xF;
	originClick.y = yF;

	glutPostRedisplay();
}

void keyboard3D(unsigned char k, int x, int y)
{

}

void wheel3D(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		c->translateZ(10.0f);
	}
	else
	{
		c->translateZ(-10.0f);
	}

	glutPostRedisplay();
}

void initMenu3D()
{
	int menu = glutCreateMenu(selectMode3D);
	glutAddMenuEntry("Close", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void selectMode3D(int selection)
{
	switch (selection)
	{
		case 1:
		{
			spaceWindow->disable();
			break;
		}
	}
}