#include "interface/extern.h"
#include "gameSDL.h"

void ezGame::start(Game*&& game)
{
	GameSDL(std::unique_ptr<Game>(game)).run();
}