#pragma once

#include "global.h"

void idle();
void keyboard(unsigned char input, int mouse_x, int mouse_y);
void mouse(int button, int state, int x, int y);
void reshape(int w, int h);