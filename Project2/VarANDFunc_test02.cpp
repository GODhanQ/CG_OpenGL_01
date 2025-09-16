#ifndef VARANDFUNC_H
#endif VARANDFUNC_H


#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VarANDFunc_test02.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

float BackGround_CR{ 0.0 }, BackGround_CG{ 0.0 }, BackGround_CB{ 0.0 }, BackGround_CA{ 1.0 };

const float originalRect_Bounds[4][4] = {
	{-1.0f, -1.0f, 0.0f, 0.0f}, // 좌하단
	{ 0.0f, -1.0f, 1.0f, 0.0f}, // 우하단
	{-1.0f,  0.0f, 0.0f, 1.0f}, // 좌상단
	{ 0.0f,  0.0f, 1.0f, 1.0f}  // 우상단
};
float Rect_Bounds[4][4] = {
	{-1.0f, -1.0f, 0.0f, 0.0f}, // 좌하단
	{ 0.0f, -1.0f, 1.0f, 0.0f}, // 우하단
	{-1.0f,  0.0f, 0.0f, 1.0f}, // 좌상단
	{ 0.0f,  0.0f, 1.0f, 1.0f}  // 우상단
};
float Rect_Color[4][4] = {
	{ 1.0f, 1.0f, 0.0f, 1.0f}, // 좌하단
	{ 0.0f, 1.0f, 1.0f, 1.0f}, // 우하단
	{ 1.0f, 0.0f, 1.0f, 1.0f}, // 좌상단
	{ 1.0f, 1.0f, 1.0f, 1.0f}  // 우상단
};
float BackRect_Bounds[4][4] = {
	{-1.0f, -1.0f, 0.0f, 0.0f}, // 좌하단
	{ 0.0f, -1.0f, 1.0f, 0.0f}, // 우하단
	{-1.0f,  0.0f, 0.0f, 1.0f}, // 좌상단
	{ 0.0f,  0.0f, 1.0f, 1.0f}  // 우상단
};
float BackRect_Color[4][4] = {
	{ 1.0f, 1.0f, 1.0f, 1.0f}, // 좌하단
	{ 1.0f, 0.0f, 1.0f, 1.0f}, // 우하단
	{ 0.0f, 1.0f, 1.0f, 1.0f}, // 좌상단
	{ 1.0f, 1.0f, 0.0f, 1.0f}  // 우상단
};