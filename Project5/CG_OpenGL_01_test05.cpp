#include "VarANDFunc_test05.h"

static int constexpr Window_width{ 700 }, Window_height{ 700 };

// Rect_dir: bounds1, bounds2, color
std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>>> Spread_Rect_dir;
// Eraeser_Rect: bounds1, bounds2, color, half_size
std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>, float> Eraser_Rect;
auto seed = std::chrono::system_clock::now().time_since_epoch().count();

std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_int_distribution<int> uid_window(-Window_width, Window_width);
std::uniform_int_distribution<int> uid_spread(20, 40);
std::uniform_real_distribution<float> uid_color(0.0f, 1.0f);

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
	glutMotionFunc(MouseDrag);

	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& rect : Spread_Rect_dir) {
		auto Rect_color{ std::get<2>(rect) };
		auto& Rect_Bounds1{ std::get<0>(rect) };
		auto& Rect_Bounds2{ std::get<1>(rect) };
		glColor3f(std::get<0>(Rect_color), std::get<1>(Rect_color), std::get<2>(Rect_color));
		glRectf(Rect_Bounds1.first, Rect_Bounds1.second, Rect_Bounds2.first, Rect_Bounds2.second);
	}
	if (!mouse_release) {
		auto& Rect_Bounds1{ std::get<0>(Eraser_Rect) };
		auto& Rect_Bounds2{ std::get<1>(Eraser_Rect) };
		auto& Rect_color{ std::get<2>(Eraser_Rect) };
		glColor3f(std::get<0>(Rect_color), std::get<1>(Rect_color), std::get<2>(Rect_color));
		glRectf(Rect_Bounds1.first, Rect_Bounds1.second, Rect_Bounds2.first, Rect_Bounds2.second);
	}
	if (right_click_mod) {
		auto& Rect_Bounds1{ std::get<0>(Eraser_Rect) };
		auto& Rect_Bounds2{ std::get<1>(Eraser_Rect) };
		auto& Rect_color{ std::get<2>(Eraser_Rect) };
		glColor3f(std::get<0>(Rect_color), std::get<1>(Rect_color), std::get<2>(Rect_color));
		glRectf(Rect_Bounds1.first, Rect_Bounds1.second, Rect_Bounds2.first, Rect_Bounds2.second);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void MouseDrag(int x, int y) {
	if (!mouse_release) {
		if (left_click_mod) {
			auto ogl_xy = ConvertMouseWxy2GLxy(x, y);
			float mouse_x = ogl_xy.first;
			float mouse_y = ogl_xy.second;
			float half_size = std::get<3>(Eraser_Rect);
			std::get<0>(Eraser_Rect).first = mouse_x - half_size;
			std::get<1>(Eraser_Rect).first = mouse_x + half_size;
			std::get<0>(Eraser_Rect).second = mouse_y + half_size;
			std::get<1>(Eraser_Rect).second = mouse_y - half_size;
			

			std::cout << "Erase detection initiated." << std::endl;
			for (int i = 0; i < Spread_Rect_dir.size(); ++i) {
				auto& Rect_Bounds1 = std::get<0>(Spread_Rect_dir[i]);
				auto& Rect_Bounds2 = std::get<1>(Spread_Rect_dir[i]);
				auto& Eraser_Rect_Bounds1 = std::get<0>(Eraser_Rect);
				auto& Eraser_Rect_Bounds2 = std::get<1>(Eraser_Rect);

				// if the eraser rectangle overlaps with the spread rectangle, erase the spread rectangle
				if (!(Eraser_Rect_Bounds2.first < Rect_Bounds1.first || // left
					Eraser_Rect_Bounds1.first > Rect_Bounds2.first || // right
					Eraser_Rect_Bounds2.second > Rect_Bounds1.second || // bottom
					Eraser_Rect_Bounds1.second < Rect_Bounds2.second)) { // top
					std::cout << "Erased rectangle at index " << i << std::endl;
					Spread_Rect_dir.erase(Spread_Rect_dir.begin() + i);

					auto& Eraser_Rect_half_size = std::get<3>(Eraser_Rect);
					Eraser_Rect_half_size += increment_size;

					--i; // Adjust index after erasure
				}
			}

			glutPostRedisplay();
		}
	}
}

void MouseClick(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		mouse_release = false;
		if (button == GLUT_LEFT_BUTTON) {
			std::cout << "Left click detected at (" << x << ", " << y << ")." << std::endl;
			left_click_mod = true;
			right_click_mod = false;
		}

		else if (button == GLUT_RIGHT_BUTTON) {
			std::cout << "Right click detected at (" << x << ", " << y << ")." << std::endl;
			right_click_mod = true;
			left_click_mod = false;

			auto ogl_xy = ConvertMouseWxy2GLxy(x, y);
			float mouse_x = ogl_xy.first;
			float mouse_y = ogl_xy.second;

			auto& Eraser_Rect_half_size = std::get<3>(Eraser_Rect);
			if (Eraser_Rect_half_size > 0.06f) {
				std::cout << "Decreasing eraser size." << std::endl;
				Eraser_Rect_half_size -= increment_size;
				auto& Eraser_Rect_Bounds1 = std::get<0>(Eraser_Rect);
				auto& Eraser_Rect_Bounds2 = std::get<1>(Eraser_Rect);
				Eraser_Rect_Bounds1.first = mouse_x - Eraser_Rect_half_size;
				Eraser_Rect_Bounds2.first = mouse_x + Eraser_Rect_half_size;
				Eraser_Rect_Bounds1.second = mouse_y + Eraser_Rect_half_size;
				Eraser_Rect_Bounds2.second = mouse_y - Eraser_Rect_half_size;

				float mid_x = static_cast<float>(uid_window(dre)) / Window_width;
				float mid_y = static_cast<float>(uid_window(dre)) / Window_height;
				float half_size = spread_rect_size / 2.0f;
				float x1 = mid_x - half_size, y1 = mid_y + half_size, x2 = mid_x + half_size, y2 = mid_y - half_size;
				std::tuple<float, float, float> Rect_color;
				Rect_color = std::make_tuple(uid_color(dre), uid_color(dre), uid_color(dre));
				Spread_Rect_dir.push_back({ {x1, y1}, {x2, y2}, Rect_color });
			}
			else {
				std::cout << "Eraser size is at minimum. Cannot decrease further." << std::endl;
			}

			glutPostRedisplay();
		}
	}
	else {
		std::cout << "Mouse button released at (" << x << ", " << y << ")." << std::endl;
		mouse_release = true;
		left_click_mod = false;
		right_click_mod = false;
	}
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		std::cout << "resetting the screen." << std::endl;
		Spread_Rect_dir.clear();

		int num_rects;
		num_rects = uid_spread(dre);
		for (int i = 0; i < num_rects; ++i) {
			float mid_x = static_cast<float>(uid_window(dre)) / Window_width;
			float mid_y = static_cast<float>(uid_window(dre)) / Window_height;
			float half_size = spread_rect_size / 2.0f;
			float x1 = mid_x - half_size, y1 = mid_y + half_size, x2 = mid_x + half_size, y2 = mid_y - half_size;
			std::tuple<float, float, float> Rect_color;
			Rect_color = std::make_tuple(uid_color(dre), uid_color(dre), uid_color(dre));
			Spread_Rect_dir.push_back({ {x1, y1}, {x2, y2}, Rect_color });
		}
		Eraser_Rect = { {0.1f, 0.1f}, {0.1f, 0.1f}, {0.5f, 0.5f, 0.5f}, eraser_half_size };

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