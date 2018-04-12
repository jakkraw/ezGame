#pragma once
#include <memory>
#include <functional>

#include "interface\ez.h"
#include "interface\game.h"
#include "windowSDL.h"

#pragma warning( push )
#pragma warning( disable : 4250)

class GameImpl : public virtual Ez{
	using StagePtr = std::unique_ptr<Stage, std::function<void(Stage*)>>;
	StagePtr stage = nullptr, next = nullptr;

	Seconds delta() const;
	void goTo(Stage *&&) override;
	virtual void update() override;

	
public:
	GameImpl(std::unique_ptr<Game>);
};

#pragma warning( pop ) 