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

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
{
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(500, 500); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { // glew 초기화 {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정

	glutKeyboardFunc(Keyboard);
	glutTimerFunc(TimerFunction);

	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() { //--- 콜백 함수: 출력 콜백 함수
	glClearColor(BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA); // 바탕색을 ‘blue’로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
	glViewport(0, 0, w, h);
}

void TimerFuntion(int value)
{
	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(100, TimerFuntion, 1); // 타이머함수 재 설정
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
