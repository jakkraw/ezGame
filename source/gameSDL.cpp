#include "gameSDL.h"
#include <functional>



GameSDL::GameSDL(Game*&& game, SDLData & gameData) : 
	WindowSDL(gameData), 
	game(game), 
	stage(game->start().release(), [&ez = *this](Stage* stage){stage->cleanup(ez); delete stage;  }) 
{
	this->game->setup(*this);
	this->stage->setup(*this);
}

Seconds GameSDL::delta() const
{
	return 0.;
}

void GameSDL::goTo(Stage *&& stage)
{
	next.reset(stage);
}

void GameSDL::update()
{
	WindowSDL::update();

	if (next) {
		next->setup(*this);
		stage.reset(next.release());
	}

	stage->logic(*this);	
}
