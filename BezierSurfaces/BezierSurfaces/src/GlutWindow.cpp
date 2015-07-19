#include "GlutWindow.h"

#include "include_glut.h"

GlutWindow::GlutWindow()
	: mSize(Vector2(800, 600)),
	mPos(Vector2(100, 100)),
	mName("Basic window"),
	mMode(GL_RGB),
	mActive(false)
{

}

GlutWindow::GlutWindow(int width, int height, int xpos, int ypos, std::string name, uint mode)
	: mSize(Vector2(width, height)),
	mPos(Vector2(xpos, ypos)),
	mName(name),
	mMode(mode),
	mActive(false)
{

}

GlutWindow::~GlutWindow()
{
}

void GlutWindow::enable()
{
	glutInitDisplayMode(mMode);
	glutInitWindowSize(mSize.x, mSize.y);
	glutInitWindowPosition(mPos.x, mPos.y);
	mNum = glutCreateWindow(mName.c_str());
	mActive = true;
}

void GlutWindow::disable()
{
	glutDestroyWindow(mNum);
	mActive = false;
}

void GlutWindow::renew()
{
	disable();
	enable();
}

void GlutWindow::setSize(Vector2 size)
{
	mSize = size;
	glutInitWindowSize(size.x, size.y);
}

void GlutWindow::setSize(int width, int height)
{
	mSize.x = width;
	mSize.y = height;
	glutInitWindowSize(width, height);
}

void GlutWindow::setPosition(Vector2 pos)
{
	mPos = pos;
	renew();
}

void GlutWindow::setName(std::string name)
{
	mName = name;
	renew();
}

void GlutWindow::setMode(uint mode)
{
	mMode = mode;
	renew();
}

bool GlutWindow::isActive()
{
	return mActive;
}

int GlutWindow::getNumber()
{
	return mNum;
}

Vector2 GlutWindow::getSize() const
{
	return mSize;
}

Vector2 GlutWindow::getPosition() const
{
	return mPos;
}

std::string GlutWindow::getName() const
{
	return mName;
}

uint GlutWindow::getMode() const
{
	return mMode;
}

void GlutWindow::display(void(*fp)())
{
	glutDisplayFunc(fp);
}

void GlutWindow::reshape(void(*fp)(int w, int h))
{
	glutReshapeFunc(fp);
}

void GlutWindow::mouse(void(*fp)(int button, int state, int x, int y))
{
	glutMouseFunc(fp);
}

void GlutWindow::motion(void(*fp)(int x, int y))
{
	glutMotionFunc(fp);
}

void GlutWindow::keyboardFunc(void(*fp)(unsigned char key, int x, int y))
{
	glutKeyboardFunc(fp);
}