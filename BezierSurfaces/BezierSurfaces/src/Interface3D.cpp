#include "Common.h"
#include "include_glut.h"

int menuId3D = -1;

void reshape3D(int w, int h)
{

}

void display3D()
{
	std::cout << "top" << std::endl;

	r.render(s, c);

	if (r.autoUpdate())
		glutPostRedisplay();
}

void mouse3D(int button, int state, int x, int y)
{

}

void motion3D(int x, int y)
{

}
