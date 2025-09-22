#include "VarANDFunc_test07.h"

static const int Window_width{ 900 }, Window_height{ 900 };
auto seed = std::chrono::system_clock::now().time_since_epoch().count();

std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_real_distribution<float> uid_window(-1.0f, 1.0f);
std::uniform_real_distribution<float> uid_color(0.0f, 1.0f);

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(Window_width, Window_height);
	glutCreateWindow("Example3");
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


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void MouseClick(int button, int state, int x, int y) {

}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		std::cout << "resetting the screen." << std::endl;

		glutPostRedisplay();
		break;
	case 'q':
		exit(0);
	}
}

std::pair<float, float> ConvertMouseWxy2GLxy(int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	float ogl_x = (2.0f * x) / width - 1.0f;
	float ogl_y = 1.0f - (2.0f * y) / height;

	return { ogl_x, ogl_y };
}
