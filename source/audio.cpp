#include "audio.h"

using namespace ezGame;

AudioI::AudioI() {
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
}

AudioI::~AudioI() {
	music.clear();
	chunk.clear();
	Mix_CloseAudio();
	Mix_Quit();
}

void AudioI::playMusic(Path path, Loops l) {
	Mix_PlayMusic(music[path].data, l == Loops::INFINITE ? -1 : 0);
}

void AudioI::playEffect(Path path, Loops l) {
	channel = channel++ % 16;
	Mix_PlayChannel(channel, chunk[path].data, l == Loops::INFINITE ? -1 : 0);
}

void AudioI::stopMusic() {
	Mix_HaltMusic();
}

void AudioI::stopAllEffects() {
	Mix_HaltChannel(-1);
}

void AudioI::stopAllSound() {
	stopMusic();
	stopAllEffects();
}
