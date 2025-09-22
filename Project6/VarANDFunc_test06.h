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

extern float BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA;
extern float const increase_scale_amount;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
void MouseClick(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
std::pair<float, float> ConvertMouseWxy2GLxy(int x, int y);
void CreateParticleAt(float mouse_x, float mouse_y);