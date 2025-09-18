#include "VarANDFunc_test04.h"

// Set the background color to dark gray, and draw the squares in random colors.
// Draw a square centered on the point where you click the mouse.Draw up to five squares.
// Keyboard input(commands can be capitalized or lowercase) :
//	1 : Change position 1 -> Squares move diagonally and bounce off walls, moving in the other direction / reinput : stopping.
//	2 : Change position 2 -> Squares move / stop in a zigzag pattern.
//	3 : Change size -> Squares change / stay the same size.
//	4 : Change color -> Squares randomly change / stay the same color.
//	5 : Follow animation -> Follow / stop the animation of a randomly selected square among the animated squares in the order they were drawn.For example,
//      if the second square is selected, the animation will follow the second square in the order 1 -> 3 -> 4 -> 5 -> ...
//	s : Stop all animations.
//	m : The squares move to their original green positions.

static int constexpr Window_width{ 700 }, Window_height{ 700 };
static std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>>> Rect_dir;
// Ani_Rect_dir: bounds1, bounds2, color, movement_vector, size_change_vector_pair(width, height)
static std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>, std::pair<float, float>, std::pair<float, float>>> Ani_Rect_dir;
auto seed = std::chrono::system_clock::now().time_since_epoch().count();

std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_int_distribution<int> uid_window(-Window_width, Window_width);
std::uniform_int_distribution<int> uid_color(0, 255);
std::uniform_real_distribution<float> uid_movment_vect(-1.0f, 1.0f);
std::uniform_real_distribution<float> uid_size_vect(0.001f, 0.005f);


void Timer_Animation(int value);
void Size_Change();

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

	if (position1_moving || position2_moving || size_changing || following_moving) {
		for (auto& ani_rect : Ani_Rect_dir) {
			auto Rect_color{ std::get<2>(ani_rect) };
			auto& Rect_Bounds1{ std::get<0>(ani_rect) };
			auto& Rect_Bounds2{ std::get<1>(ani_rect) };
			glColor3f(std::get<0>(Rect_color), std::get<1>(Rect_color), std::get<2>(Rect_color));
			glRectf(Rect_Bounds1.first, Rect_Bounds1.second, Rect_Bounds2.first, Rect_Bounds2.second);
		}
	}
	else {
		for (auto& rect : Rect_dir) {
			auto Rect_color{ std::get<2>(rect) };
			auto& Rect_Bounds1{ std::get<0>(rect) };
			auto& Rect_Bounds2{ std::get<1>(rect) };
			glColor3f(std::get<0>(Rect_color), std::get<1>(Rect_color), std::get<2>(Rect_color));
			glRectf(Rect_Bounds1.first, Rect_Bounds1.second, Rect_Bounds2.first, Rect_Bounds2.second);
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void MouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (Rect_dir.size() < 5) {
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
			Ani_Rect_dir.push_back({ {mouse_x - half_size, mouse_y + half_size}, {mouse_x + half_size, mouse_y - half_size}, Rect_color, {0.0f, 0.0f}, {uid_size_vect(dre), uid_size_vect(dre)} });
			std::cout << "Rect created at (" << mouse_x << ", " << mouse_y << ") with color ("
				<< std::get<0>(Rect_color) << ", " << std::get<1>(Rect_color) << ", " << std::get<2>(Rect_color) << ")" << std::endl;
		}
		else {
			std::cout << "Maximum number of rectangles reached." << std::endl;
		}
	}
}
void Keyboard(unsigned char key, int x, int y) {
	bool is_any_animation_running = position1_moving || position2_moving || size_changing || following_moving;

	switch (key) {
	case '1':
		if (!position2_moving && !following_moving) { // size_changing과는 동시 실행 허용
			position1_moving = !position1_moving;
			std::cout << "Position 1 moving: " << (position1_moving ? "ON" : "OFF") << std::endl;

			if (position1_moving) {
				if (last_key == 'm' || !is_any_animation_running) {
					for (auto& ani_rect : Ani_Rect_dir) {
						std::get<3>(ani_rect) = { uid_movment_vect(dre), uid_movment_vect(dre) };
					}
				}
				glutTimerFunc(10, Timer_Animation, 0);
			}
		}
		else {
			std::cout << "Cannot enable Position 1 moving while other animations are active." << std::endl;
		}

		break;
	case '2':
		if (!position1_moving && !following_moving) { // size_changing과는 동시 실행 허용
			position2_moving = !position2_moving;
			std::cout << "Position 2 moving: " << (position2_moving ? "ON" : "OFF") << std::endl;

			if (position2_moving) {
				if (last_key == 'm' || !is_any_animation_running) {
					// 위치는 0.5초마다 바뀌므로 초기 속도는 0으로 설정
					for (auto& ani_rect : Ani_Rect_dir) {
						std::get<3>(ani_rect) = { 0.0f, 0.0f };
					}
				}
				glutTimerFunc(500, Randomize_Movement_Vectors, 2);
				glutTimerFunc(10, Timer_Animation, 1);
			}
		}
		else {
			std::cout << "Cannot enable Position 2 moving while other animations are active." << std::endl;
		}

		break;
	case '3':
		if (!position1_moving && !position2_moving && !following_moving) { // 다른 애니메이션과 동시 실행 가능
			size_changing = !size_changing;
			std::cout << "Size changing: " << (size_changing ? "ON" : "OFF") << std::endl;

			if (size_changing && !is_any_animation_running) {
				glutTimerFunc(10, Timer_Animation, 3);
			}
		}
		else {
			std::cout << "Cannot enable Size changing while other animations are active." << std::endl;
		}
		break;
	case '4':
		for (int i = 0; i < Rect_dir.size(); ++i) {
			auto& Rect_color = std::get<2>(Rect_dir[i]);
			Rect_color = std::make_tuple(static_cast<float>(uid_color(dre)) / 255.0f,
				static_cast<float>(uid_color(dre)) / 255.0f,
				static_cast<float>(uid_color(dre)) / 255.0f);
		}
		std::cout << "All rectangle colors changed." << std::endl;
		break;
	case 's':
		position1_moving = false;
		position2_moving = false;
		size_changing = false;
		following_moving = false;

		for (auto& ani_rect : Ani_Rect_dir) {
			std::get<3>(ani_rect) = { 0.0f, 0.0f };
		}

		std::cout << "All animations stopped." << std::endl;
		break;
	case 'm':
		position1_moving = false;
		position2_moving = false;
		size_changing = false;
		following_moving = false;

		Ani_Rect_dir.clear();
		for (const auto& rect : Rect_dir) {
			Ani_Rect_dir.push_back({ std::get<0>(rect), std::get<1>(rect), std::get<2>(rect), {0.0f, 0.0f}, {uid_size_vect(dre), uid_size_vect(dre)} });
		}

		glutPostRedisplay();

		std::cout << "All animations stopped. Rectangles reset to original positions." << std::endl;
		break;
	case 'r':
		position1_moving = false;
		position2_moving = false;
		size_changing = false;
		following_moving = false;

		Rect_dir.clear();
		Ani_Rect_dir.clear();

		std::cout << "All rectangles cleared." << std::endl;
		break;
	case 'q':
		exit(0);
	}
	last_key = key;
}

