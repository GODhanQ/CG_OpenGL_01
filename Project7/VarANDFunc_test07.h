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
#include <type_traits>

extern float BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA;
extern std::pair<std::pair<float, float>, std::pair<float, float>> const Bounds_Bundle[12];
extern float const margin_size;
extern int Dragging_Rect_Index;
extern bool Auto_Solving_Flag;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
void MouseDrag(int x, int y);
void MouseClick(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
std::pair<float, float> ConvertMouseWxy2GLxy(int x, int y);
void CreateBoard();
void ShowAnswer();

//int GetBoundsBundleSize();