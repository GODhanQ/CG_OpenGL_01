#include "VarANDFunc_test03.h"

// Rect_dir : Bounds1, Bounds2, Color, origin
static std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>, std::pair<float, float>>> Rect_dir;
static constexpr int Window_width{ 700 }, Window_height{ 700 };
static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_real_distribution<float> uid_color(0.0f, 1.0f);
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

	float half_size_X = (std::get<1>(Rect_dir[DraggingRectIndex]).first - std::get<0>(Rect_dir[DraggingRectIndex]).first) / 2.0f;
	float half_size_Y = (std::get<0>(Rect_dir[DraggingRectIndex]).second - std::get<1>(Rect_dir[DraggingRectIndex]).second) / 2.0f;

	std::get<0>(Rect_dir[DraggingRectIndex]).first = mouse_x - half_size_X;
	std::get<1>(Rect_dir[DraggingRectIndex]).first = mouse_x + half_size_X;
	std::get<0>(Rect_dir[DraggingRectIndex]).second = mouse_y + half_size_Y;
	std::get<1>(Rect_dir[DraggingRectIndex]).second = mouse_y - half_size_Y;



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

			if (DraggingRectIndex != -1) {
				auto& DraggedRect_Bounds1 = std::get<0>(Rect_dir[DraggingRectIndex]);
				auto& DraggedRect_Bounds2 = std::get<1>(Rect_dir[DraggingRectIndex]);
				bool overlapped = false;

				for (int i = 0; i < Rect_dir.size(); ++i) {
					if (i == DraggingRectIndex) continue;
					auto& OtherRect_Bounds1 = std::get<0>(Rect_dir[i]);
					auto& OtherRect_Bounds2 = std::get<1>(Rect_dir[i]);
					if (!(DraggedRect_Bounds2.first < OtherRect_Bounds1.first || DraggedRect_Bounds1.first > OtherRect_Bounds2.first ||
						DraggedRect_Bounds2.second > OtherRect_Bounds1.second || DraggedRect_Bounds1.second < OtherRect_Bounds2.second)) {
						overlapped = true;
						std::cout << "Rect " << DraggingRectIndex << " overlapped with Rect " << i << ". Hiding both and creating a new big rect." << std::endl;

						float new_x1 = std::min(DraggedRect_Bounds1.first, OtherRect_Bounds1.first);
						float new_y1 = std::max(DraggedRect_Bounds1.second, OtherRect_Bounds1.second);
						float new_x2 = std::max(DraggedRect_Bounds2.first, OtherRect_Bounds2.first);
						float new_y2 = std::min(DraggedRect_Bounds2.second, OtherRect_Bounds2.second);
						float r = uid_color(dre), g = uid_color(dre), b = uid_color(dre);
						float mid_x = (new_x1 + new_x2) / 2.0f, mid_y = (new_y1 + new_y2) / 2.0f;
						Rect_dir.push_back({ {new_x1, new_y1}, {new_x2, new_y2}, {r, g, b}, {mid_x, mid_y} });

						if (i < DraggingRectIndex) {
							Rect_dir.erase(Rect_dir.begin() + DraggingRectIndex);
							Rect_dir.erase(Rect_dir.begin() + i);
						}
						else {
							Rect_dir.erase(Rect_dir.begin() + i);
							Rect_dir.erase(Rect_dir.begin() + DraggingRectIndex);
						}
						break;
					}
				}
			}

			DraggingRectIndex = -1;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < Rect_dir.size(); ++i) {
			auto& Rect = Rect_dir[i];
			auto& Rect_Bounds1 = std::get<0>(Rect);
			auto& Rect_Bounds2 = std::get<1>(Rect);
			auto& Rect_color = std::get<2>(Rect);

			auto ogl_xy = ConvertMouseWxy2GLxy(x, y);
			float mouse_x = ogl_xy.first;
			float mouse_y = ogl_xy.second;

			if (mouse_x >= Rect_Bounds1.first && mouse_x <= Rect_Bounds2.first &&
				mouse_y >= Rect_Bounds2.second && mouse_y <= Rect_Bounds1.second) {
				std::cout << "Erased Rect " << i << "." << std::endl;

				std::uniform_real_distribution<float> urd_SmallRect_origin_X(Rect_Bounds1.first, Rect_Bounds2.first);
				std::uniform_real_distribution<float> urd_SmallRect_origin_Y(Rect_Bounds2.second, Rect_Bounds1.second);
				std::uniform_real_distribution<float> urd_Color(0.0f, 1.0f);

				for (int i = 0; i < 2; ++i) {
					float mid_x = urd_SmallRect_origin_X(dre);
					float mid_y = urd_SmallRect_origin_Y(dre);

					float max_half_X = std::min(mid_x - Rect_Bounds1.first, Rect_Bounds2.first - mid_x);
					std::uniform_real_distribution<float> urd_SmallRect_half_X(0.05f, max_half_X);

					float max_half_Y = std::min(mid_y - Rect_Bounds2.second, Rect_Bounds1.second - mid_y);
					std::uniform_real_distribution<float> urd_SmallRect_half_Y(0.05f, max_half_Y);

					float half_size_X = urd_SmallRect_half_X(dre);
					float half_size_Y = urd_SmallRect_half_Y(dre);
					float x1 = mid_x - half_size_X, y1 = mid_y + half_size_Y, x2 = mid_x + half_size_X, y2 = mid_y - half_size_Y;
					float r = urd_Color(dre), g = urd_Color(dre), b = urd_Color(dre);

					Rect_dir.push_back({ {x1, y1}, {x2, y2}, {r, g, b}, {mid_x, mid_y} });
				}
				
				Rect_dir.erase(Rect_dir.begin() + i);
				break;
			}
		}

		glutPostRedisplay();
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
		if (Rect_dir.size() < 30) {
			std::cout << "Creating Rect." << std::endl;
			float mid_x = static_cast<float>(uid_window(dre)) / Window_width;
			float mid_y = static_cast<float>(uid_window(dre)) / Window_height;

			std::uniform_real_distribution<float> uid_half_size_X(0.05f, 0.2f);
			std::uniform_real_distribution<float> uid_half_size_Y(0.05f, 0.2f);
			float half_size_X = uid_half_size_X(dre);
			float half_size_Y = uid_half_size_Y(dre);
			float x1 = mid_x - half_size_X, y1 = mid_y + half_size_Y, x2 = mid_x + half_size_X, y2 = mid_y - half_size_Y;
			float r = uid_color(dre), g = uid_color(dre), b = uid_color(dre);

			Rect_dir.push_back({ {x1, y1}, {x2, y2}, {r, g, b}, {mid_x, mid_y} });
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
