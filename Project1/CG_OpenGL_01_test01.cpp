#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <random>
#include <chrono>

auto seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_int_distribution<int> uid(0, 255);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

float BackGround_CR{ 0.0 }, BackGround_CG{ 0.0 }, BackGround_CB{ 1.0 }, BackGround_CA{ 1.0 };
bool isTimerActive = false;

void TimerFunction(int value);
void SetRandomBackgroundColor();
void Keyboard(unsigned char key, int x, int y);

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Example1");

	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutKeyboardFunc(Keyboard);

	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA);
	glClear(GL_COLOR_BUFFER_BIT);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void TimerFunction(int value)
{
	if (isTimerActive) {
		SetRandomBackgroundColor();
		glutPostRedisplay();
		glutTimerFunc(5000, TimerFunction, 1); // 5초마다 반복
	}
}

void SetRandomBackgroundColor()
{
	BackGround_CR = static_cast<float>(uid(dre) / 255.0);
	BackGround_CG = static_cast<float>(uid(dre) / 255.0);
	BackGround_CB = static_cast<float>(uid(dre) / 255.0);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		BackGround_CR = 0.0;
		BackGround_CG = 1.0;
		BackGround_CB = 1.0;
		glutPostRedisplay();
		break;
	case 'm':
		BackGround_CR = 1.0;
		BackGround_CG = 0.0;
		BackGround_CB = 1.0;
		glutPostRedisplay();
		break;
	case 'y':
		BackGround_CR = 1.0;
		BackGround_CG = 1.0;
		BackGround_CB = 0.0;
		glutPostRedisplay();
		break;
	case 'a':
		SetRandomBackgroundColor();
		glutPostRedisplay();
		break;
	case 'w':
		BackGround_CR = 1.0;
		BackGround_CG = 1.0;
		BackGround_CB = 1.0;
		glutPostRedisplay();
		break;
	case 'k':
		BackGround_CR = 0.0;
		BackGround_CG = 0.0;
		BackGround_CB = 0.0;
		glutPostRedisplay();
		break;
	case 't':
		if (!isTimerActive) {
			isTimerActive = true;
			glutTimerFunc(5000, TimerFunction, 1); // 타이머 시작
		}
		else {
			isTimerActive = false; // 타이머 중지
		}
		break;
	case 's':
		isTimerActive = false;
		break;
	case 'q':
		exit(0);
	}
}