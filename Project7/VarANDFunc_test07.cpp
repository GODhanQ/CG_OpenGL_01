#ifndef VARANDFUNC_TEST07_H
#endif VARANDFUNC_TEST07_H

#include "VarANDFunc_test07.h"

float BackGround_CR{ 0.8f }, BackGround_CG{ 0.8f }, BackGround_CB{ 0.8f }, BackGround_CA{ 1.0 };
float const margin_size{ 0.025f };
int Dragging_Rect_Index{ -1 };
bool Auto_Solving_Flag{ false };

std::pair<std::pair<float, float>, std::pair<float, float>> const Bounds_Bundle[12] = {
	{ { -0.7125f, 0.9f }, { -0.6875f, 0.7f } },
	{ { -0.90f, 0.7f }, { -0.50f, 0.5f } },
	{ { -0.825f, 0.5f }, { -0.775f, 0.1f } },
	{ { -0.625f, 0.5f }, { -0.575f, 0.1f } },
	{ { -0.925f, 0.1f }, { -0.475f, -0.1f } },
	{ { -0.85f, -0.1f }, { -0.80f, -0.7f } },
	{ { -0.60f, -0.1f }, { -0.55f, -0.7f } },
	{ { -0.925f, -0.7f }, { -0.475f, -0.9f } },
	{ { -0.3f, 0.7f }, { -0.2f, 0.5f } },
	{ { -0.35f, 0.5f }, { -0.15f, 0.3f } },
	{ { -0.45f, 0.3f }, { -0.05f, -0.1f } },
	{ { -0.3f, -0.1f }, { -0.2f, -0.9f } }
};

//static int GetBoundsBundleSize() {
//	return sizeof(Bounds_Bundle) / sizeof(Bounds_Bundle[0]);
//}