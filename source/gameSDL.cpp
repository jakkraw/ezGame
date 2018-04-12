#include "gameSDL.h"
#include <functional>



GameImpl::GameImpl(std::unique_ptr<Game> game)
{
	game->setup(*this);
	stage = StagePtr(game->start().release(), [&ez = *this](Stage* stage){ stage->cleanup(ez); delete stage; });
	stage->setup(*this);
}

Seconds GameImpl::delta() const
{
	return 0.;
}

void GameImpl::goTo(Stage *&& stage)
{
	next.reset(stage);
}

void GameImpl::update()
{
	if (next) {
		next->setup(*this);
		stage.reset(next.release());
	}

	stage->logic(*this);	
}
