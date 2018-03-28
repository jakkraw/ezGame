#pragma once
#include <memory>
#include <functional>

#include "interface\ez.h"
#include "interface\game.h"
#include "windowSDL.h"

#pragma warning( push )
#pragma warning( disable : 4250)

class GameSDL : public virtual Ez, public WindowSDL {
	std::unique_ptr<Game> game;
	std::unique_ptr<Stage, std::function<void(Stage*)>> stage, next = nullptr;
	
public:
	GameSDL(Game*&& game, SDLData& gameData);

	Seconds delta() const;
	void goTo(Stage *&&) override;
	virtual void update() override;
};

#pragma warning( pop ) 