#include "interface/ezGame.h"

#include "draw.h"
#include "input.h"
#include "audio.h"
#include "window.h"
#include "flow.h"
#include "timer.h"
#include "ez.h"

void ezGame::start(const Game& game)
{
	WindowI window("ezGame", WindowI::Resolution(640, 480), Size{ 640, 480 });
	DrawI draw(window.renderer());
	AudioI audio;
	InputI input;
	SettingsI settings(window);
	
	game.setup(settings);

	Flow flow(game.createEntry(), settings);
	Timer timer;

	while (window.isOpen()) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			input.update(event);
			window.update(event);
		}

		flow.logic(::EzI(timer, flow, draw, input, audio, window, settings));

		timer.reset();
		draw.reset();
		input.reset();
	}
}