#include "VarANDFunc_test06.h"

static const int Window_width{ 900 }, Window_height{ 700 };

auto seed = std::chrono::system_clock::now().time_since_epoch().count();

std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_real_distribution<float> uid_window(-1.0f, 1.0f);
std::uniform_real_distribution<float> uid_color(0.0f, 1.0f);

// Rect_dir: bounds1, bounds2, color, origin
std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>, std::pair<float, float>>> Rect_dir;
// Particle_Rect_dir : bounds1, bounds2, color, origin, move_vec, scale_vec
std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>, std::pair <float, float>, std::pair<float, float>, std::pair<float, float>>> Particle_Rect_dir;

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

	for(int i = 0 ;i < Rect_dir.size(); ++i){
		auto Rect_color{ std::get<2>(Rect_dir[i]) };
		auto& Rect_Bounds1{ std::get<0>(Rect_dir[i]) };
		auto& Rect_Bounds2{ std::get<1>(Rect_dir[i]) };
		glColor3f(std::get<0>(Rect_color), std::get<1>(Rect_color), std::get<2>(Rect_color));
		glRectf(Rect_Bounds1.first, Rect_Bounds1.second, Rect_Bounds2.first, Rect_Bounds2.second);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void MouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		std::cout << "Mouse left button clicked at (" << x << ", " << y << ")." << std::endl;

		auto ogl_xy = ConvertMouseWxy2GLxy(x, y);
		float mouse_x = ogl_xy.first;
		float mouse_y = ogl_xy.second;

		for(int i = 0; i < Rect_dir.size(); ++i){
			auto& Rect_Bounds1{ std::get<0>(Rect_dir[i]) };
			auto& Rect_Bounds2{ std::get<1>(Rect_dir[i]) };
			if (mouse_x >= Rect_Bounds1.first && mouse_x <= Rect_Bounds2.first &&
				mouse_y <= Rect_Bounds1.second && mouse_y >= Rect_Bounds2.second) {
				std::cout << "Rectangle at index " << i << " clicked and removed." << std::endl;

				// 1. calculate particle rect bound1
				// 2. calculate particle rect bound2
				// 3. copy color
				// 4. calculate particle rect origin
				// 5. calculate move vec
				// 6. calculate scale vec

				Rect_dir.erase(Rect_dir.begin() + i);
				break;
			}
		}

	}
	else {
		std::cout << "Mouse button released at (" << x << ", " << y << ")." << std::endl;
	}
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		std::cout << "resetting the screen." << std::endl;
		Rect_dir.clear();

		{
			std::uniform_int_distribution<int> uid_spread(5, 10);
			std::uniform_real_distribution<float> urd_half_size_X(0.05f, 0.2f);
			std::uniform_real_distribution<float> urd_half_size_Y(0.05f, 0.2f);

			int num_rects = uid_spread(dre);
			for (int i = 0; i < num_rects; ++i) {
				float mid_x = uid_window(dre);
				float mid_y = uid_window(dre);
				float half_size_X{ urd_half_size_X(dre) };
				float half_size_Y{ urd_half_size_Y(dre) };
				float x1 = mid_x - half_size_X, y1 = mid_y + half_size_Y, x2 = mid_x + half_size_X, y2 = mid_y - half_size_Y;
				std::tuple<float, float, float> Rect_color;
				Rect_color = std::make_tuple(uid_color(dre), uid_color(dre), uid_color(dre));
				Rect_dir.push_back({ {x1, y1}, {x2, y2}, Rect_color , {mid_x, mid_y} });
			}
		}

		glutPostRedisplay();
		break;
	case 'q':
		exit(0);
	}
}
