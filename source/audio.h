#pragma once
#include "loader.h"
#include "sdl2.h"
struct MusicWrapper {
	Mix_Music* data;
	MusicWrapper(MusicWrapper&& m) : data(m.data) { m.data = nullptr; }
	MusicWrapper(const char* path) : data(Mix_LoadMUS(path)) {}
	~MusicWrapper() { Mix_FreeMusic(data); }
};

struct ChunkWrapper {
	Mix_Chunk* data;
	ChunkWrapper(ChunkWrapper&& c) : data(c.data) { c.data = nullptr; }
	ChunkWrapper(const char* path) : data(Mix_LoadWAV(path)) {}
	~ChunkWrapper() { Mix_FreeChunk(data); }
};

#include "interface/audio.h"
class AudioI : public ezGame::Audio {
	Loader<MusicWrapper> music;
	Loader<ChunkWrapper> chunk;
public:
	void playMusic(Path path, Loops l) override{
		Mix_PlayMusic(music[path].data, (int)l);
	}

	void playEffect(Path path, Loops l) override{
		Mix_PlayChannel(-1, chunk[path].data, (int)l);
	}

	void stopMusic() override{
		Mix_HaltMusic();
	}

	void stopAllEffects() override{
		Mix_HaltChannel(-1);
	}

	void stopAllSound() override{
		stopMusic();
		stopAllEffects();
	}
};