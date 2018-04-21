#pragma once
#include "loader.h"
#include "SDL_mixer.h"
#pragma comment(lib, "SDL2_mixer")

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
	short channel = 0;
public:
	AudioI();
	~AudioI();

	// Inherited via Audio
	void playMusic(Path, Loops) override;
	void playEffect(Path, Loops) override;
	void stopMusic() override;
	void stopAllEffects() override;
	void stopAllSound() override;
};