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

void TimerFuntion(int value);

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� { //--- ������ �����ϱ�
{
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(500, 500); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { // glew �ʱ�ȭ {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����

	glutKeyboardFunc(Keyboard);
	glutTimerFunc(TimerFunction);

	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() { //--- �ݹ� �Լ�: ��� �ݹ� �Լ�
	glClearColor(BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA); // �������� ��blue���� ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
	glViewport(0, 0, w, h);
}

void TimerFuntion(int value)
{
	glutPostRedisplay(); // ȭ�� �� ���
	glutTimerFunc(100, TimerFuntion, 1); // Ÿ�̸��Լ� �� ����
}


void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		BackGround_CR = 0.0;
		BackGround_CG = 1.0;
		BackGround_CB = 1.0;
		break;
	case 'm':
		BackGround_CR = 1.0;
		BackGround_CG = 0.0;
		BackGround_CB = 1.0;
		break;
	case 'y':
		BackGround_CR = 1.0;
		BackGround_CG = 1.0;
		BackGround_CB = 0.0;
		break;
	case 'a':
		BackGround_CR = static_cast<double>(uid(dre)) / 255.0;
		BackGround_CG = static_cast<double>(uid(dre)) / 255.0;
		BackGround_CB = static_cast<double>(uid(dre)) / 255.0;
		break;
	case 'w':
		BackGround_CR = 1.0;
		BackGround_CG = 1.0;
		BackGround_CB = 1.0;
		break;
	case 'k':
		BackGround_CR = 0.0;
		BackGround_CG = 0.0;
		BackGround_CB = 0.0;
		break;
	case 't':
		
		break;
	case 's':
		
		break;
	case 'q':
		exit(0);
	}
}
