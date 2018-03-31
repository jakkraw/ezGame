#pragma once
#include <memory>
#include <functional>

#include "interface\ez.h"
#include "interface\game.h"
#include "windowSDL.h"

#pragma warning( push )
#pragma warning( disable : 4250)

class GameSDL : public virtual Ez, public WindowSDL {
	using StagePtr = std::unique_ptr<Stage, std::function<void(Stage*)>>;
	StagePtr stage = nullptr, next = nullptr;

	Seconds delta() const;
	void goTo(Stage *&&) override;
	virtual void update() override;

	
public:
	GameSDL(std::unique_ptr<Game>);
	void run();
};

#pragma warning( pop ) 