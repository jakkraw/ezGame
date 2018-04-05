#pragma once
#include <string>
#include <unordered_map>

#include "../SDL2/include/SDL_mixer.h"
#include "../SDL2/include/SDL_image.h"
#include "../SDL2/include/SDL_ttf.h"

template<typename Data>
class Loader {
protected:
	using Map = std::unordered_map<std::string, Data>;
	Map map;
public:
	Data& operator[](std::string name) {
		auto& data = map.find(name);
		if (data == map.end())
			return map.emplace(name, Data(name.c_str())).first->second;
		else return data->second;
	}
};

struct MusicWrapper {
	Mix_Music* data;
	MusicWrapper(MusicWrapper&& m) : data(std::move(m.data)) { m.data = nullptr; }
	MusicWrapper(const char* path) : data(Mix_LoadMUS(path)) {}
	~MusicWrapper() { Mix_FreeMusic(data); }
};

struct ChunkWrapper {
	Mix_Chunk* data;
	ChunkWrapper(ChunkWrapper&& c) : data(std::move(c.data)) { c.data = nullptr; }
	ChunkWrapper(const char* path) : data(Mix_LoadWAV(path)){}
	~ChunkWrapper() { Mix_FreeChunk(data); }
};

struct SurfaceWrapper {
	SDL_Surface* data;
	SurfaceWrapper(SurfaceWrapper&& s) : data(std::move(s.data)) { s.data = nullptr; };
	SurfaceWrapper(const char* path) : data(IMG_Load(path)) { }
	~SurfaceWrapper() { SDL_FreeSurface(data); }
};

struct TextureWrapper {
	SDL_Texture* data;
	TextureWrapper(TextureWrapper&& t) : data(std::move(t.data)) { t.data = nullptr; };
	TextureWrapper(SurfaceWrapper& s, SDL_Renderer& r) : data(SDL_CreateTextureFromSurface(&r, s.data)){ }
	~TextureWrapper() { SDL_DestroyTexture(data); }
};

class TextureLoader : Loader<TextureWrapper> {
	Loader<SurfaceWrapper> surfaceLoader;
	SDL_Renderer& renderer;

public:
	TextureLoader(SDL_Renderer& r) : renderer(r){}

	TextureWrapper& operator[](std::string name) {
		
		auto& data = map.find(name);
		if (data == map.end())
			return map.emplace(name, TextureWrapper(surfaceLoader[name], renderer)).first->second;
		else return data->second;
	}
};


#include "resources.h"	
class TextLoader {
public:
	SDL_Renderer& renderer;
	TTF_Font* font = TTF_OpenFontRW(SDL_RWFromConstMem(exampleFont, exampleFont_size), true, 86);
	TextLoader(SDL_Renderer& renderer) : renderer(renderer){}
	~TextLoader() {
		TTF_CloseFont(font);
	}
};