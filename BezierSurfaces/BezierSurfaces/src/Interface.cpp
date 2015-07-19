#include "Interface.h"

#include "include_glut.h"

void specialKey(int, int, int);
void key(unsigned char, int, int);
void mouse(int, int, int, int);
void motion(int, int);

void mouseDown(int, int, int);
void mouseUp(int, int, int);

void initMenu()
{
	refreshUI();
}


void refreshUI()
{
	switch (currentMode)
	{
		case MAIN:
		{
			int mainMenu = glutCreateMenu(selectMode);
			glutAddMenuEntry("B-Splines", 1);
			glutAddMenuEntry("Bezier Surfaces", 2);
			glutAddMenuEntry("Quit", 3);
			break;
		}
	
	}
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void selectMode(int mode)
{
	std::cout << mode << std::endl;
}