std::pair<float, float> ConvertMouseWxy2GLxy(int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	float ogl_x = (2.0f * x) / width - 1.0f;
	float ogl_y = 1.0f - (2.0f * y) / height;

	return { ogl_x, ogl_y };
}

void Timer_Animation(int value) {
	bool is_any_animation_running = false;

	if (position1_moving) {
		Position1_Move();
		is_any_animation_running = true;
	}
	else if (position2_moving) {
		Position2_Move();
		is_any_animation_running = true;
	}

	if (size_changing) {
		Size_Change();
		is_any_animation_running = true;
	}

	if (is_any_animation_running) {
		glutPostRedisplay();
		glutTimerFunc(10, Timer_Animation, 0); // value는 더 이상 중요하지 않음
	}
}

void Position1_Move() {
	for (int i = 0; i < Ani_Rect_dir.size(); ++i) {
		auto& Rect_Bounds1 = std::get<0>(Ani_Rect_dir[i]);
		auto& Rect_Bounds2 = std::get<1>(Ani_Rect_dir[i]);
		auto& Movement_Vector = std::get<3>(Ani_Rect_dir[i]);

		Rect_Bounds1.first += Movement_Vector.first * moving_amount;
		Rect_Bounds1.second += Movement_Vector.second * moving_amount;
		Rect_Bounds2.first += Movement_Vector.first * moving_amount;
		Rect_Bounds2.second += Movement_Vector.second * moving_amount;

		if (Rect_Bounds1.first < -1.0f || Rect_Bounds2.first > 1.0f) {
			Movement_Vector.first = -Movement_Vector.first;
		}
		if (Rect_Bounds1.second > 1.0f || Rect_Bounds2.second < -1.0f) {
			Movement_Vector.second = -Movement_Vector.second;
		}
	}
}

void Position2_Move() {
	Position1_Move();
}

void Size_Change() {
	for (auto& ani_rect : Ani_Rect_dir) {
		auto& Rect_Bounds1 = std::get<0>(ani_rect);
		auto& Rect_Bounds2 = std::get<1>(ani_rect);
		auto& size_vect_pair = std::get<4>(ani_rect);

		// 너비 변경
		Rect_Bounds1.first -= size_vect_pair.first;
		Rect_Bounds2.first += size_vect_pair.first;

		// 높이 변경
		Rect_Bounds1.second += size_vect_pair.second;
		Rect_Bounds2.second -= size_vect_pair.second;

		float width = Rect_Bounds2.first - Rect_Bounds1.first;
		if (width > 0.5f || width < 0.05f) {
			size_vect_pair.first = -size_vect_pair.first;
		}

		float height = Rect_Bounds1.second - Rect_Bounds2.second;
		if (height > 0.5f || height < 0.05f) {
			size_vect_pair.second = -size_vect_pair.second;
		}
	}
}

void Randomize_Movement_Vectors(int value) {
	if (position2_moving) {
		for (auto& ani_rect : Ani_Rect_dir) {
			std::get<3>(ani_rect) = { uid_movment_vect(dre), uid_movment_vect(dre) };
		}
		glutTimerFunc(500, Randomize_Movement_Vectors, 2);
	}
}