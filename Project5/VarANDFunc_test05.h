#pragma once
#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tuple>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <random>
#include <chrono>

extern float const BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA;
extern bool mouse_release, left_click_mod, right_click_mod;
extern float spread_rect_size;
extern float eraser_half_size;

void drawScene();
GLvoid Reshape(int w, int h);
void MouseDrag(int x, int y);
void MouseClick(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
std::pair<float, float> ConvertMouseWxy2GLxy(int x, int y);