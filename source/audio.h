#pragma once
#include "loader.h"
#include "SDL_mixer.h"
#pragma comment(lib, "SDL2_mixer")

struct MusicWrapper {
	Mix_Music* data;
	MusicWrapper(MusicWrapper&& m) : data(std::move(m.data)) { m.data = nullptr; }
	MusicWrapper(const char* path) : data(Mix_LoadMUS(path)) {}
	~MusicWrapper() { Mix_FreeMusic(data); }
};

struct ChunkWrapper {
	Mix_Chunk* data;
	ChunkWrapper(ChunkWrapper&& c) : data(std::move(c.data)) { c.data = nullptr; }
	ChunkWrapper(const char* path) : data(Mix_LoadWAV(path)) {}
	~ChunkWrapper() { Mix_FreeChunk(data); }
};


#include "interface\audio.h"
using namespace ezGame;
class Audio : public ezGame::Audio {
	Loader<MusicWrapper> music;
	Loader<ChunkWrapper> chunk;
	short channel = 0;
public:
	Audio() {
		Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG);
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	}

	~Audio() {
		Mix_CloseAudio();
		Mix_Quit();
	}

	// Inherited via Audio
	virtual void play(const Music &m) override {
		Mix_PlayMusic(music[m.cPath()].data, m.loopTimes == LoopTimes::Infinite ? -1 : 0);
	}
	virtual void play(const Effect &e) override {
		channel = channel++ % 16;
		Mix_PlayChannel(channel, chunk[e.cPath()].data, e.loopTimes == LoopTimes::Infinite ? -1 : 0);
	}
	virtual void stopMusic() override {
		Mix_HaltMusic();
	}
	virtual void stopAllEffects() override {
		Mix_HaltChannel(-1);
	}
	virtual void stopAllSound() override {
		stopMusic();
		stopAllEffects();
	}
};