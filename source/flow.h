#pragma once
#include "interface/game.h" 

#include <memory>
struct Flow {
	using Stage = ezGame::Stage;
	using Settings = ezGame::Settings;

	using StagePtr = std::unique_ptr<Stage>;
	StagePtr stage, next = nullptr;
	Flow(Stage* entry, Settings&) : stage(entry) {}

	void moveTo(StagePtr next) {
		this->next = std::move(next);
	}

	void logic(ezGame::Ez& ez) {
		stage->logic(ez);
		if (next) {
			stage = std::move(next);
			stage->setup(ez.settings());
		}
	}
};