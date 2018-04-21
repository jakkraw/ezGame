#pragma once
namespace ezGame {
	struct Audio {
		enum class Loops { ZERO, INFINITE = -1 };
		using Path = const char*;
		virtual void playMusic(Path, Loops = Loops::ZERO) = 0;
		virtual void playEffect(Path, Loops = Loops::ZERO) = 0;
		virtual void stopMusic() = 0;
		virtual void stopAllEffects() = 0;
		virtual void stopAllSound() = 0;
	};
}