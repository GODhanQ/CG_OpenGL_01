#ifndef VARANDFUNC_TEST04_H
#endif VARANDFUNC_TEST04_H

#include "VarANDFunc_test04.h"

float moving_amount{ 0.01f };
float BackGround_CR{ 0.5f }, BackGround_CG{ 0.5f }, BackGround_CB{ 0.5f }, BackGround_CA{ 1.0f };
unsigned char last_key{ '\0' };
bool position1_moving{ false }, position2_moving{ false }, size_changing{ false }, following_moving{ false };