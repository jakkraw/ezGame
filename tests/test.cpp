#pragma once
#include <string>
#include "../source/interface/ezGame.h"
#include "../source/interface/drawView.h"
#pragma comment(lib, "ezGame")
using namespace ezGame;

struct Stage2;
struct Stage1 : Stage {
	void logic(Ez& ez) override {
		if (ez.key(Keyboard::N2).pressed) return ez.moveTo<Stage2>();
		if (ez.key(Keyboard::Q).pressed) return ez.quit();
		ez.drawImage("avatar.png", { ez.cursorRelative(), { 0.5, 0.5 } });
		ez.drawText("Delta: " + Text(ez.delta()), RelativeArea{ {0,0}, {0.40, 0.10} }, WHITE);
	}
};

struct Stage2 : Stage {
	void logic(Ez& ez) override {
		if (ez.key(Mouse::LEFT).down) ez.drawImage("avatar.png", RelativeArea{ {0.5, 0.5}, {0.25, 0.25} });
		if (ez.key(Keyboard::N1).pressed) return ez.moveTo<Stage1>();
		if (ez.key(Keyboard::Q).pressed) return ez.quit();	
		if (ez.key(Keyboard::M).pressed) ez.interfaces().audio().playMusic("music.mp3");
		ez.drawRectangle({ {0, 0}, Size(ez.cursor().x, ez.cursor().y) }, RED);

		//auto menu = DrawView(ez.interfaces().draw());
	}
};

struct TestGame : Game{
	EntryStage entry() const override { return Entry<Stage1>(); }
};


#include <Windows.h>
int WINAPI WinMain(HINSTANCE,HINSTANCE ,PSTR ,int ) {
	start(TestGame());
}