#pragma once
#include "game.h"

namespace ezGame {
#define START(G) int WINAPI WinMain(HINSTANCE,HINSTANCE ,PSTR ,int ) { ezGame::start(G); }
	extern  __declspec(dllexport) void start(const Game&);
}