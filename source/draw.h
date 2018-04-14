#pragma once
#include "SDL.h"

#include "SDL_image.h"
#pragma comment(lib, "SDL2_image")

#include "SDL_ttf.h"
#pragma comment(lib, "SDL2_ttf")

#include "loader.h"
struct SurfaceWrapper {
	SDL_Surface* data;
	SurfaceWrapper(SurfaceWrapper&& s) : data(std::move(s.data)) { s.data = nullptr; };
	SurfaceWrapper(const char* path) : data(IMG_Load(path)) { }
	~SurfaceWrapper() { SDL_FreeSurface(data); }
};

struct TextureWrapper {
	SDL_Texture* data;
	TextureWrapper(TextureWrapper&& t) : data(std::move(t.data)) { t.data = nullptr; };
	TextureWrapper(SurfaceWrapper& s, SDL_Renderer& r) : data(SDL_CreateTextureFromSurface(&r, s.data)) { }
	~TextureWrapper() { SDL_DestroyTexture(data); }
};

class TextureLoader : Loader<TextureWrapper> {
	Loader<SurfaceWrapper> surfaceLoader;
	SDL_Renderer& renderer;
public:
	TextureLoader(SDL_Renderer& r) : renderer(r) {}

	TextureWrapper& operator[](std::string name) {
		auto& data = map.find(name);
		if (data == map.end())
			return map.emplace(name, TextureWrapper(surfaceLoader[name], renderer)).first->second;
		else return data->second;
	}
};

#include "resources.h"
struct Font {
	using FontData = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>;
	FontData font = { nullptr, TTF_CloseFont };
	Font() {
		TTF_Init();
		font.reset(resource::loadFont());
	}
	~Font() { TTF_Quit(); }
	operator TTF_Font*() { return &*font; }
};

#include "interface\draw.h"
using namespace ezGame;
class Draw : public ezGame::Draw {
	SDL_Renderer& renderer;
	TextureLoader texture;
	Font font;
	Color background = BLACK;

	void set(const Color & c) {
		SDL_SetRenderDrawColor(&renderer, c.red, c.green, c.blue, Uint8(255 * c.alpha));
	}

public:
	Draw(SDL_Renderer& renderer) : renderer(renderer), texture(renderer) {
		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_JPG);
	}

	~Draw() { IMG_Quit(); }

	void reset() {
		SDL_RenderPresent(&renderer);
		set(background);
		SDL_RenderClear(&renderer);
	}

	void update(const SDL_Event& event) {}

	//// Inherited via Draw
	//virtual const Resolution resolution() const override {
	//	int w, h;
	//	SDL_RenderGetLogicalSize(&renderer, &w, &h);
	//	return{ w,h };
	//}

	virtual void draw(const Rectangle& r) override {
		set(r.color);
		SDL_Rect rect{ r.topLeft.x, r.topLeft.y, r.size.width, r.size.height };
		r.type == ShapeType::Filled ?
			SDL_RenderFillRect(&renderer, &rect) : SDL_RenderDrawRect(&renderer, &rect);
	}

	virtual void draw(const Line &l) override {
		set(l.color);
		SDL_RenderDrawLine(&renderer, l.begin.x, l.begin.y, l.end.x, l.end.y);
	}

	virtual void draw(const Point &p) override {
		set(p.color);
		SDL_RenderDrawPoint(&renderer, p.x, p.y);
	}

	virtual void draw(const Image &t) override {
		SDL_Rect destination{ t.topLeft.x, t.topLeft.y, t.size.width, t.size.height };

		SDL_RenderCopyEx(&renderer, texture[t.cPath()].data, NULL,
			t.size == DEFAULT_SIZE ? nullptr : &destination,
			t.degrees, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	virtual void draw(const Text &t) override {
		auto co = t.color;
		auto c = SDL_Color{ co.red, co.green, co.blue , Uint8(255 * co.alpha) };
		auto textSurface = TTF_RenderText_Solid(font, t.cText(), c);
		auto mTexture = SDL_CreateTextureFromSurface(&renderer, textSurface);
		SDL_FreeSurface(textSurface);

		int w, h;
		SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);

		if (t.max != DEFAULT_SIZE && t.max.width < w || t.max.height < h) {
			auto multiplier = __min(t.max.width / (double)w, t.max.height / (double)h);
			w = int(w * multiplier);
			h = int(h * multiplier);
		}

		auto rect = SDL_Rect{ t.topLeft.x ,t.topLeft.y , w, h };
		SDL_RenderCopyEx(&renderer, mTexture, nullptr, &rect, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
		SDL_DestroyTexture(mTexture);
	}

};