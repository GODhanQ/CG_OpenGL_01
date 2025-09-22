#include "VarANDFunc_test07.h"

//Matching Shape Blocks
// Draw a block board on the left side of the screen.
// Draw a board with all squares connected using squares of various sizes.
// Use at least 10 squares.
// Use a variety of colors.
// Squares to be placed on the board are scattered on the right side of the screen.
// Select a square on the right and drag it to align it with the board on the left.
// If the shape matches the board, a message will be written to the console indicating that it has been matched.It's difficult to align the shape exactly, so if it's within a certain area, it's considered a match.
// Keyboard Commands
// r : Reset and redraw
// q : Exit the program
// Bonus : Pressing a keyboard command will automatically move the shapes one by one to their correct positions.

static const int Window_width{ 1200 }, Window_height{ 900 };
auto seed = std::chrono::system_clock::now().time_since_epoch().count();
int Bundle_size = std::extent<decltype(Bounds_Bundle)>::value;

// Rect_dir : Bounds1, Bounds2, Color, Origin, Half Size, Move_vec
std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>,
	std::pair<float, float>, std::pair<float, float>, std::pair<float, float>>> Rect_dir;
// Rect_Board : Bounds1, Bounds2, Color, Origin
std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>, std::pair<float, float>>> Rect_Board;
// Placed_Rect : Bounds1, Bounds2, Color
std::vector<std::tuple<std::pair<float, float>, std::pair<float, float>, std::tuple<float, float, float>>> Placed_Rect;

