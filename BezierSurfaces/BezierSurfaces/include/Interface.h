#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "Common.h"

void specialKey(int, int, int);
void key(unsigned char, int, int);
void mouse(int, int, int, int);
void motion(int, int);

void mouseDown(int, int, int);
void mouseUp(int, int, int);

void initMenu();
void refreshUI();

void selectMode(int);

#endif