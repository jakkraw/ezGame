#pragma once
#include "../source/interface/ezGame.h"
#pragma comment(lib, "ezGame")
using namespace ezGame;

struct Stage1 : public Stage {
	void logic(Ez&) override;
	void setup(Ez& ez) override {
		ez.settings().set(Window::Type::Windowed);

		auto& settings = ez.settings();
		settings.set(Title("Stage1"));
		auto a1 = settings.cursorVisibility();
		auto a2 = settings.background();
		auto a3 = settings.title();
		auto a4 = settings.windowType();

	}

	~Stage1() {
		int j = 0;
	}
};

struct Stage2 : public Stage {
	void setup(Ez& ez) override {
		auto& settings = ez.settings();
		settings.set(Title("Stage2"));
		settings.set(Background(RED));
		//settings.set(Window::Type::Fullscreen);

		auto a1 = settings.cursorVisibility();
		auto a2 = settings.background();
		auto a3 = settings.title();
		auto a4 = settings.windowType();

	}

	void cleanup(Ez& ez) override {
		ez.stopAllSound();
		ez.settings().set(Background(BLACK));
	}

	void logic(Ez& ez) override {
		ez.draw(ezGame::Rectangle(Position(0, 0), Size(ez.cursor().x, ez.cursor().y), RED));
		
		if (ez.down(Mouse::MIDDLE))
			ez.draw(Image("avatar.png", RelativePosition(0.5, 0.5, ez.resolution()), RelativeSize(0.25, 0.25, ez.resolution())));

		if (ez.pressed(Keyboard::N1))
			return ez.goTo(new Stage1);

		if (ez.down(Keyboard::Q))
			return ez.quit();

		if (ez.down(Keyboard::M))
			ez.play(Music("music.mp3"));


		auto text = Text("ezGame, Stage 2 , press \"1\" to go back", Position(0, 0), Size(600, 30), BLACK);
		ez.draw(Text("ezGame, Stage 2 , press \"1\" to go back", Position(0, 0), Size(600, 30), BLACK));

		ez.draw(Text("Stage 2", ez.cursor(), Size(600, 30), BLACK));	
	}

};


void Stage1::logic(Ez & ez) {

	if (ez.pressed(Keyboard::N2))
		 return ez.goTo(new Stage2);


	if (ez.down(Keyboard::Q))
		return ez.quit();
	
	ez.draw(Image("avatar.png", Position(ez.cursor()), Size(30,30)));
	ez.draw(Text("ezGame, STAGE 1", Position(0, 0), Size(600, 30), WHITE));
}

struct Test : public Game {

	void setup(Ez& ez) {
		ez.settings().set(Title("Elo Morelo."));
		ez.settings().set(Resolution(800, 600));
	}
	// Inherited via ez
	std::unique_ptr<Stage> start() override {
		return std::make_unique<Stage1>();
	}

	~Test() {
		int i = 0;
	}

};

#include <Windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
	start(new Test);
}