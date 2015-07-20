#ifndef _GLUTWINDOW_H_
#define _GLUTWINDOW_H_

#include "Math/Vector2.h"

class GlutWindow
{
public:

	GlutWindow();
	GlutWindow(int width, int height, int xpos, int ypos, std::string name, uint mode);
	~GlutWindow();

	void enable();
	void disable();
	void renew();

	void setSize(Vector2 size);
	void setSize(int width, int height);
	void setPosition(Vector2 pos);
	void setName(std::string name);
	void setMode(uint mode);
	bool isActive();
	int getNumber();
	Vector2 getSize() const;
	Vector2 getPosition() const;
	std::string getName() const;
	uint getMode() const;

	void display(void(*)());
	void reshape(void(*)(int w, int h));
	void mouse(void(*)(int button, int state, int x, int y));
	void motion(void(*)(int x, int y));
	void keyboardFunc(void(*)(unsigned char key, int x, int y));
	void wheel(void(*)(int button, int dir, int x, int y));

private:
	bool mActive;
	int mNum;
	Vector2 mSize;
	Vector2 mPos;
	std::string mName;
	uint mMode;
};

#endif