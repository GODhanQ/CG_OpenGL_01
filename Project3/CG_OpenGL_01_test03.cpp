#include "VarANDFunc_test03.h"

static std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>>> Rect_dir;
static constexpr int Window_width{ 1000 }, Window_height{ 1000 };
static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_int_distribution<int> uid_color(0, 255);
std::uniform_int_distribution<int> uid_window(-Window_width, Window_width);

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
	UpdateRect();

	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(BackGround_CR, BackGround_CG, BackGround_CB, BackGround_CA);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < Rect_dir.size(); ++i) {
		auto Rect_color{ std::get<2>(Rect_dir[i]) };
		auto &Rect_Bounds1{ std::get<0>(Rect_dir[i]) };
		auto &Rect_Bounds2{ std::get<1>(Rect_dir[i]) };
		glColor3f(std::get<0>(Rect_color), std::get<1>(Rect_color), std::get<2>(Rect_color));
		glRectf(Rect_Bounds1.first, Rect_Bounds1.second, Rect_Bounds2.first, Rect_Bounds2.second);
	}

	glutPostRedisplay();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void MouseDrag(int x, int y) {
	if (DraggingRectIndex == -1) return;
	auto ogl_xy = ConvertMouseWxy2GLxy(x, y);
	float mouse_x = ogl_xy.first;
	float mouse_y = ogl_xy.second;
	float half_size = 0.1f;
	std::get<0>(Rect_dir[DraggingRectIndex]).first = mouse_x - half_size;
	std::get<1>(Rect_dir[DraggingRectIndex]).first = mouse_x + half_size;
	std::get<0>(Rect_dir[DraggingRectIndex]).second = mouse_y + half_size;
	std::get<1>(Rect_dir[DraggingRectIndex]).second = mouse_y - half_size;
	glutPostRedisplay();
}

void MouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			auto ogl_xy = ConvertMouseWxy2GLxy(x, y);
			float mouse_x = ogl_xy.first;
			float mouse_y = ogl_xy.second;
			DraggingRectIndex = -1;
			for (int i = 0; i < Rect_dir.size(); ++i) {
				auto &Rect_Bounds1 = std::get<0>(Rect_dir[i]);
				auto &Rect_Bounds2 = std::get<1>(Rect_dir[i]);
				if (mouse_x >= Rect_Bounds1.first && mouse_x <= Rect_Bounds2.first &&
					mouse_y >= Rect_Bounds2.second && mouse_y <= Rect_Bounds1.second) {
					DraggingRectIndex = i;
					std::cout << "Selected Rect " << i << " for dragging." << std::endl;
					break;
				}
			}
		}
		else if (state == GLUT_UP) {
			std::cout << "Released mouse button." << std::endl;
			DraggingRectIndex = -1;
		}
	}
}


void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		std::cout << "Rectangles" << std::endl;
		for (int i = 0; i < Rect_dir.size(); ++i) {
			auto Rect_color{ std::get<2>(Rect_dir[i]) };
			auto& Rect_Bounds1{ std::get<0>(Rect_dir[i]) };
			auto& Rect_Bounds2{ std::get<1>(Rect_dir[i]) };
			std::cout << "Rect " << i << ": ";
			std::cout << "Bounds: ((" << Rect_Bounds1.first << ", " << Rect_Bounds1.second << "), (" << Rect_Bounds2.first << ", " << Rect_Bounds2.second << ")) ";
			std::cout << "Color: (" << std::get<0>(Rect_color) << ", " << std::get<1>(Rect_color) << ", " << std::get<2>(Rect_color) << ")" << std::endl;
		}
		break;
	case 'a':
		if (Rect_dir.size() < 10) {
			std::cout << "Creating Rect." << std::endl;
			float mid_x = static_cast<float>(uid_window(dre)) / Window_width;
			float mid_y = static_cast<float>(uid_window(dre)) / Window_height;

			float half_size = 0.1f;
			float x1 = mid_x - half_size, y1 = mid_y + half_size, x2 = mid_x + half_size, y2 = mid_y - half_size;
			float r = static_cast<float>(uid_color(dre)) / 255.0f, g = static_cast<float>(uid_color(dre)) / 255.0f, b = static_cast<float>(uid_color(dre)) / 255.0f;

			Rect_dir.push_back({ {x1, y1}, {x2, y2}, {r, g, b} });
		}
		else std::cout << "Maximum number of Rect reached." << std::endl;
		break;
	case 'q':
		std::cout << "Exiting program." << std::endl;
		exit(0);
	}
}

std::pair<float, float> ConvertMouseWxy2GLxy(int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	float ogl_x = (2.0f * x) / width - 1.0f;
	float ogl_y = 1.0f - (2.0f * y) / height;

	std::cout << "Mouse Clicked at (Window coords): (" << x << ", " << y << ") -> (OpenGL coords): (" << ogl_x << ", " << ogl_y << ")" << std::endl;

	return { ogl_x, ogl_y };
}

void UpdateRect() {

}