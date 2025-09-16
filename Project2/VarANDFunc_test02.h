#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

extern float BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA;
extern const float originalRect_Bounds[4][4];
extern float Rect_Color[4][4];
extern float Rect_Bounds[4][4];
extern float BackRect_Color[4][4];
extern float BackRect_Bounds[4][4];

void SetRandomRectColor(int rectIndex);
void SetRandomBackRectColor(int rectIndex);
void Keyboard(unsigned char key, int x, int y);
void MouseClick(int button, int state, int x, int y);
std::pair<float, float> ConvertMouseWxy2GLxy(int x, int y);
void Show_Rect();
void Show_Background();
void ScaleRectangle(int rectIndex, float amount);