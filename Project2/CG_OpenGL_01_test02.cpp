#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>
#include <iostream>
#include <random>
#include <chrono>
#include "VarANDFunc_test02.h"

auto seed{ std::chrono::system_clock::now().time_since_epoch().count() };
std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_int_distribution<int> uid(0, 255);

constexpr int Window_width{ 500 }, Window_height{ 500 };

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Window_width, Window_height);
	glutCreateWindow("Example2");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseClick);

	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA);
	glClear(GL_COLOR_BUFFER_BIT);

	Show_Background();
	Show_Rect();

	glutPostRedisplay();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		std::cout << "Exiting program." << std::endl;
		exit(0);
	}
}

void MouseClick(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		std::pair<float, float> ogl_xy = ConvertMouseWxy2GLxy(x, y);
		float mouse_x = ogl_xy.first;
		float mouse_y = ogl_xy.second;

		int clickedRectIndex = -1;
		int clickedBackRectIndex = -1;

		for (int i = 0; i < 4; ++i) {
			if (mouse_x >= originalRect_Bounds[i][0] && mouse_x <= originalRect_Bounds[i][2] &&
				mouse_y >= originalRect_Bounds[i][1] && mouse_y <= originalRect_Bounds[i][3]) {

				if (mouse_x >= Rect_Bounds[i][0] && mouse_x <= Rect_Bounds[i][2] &&
					mouse_y >= Rect_Bounds[i][1] && mouse_y <= Rect_Bounds[i][3]) {
					clickedRectIndex = i;
				}
				else {
					clickedBackRectIndex = i;
				}
				break;
			}
		}

		if (button == GLUT_LEFT_BUTTON) {
			if (clickedRectIndex != -1) {
				std::cout << "Left Clicked Inside Rectangle " << clickedRectIndex << std::endl;
				SetRandomRectColor(clickedRectIndex);
				std::cout << "Rectangle " << clickedRectIndex << " color changed." << std::endl;
			}
			else {
				std::cout << "Left Clicked Outside" << std::endl;
				SetRandomBackRectColor(clickedBackRectIndex);
				std::cout << "Rectangle " << clickedBackRectIndex << " color changed." << std::endl;
			}
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			const float scale_amount = 0.05f;
			if (clickedRectIndex != -1) {
				ScaleRectangle(clickedRectIndex, -scale_amount);
			}
			else {
				ScaleRectangle(clickedBackRectIndex, scale_amount);
			}
		}

		glutPostRedisplay();
	}
}

std::pair<float, float> ConvertMouseWxy2GLxy(int x, int y)
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	float ogl_x = (2.0f * x) / width - 1.0f;
	float ogl_y = 1.0f - (2.0f * y) / height;

	return { ogl_x, ogl_y };
}

void SetRandomRectColor(int rectIndex)
{
	if (rectIndex < 0 || rectIndex >= 4) return;
	Rect_Color[rectIndex][0] = static_cast<float>(uid(dre) / 255.0);
	Rect_Color[rectIndex][1] = static_cast<float>(uid(dre) / 255.0);
	Rect_Color[rectIndex][2] = static_cast<float>(uid(dre) / 255.0);
	std::cout << "Rect color changed" << std::endl;
}

void SetRandomBackRectColor(int rectIndex)
{
	if (rectIndex < 0 || rectIndex >= 4) return;
	BackRect_Color[rectIndex][0] = static_cast<float>(uid(dre) / 255.0);
	BackRect_Color[rectIndex][1] = static_cast<float>(uid(dre) / 255.0);
	BackRect_Color[rectIndex][2] = static_cast<float>(uid(dre) / 255.0);
	std::cout << "BackRect color changed" << std::endl;
}

void ScaleRectangle(int rectIndex, float amount) {
	float x_min = Rect_Bounds[rectIndex][0];
	float y_min = Rect_Bounds[rectIndex][1];
	float x_max = Rect_Bounds[rectIndex][2];
	float y_max = Rect_Bounds[rectIndex][3];

	float center_x = (x_min + x_max) / 2.0f;
	float center_y = (y_min + y_max) / 2.0f;

	float half_width = (x_max - x_min) / 2.0f;
	float half_height = (y_max - y_min) / 2.0f;

	float new_half_width = half_width + amount;
	float new_half_height = half_height + amount;

	const float min_half_size = 0.1f;
	if (new_half_width < min_half_size) new_half_width = min_half_size;
	if (new_half_height < min_half_size) new_half_height = min_half_size;

	float original_half_width = (originalRect_Bounds[rectIndex][2] - originalRect_Bounds[rectIndex][0]) / 2.0f;
	if (new_half_width > original_half_width) new_half_width = original_half_width;
	float original_half_height = (originalRect_Bounds[rectIndex][3] - originalRect_Bounds[rectIndex][1]) / 2.0f;
	if (new_half_height > original_half_height) new_half_height = original_half_height;

	Rect_Bounds[rectIndex][0] = center_x - new_half_width;
	Rect_Bounds[rectIndex][1] = center_y - new_half_height;
	Rect_Bounds[rectIndex][2] = center_x + new_half_width;
	Rect_Bounds[rectIndex][3] = center_y + new_half_height;
}

void Show_Rect()
{
	// 좌하단
	glColor3f(Rect_Color[0][0], Rect_Color[0][1], Rect_Color[0][2]);
	glRectf(Rect_Bounds[0][0], Rect_Bounds[0][1], Rect_Bounds[0][2], Rect_Bounds[0][3]);

	// 우하단
	glColor3f(Rect_Color[1][0], Rect_Color[1][1], Rect_Color[1][2]);
	glRectf(Rect_Bounds[1][0], Rect_Bounds[1][1], Rect_Bounds[1][2], Rect_Bounds[1][3]);

	// 좌상단
	glColor3f(Rect_Color[2][0], Rect_Color[2][1], Rect_Color[2][2]);
	glRectf(Rect_Bounds[2][0], Rect_Bounds[2][1], Rect_Bounds[2][2], Rect_Bounds[2][3]);

	// 우상단
	glColor3f(Rect_Color[3][0], Rect_Color[3][1], Rect_Color[3][2]);
	glRectf(Rect_Bounds[3][0], Rect_Bounds[3][1], Rect_Bounds[3][2], Rect_Bounds[3][3]);
}

void Show_Background()
{
	// 좌하단
	glColor3f(BackRect_Color[0][0], BackRect_Color[0][1], BackRect_Color[0][2]);
	glRectf(BackRect_Bounds[0][0], BackRect_Bounds[0][1], BackRect_Bounds[0][2], BackRect_Bounds[0][3]);

	// 우하단
	glColor3f(BackRect_Color[1][0], BackRect_Color[1][1], BackRect_Color[1][2]);
	glRectf(BackRect_Bounds[1][0], BackRect_Bounds[1][1], BackRect_Bounds[1][2], BackRect_Bounds[1][3]);

	// 좌상단
	glColor3f(BackRect_Color[2][0], BackRect_Color[2][1], BackRect_Color[2][2]);
	glRectf(BackRect_Bounds[2][0], BackRect_Bounds[2][1], BackRect_Bounds[2][2], BackRect_Bounds[2][3]);

	// 우상단
	glColor3f(BackRect_Color[3][0], BackRect_Color[3][1], BackRect_Color[3][2]);
	glRectf(BackRect_Bounds[3][0], BackRect_Bounds[3][1], BackRect_Bounds[3][2], BackRect_Bounds[3][3]);
}