std::default_random_engine dre(static_cast<unsigned int>(seed));
std::uniform_real_distribution<float> urd_window(-1.0f, 1.0f);
std::uniform_real_distribution<float> urd_left_window(-1.0f, 0.0f);
std::uniform_real_distribution<float> urd_right_window(0.0f, 1.0f);
std::uniform_real_distribution<float> uid_color(0.0f, 1.0f);

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(60, 60);
	glutInitWindowSize(Window_width, Window_height);
	glutCreateWindow("Example7");
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

	if (Rect_Board.empty()) {
		if (!Placed_Rect.empty()) {
			// cleared game
			Rect_dir.clear();
			Rect_Board.clear();
			Placed_Rect.clear();
			std::cout << "Game Cleared!" << std::endl;
			Sleep(1000);
			glutPostRedisplay();
			return;
		}
		CreateBoard();
	}
	for (int i = 0; i < Rect_Board.size(); ++i) {
		auto& Bounds1 = std::get<0>(Rect_Board[i]);
		auto& Bounds2 = std::get<1>(Rect_Board[i]);
		auto& Color = std::get<2>(Rect_Board[i]);
		glColor3f(std::get<0>(Color), std::get<1>(Color), std::get<2>(Color));
		glLineWidth(3.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2f(Bounds1.first, Bounds1.second);
		glVertex2f(Bounds2.first, Bounds1.second);
		glVertex2f(Bounds2.first, Bounds2.second);
		glVertex2f(Bounds1.first, Bounds2.second);
		glEnd();
	}
	for (int i = 0; i < Rect_dir.size(); ++i) {
		if (Auto_Solving_Flag) {
			auto& Move_vec = std::get<5>(Rect_dir[i]);
			auto& Origin = std::get<3>(Rect_dir[i]);
			Origin.first += Move_vec.first;
			Origin.second += Move_vec.second;

			auto& Bounds1 = std::get<0>(Rect_dir[i]);
			auto& Bounds2 = std::get<1>(Rect_dir[i]);
			auto& Half_size = std::get<4>(Rect_dir[i]);
			float half_size_X = Half_size.first;
			float half_size_Y = Half_size.second;
			Bounds1.first = Origin.first - half_size_X;
			Bounds2.first = Origin.first + half_size_X;
			Bounds1.second = Origin.second - half_size_Y;
			Bounds2.second = Origin.second + half_size_Y;

			auto& Board_Rect = Rect_Board[i];
			auto& Board_Origin = std::get<3>(Board_Rect);
			auto& Board_Bounds1 = std::get<0>(Board_Rect);
			auto& Board_Bounds2 = std::get<1>(Board_Rect);

			auto& Rect_Color = std::get<2>(Rect_dir[i]);
			if (std::abs(Origin.first - Board_Origin.first) < margin_size &&
				std::abs(Origin.second - Board_Origin.second) < margin_size) {
				std::cout << "Auto Matched Rect " << i << "!" << std::endl;

				Placed_Rect.push_back({ Board_Bounds1, Board_Bounds2, Rect_Color });
				Rect_dir.erase(Rect_dir.begin() + i);
				Rect_Board.erase(Rect_Board.begin() + i);
				--i;
				glutPostRedisplay();
			}
			if (Rect_dir.empty()) Auto_Solving_Flag = false;

			glutPostRedisplay();
		}
		auto& Bounds1 = std::get<0>(Rect_dir[i]);
		auto& Bounds2 = std::get<1>(Rect_dir[i]);
		auto& Color = std::get<2>(Rect_dir[i]);

		glColor3f(std::get<0>(Color), std::get<1>(Color), std::get<2>(Color));
		glRectf(Bounds1.first, Bounds1.second, Bounds2.first, Bounds2.second);
	}
	for (int i = 0; i < Placed_Rect.size(); ++i) {
		auto& Bounds1 = std::get<0>(Placed_Rect[i]);
		auto& Bounds2 = std::get<1>(Placed_Rect[i]);
		auto& Color = std::get<2>(Placed_Rect[i]);
		glColor3f(std::get<0>(Color), std::get<1>(Color), std::get<2>(Color));
		glRectf(Bounds1.first, Bounds1.second, Bounds2.first, Bounds2.second);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void MouseDrag(int x, int y) {
	if (Dragging_Rect_Index == -1 || Auto_Solving_Flag) return;
	auto ogl_xy = ConvertMouseWxy2GLxy(x, y);
	float mouse_x = ogl_xy.first;
	float mouse_y = ogl_xy.second;

	auto& half_size = std::get<4>(Rect_dir[Dragging_Rect_Index]);
	float half_size_X = half_size.first;
	float half_size_Y = half_size.second;

	std::get<0>(Rect_dir[Dragging_Rect_Index]).first = mouse_x - half_size_X;
	std::get<1>(Rect_dir[Dragging_Rect_Index]).first = mouse_x + half_size_X;
	std::get<0>(Rect_dir[Dragging_Rect_Index]).second = mouse_y - half_size_Y;
	std::get<1>(Rect_dir[Dragging_Rect_Index]).second = mouse_y + half_size_Y;
	
	std::get<3>(Rect_dir[Dragging_Rect_Index]).first = mouse_x;
	std::get<3>(Rect_dir[Dragging_Rect_Index]).second = mouse_y;

	glutPostRedisplay();
}

void MouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		auto ogl_xy = ConvertMouseWxy2GLxy(x, y);
		float mouse_x = ogl_xy.first;
		float mouse_y = ogl_xy.second;

		if (state == GLUT_DOWN) {
			std::cout << "Mouse Down at (" << mouse_x << ", " << mouse_y << ")" << std::endl;
			Dragging_Rect_Index = -1;

			for (int i = Rect_dir.size() - 1; i >= 0; --i) {
				auto& Rect_Bounds1 = std::get<0>(Rect_dir[i]);
				auto& Rect_Bounds2 = std::get<1>(Rect_dir[i]);

				// Y좌표 클릭 감지 로직 수정: bottom <= y <= top
				if (mouse_x >= Rect_Bounds1.first && mouse_x <= Rect_Bounds2.first &&
					mouse_y >= Rect_Bounds1.second && mouse_y <= Rect_Bounds2.second) {
					Dragging_Rect_Index = i;
					std::cout << "Selected Rect " << i << " for dragging." << std::endl;
					break;
				}
			}
		}
		else if (state == GLUT_UP) {
			if (Dragging_Rect_Index != -1) {
				std::cout << "Finished dragging Rect " << Dragging_Rect_Index << "." << std::endl;
			}

			auto& Dragged_Rect = Rect_dir[Dragging_Rect_Index];
			auto& Dragged_Bounds1 = std::get<0>(Dragged_Rect);
			auto& Dragged_Bounds2 = std::get<1>(Dragged_Rect);
			auto& Dragged_Color = std::get<2>(Dragged_Rect);

			// the dragged rect index as same as the board rect index
			auto& Board_Rect = Rect_Board[Dragging_Rect_Index];
			auto& Board_Bounds1 = std::get<0>(Board_Rect);
			auto& Board_Bounds2 = std::get<1>(Board_Rect);

			if (mouse_x >= Board_Bounds1.first - margin_size && mouse_x <= Board_Bounds2.first + margin_size &&
				mouse_y <= Board_Bounds1.second + margin_size && mouse_y >= Board_Bounds2.second - margin_size) {
				std::cout << "Matched Rect " << Dragging_Rect_Index << "!" << std::endl;
				Placed_Rect.push_back({ Board_Bounds1, Board_Bounds2, Dragged_Color });

				Rect_dir.erase(Rect_dir.begin() + Dragging_Rect_Index);
				Rect_Board.erase(Rect_Board.begin() + Dragging_Rect_Index);
			}
			else {
				std::cout << "Not Matched Rect " << Dragging_Rect_Index << "." << std::endl;
			}

			Dragging_Rect_Index = -1;
			glutPostRedisplay();
		}
	}
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		std::cout << "Showing automatic solving." << std::endl;

		Auto_Solving_Flag = !Auto_Solving_Flag;
		ShowAnswer();

		glutPostRedisplay();
		break;
	case 'r':
		std::cout << "resetting the screen." << std::endl;
		Rect_dir.clear();
		Rect_Board.clear();

		CreateBoard();

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

void CreateBoard() {
	for(int i = 0; i < Bundle_size; ++i) {
		auto& Bounds1 = std::get<0>(Bounds_Bundle[i]);
		auto& Bounds2 = std::get<1>(Bounds_Bundle[i]);
		auto Color = std::make_tuple(uid_color(dre), uid_color(dre), uid_color(dre));
		float board_origin_x = Bounds1.first + (Bounds2.first - Bounds1.first) / 2.0f;
		float board_origin_y = Bounds1.second + (Bounds2.second - Bounds1.second) / 2.0f;
		Rect_Board.push_back({ Bounds1, Bounds2, Color, {board_origin_x, board_origin_y} });

		float origin_x = urd_right_window(dre);
		float origin_y = urd_window(dre);
		float half_size_x = (Bounds2.first - Bounds1.first) / 2.0f;
		float half_size_y = std::abs((Bounds2.second - Bounds1.second) / 2.0f);

		float Rect_Bounds1_x = origin_x - half_size_x;
		float Rect_Bounds1_y = origin_y - half_size_y;
		float Rect_Bounds2_x = origin_x + half_size_x;
		float Rect_Bounds2_y = origin_y + half_size_y;

		Rect_dir.push_back({ {Rect_Bounds1_x, Rect_Bounds1_y}, {Rect_Bounds2_x, Rect_Bounds2_y}, Color,
			{origin_x, origin_y}, {half_size_x, half_size_y}, {0.0f, 0.0f} });
	}
}

void ShowAnswer() {
	// flag true
	// calculate move vector for each rect
	// move rect origin to the board origin -> not in showAnswer()
	// if arrived, place the rect to the board
	for (int i = 0; i < Rect_dir.size(); ++i) {
		auto& Dragged_Rect = Rect_dir[i];
		auto& Dragged_Origin = std::get<3>(Dragged_Rect);
		auto& Board_Rect = Rect_Board[i];
		auto& Board_Origin = std::get<3>(Board_Rect);
		float move_vec_x = (Board_Origin.first - Dragged_Origin.first) / 50.0f;
		float move_vec_y = (Board_Origin.second - Dragged_Origin.second) / 50.0f;

		// set move vector set slowly
		float length = std::sqrt(move_vec_x * move_vec_x + move_vec_y * move_vec_y);
		if (length != 0.0f) {
			move_vec_x = (move_vec_x / length) * 0.02f;
			move_vec_y = (move_vec_y / length) * 0.02f;
		}

		std::get<5>(Dragged_Rect).first = move_vec_x;
		std::get<5>(Dragged_Rect).second = move_vec_y;
	}
}