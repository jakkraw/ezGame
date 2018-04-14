#pragma once
#include <memory>
#include <functional>

#include "interface\game.h"

#pragma warning( push )
#pragma warning( disable : 4250)

class StageFlow : public virtual Ez{
	using StagePtr = std::unique_ptr<Stage, std::function<void(Stage*)>>;
	StagePtr stage = nullptr, next = nullptr;

	Seconds delta() const
	{
		return 0.;
	}
	void goTo(Stage *&&) override
	{
		next.reset(stage);
	}
	virtual void update() override
	{
		if (next) {
			next->setup(*this);
			stage.reset(next.release());
		}

		stage->logic(*this);
	}

	
public:
	GameImpl(std::unique_ptr<Game>)
	{
		game->setup(*this);
		stage = StagePtr(game->start().release(), [&ez = *this](Stage* stage){ stage->cleanup(ez); delete stage; });
		stage->setup(*this);
	}
};

#pragma warning( pop ) 