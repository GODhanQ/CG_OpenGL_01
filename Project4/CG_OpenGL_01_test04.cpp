#include "VarANDFunc_test04.h"


// Set the background color to dark gray, and draw the squares in random colors.
// Draw a square centered on the point where you click the mouse.Draw up to five squares.
// Keyboard input(commands can be capitalized or lowercase) :
//	1 : Change position 1 -> Squares move diagonally and bounce off walls, moving in the other direction / stopping.
//	2 : Change position 2 -> Squares move / stop in a zigzag pattern.
//	3 : Change size -> Squares change / stay the same size.
//	4 : Change color -> Squares randomly change / stay the same color.
//	5 : Follow animation -> Follow / stop the animation of a randomly selected square among the animated squares in the order they were drawn.For example,
//      if the second square is selected, the animation will follow the second square in the order 1 -> 3 -> 4 -> 5 -> ...
//	s : Stop all animations.
//	m : The squares move to their original green positions.

auto seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_int_distribution<int> uid_color(0, 255);
std::uniform_int_distribution<int> uid_window(-Window_width, Window_width);

static int constexpr Window_width{ 700 }, Window_height{ 700 };

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
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

	

	glutPostRedisplay();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void MouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (Rect_dir.size() < 6) {
			std::cout << "Creating Rect." << std::endl;
			float mid_x = static_cast<float>(uid_window(dre)) / Window_width;
			float mid_y = static_cast<float>(uid_window(dre)) / Window_height;
			float half_size = 0.1f;

			auto ogl_xy = ConvertMouseWxy2GLxy(x, y);
			float mouse_x = ogl_xy.first;
			float mouse_y = ogl_xy.second;

			auto Rect_color = std::make_tuple(static_cast<float>(uid_color(dre)) / 255.0f,
				static_cast<float>(uid_color(dre)) / 255.0f,
				static_cast<float>(uid_color(dre)) / 255.0f);
			Rect_dir.push_back({ {mouse_x - half_size, mouse_y + half_size}, {mouse_x + half_size, mouse_y - half_size}, Rect_color });
			std::cout << "Rect created at (" << mouse_x << ", " << mouse_y << ") with color ("
				<< std::get<0>(Rect_color) << ", " << std::get<1>(Rect_color) << ", " << std::get<2>(Rect_color) << ")" << std::endl;
		}
		else {
			std::cout << "Maximum number of rectangles reached." << std::endl;
		}
	}
}
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		// rect are moving 
	case 'r':
		Rect_dir.clear();
		break;
	case 'q':
		exit(0);
	}
}
std::pair<float, float> ConvertMouseWxy2GLxy(int x, int y) {
	float ogl_x = (2.0f * x) / Window_width - 1.0f;
	float ogl_y = 1.0f - (2.0f * y) / Window_height;
	return { ogl_x, ogl_y };
}