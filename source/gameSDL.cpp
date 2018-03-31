#include "gameSDL.h"
#include <functional>



GameSDL::GameSDL(std::unique_ptr<Game> game) : WindowSDL()
{
	game->setup(*this);
	stage = StagePtr(game->start().release(), [&ez = *this](Stage* stage){ stage->cleanup(ez); delete stage; });
	stage->setup(*this);
}

void GameSDL::run()
{
	while (isRunning())
		update();
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
