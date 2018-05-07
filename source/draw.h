#pragma once
#include "sdl2.h"

#include "loader.h"
struct SurfaceWrapper {
	SDL_Surface* data;
	SurfaceWrapper(SurfaceWrapper&& s) : data(s.data) { s.data = nullptr; };
	SurfaceWrapper(const char* path) : data(IMG_Load(path)) {
	}
	~SurfaceWrapper() { SDL_FreeSurface(data); }
};

struct TextureWrapper {
	SDL_Texture* data;
	TextureWrapper(TextureWrapper&& t) : data(t.data) { t.data = nullptr; };
	TextureWrapper(SurfaceWrapper& s, SDL_Renderer& r) : data(SDL_CreateTextureFromSurface(&r, s.data)) { }
	~TextureWrapper() { SDL_DestroyTexture(data); }
};

class TextureLoader : Loader<TextureWrapper> {
	Loader<SurfaceWrapper> surfaceLoader;
	SDL_Renderer& renderer;
public:
	TextureLoader(SDL_Renderer& r) : renderer(r) {}

	TextureWrapper& operator[](std::string name) {
		auto data = map.find(name);
		if (data == map.end())
			return map.emplace(name, TextureWrapper(surfaceLoader[name], renderer)).first->second;
		return data->second;
	}
};

#include "font.h"
struct Font {
	using FontData = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>;
	FontData font = { resource::createFont(), TTF_CloseFont };
	operator TTF_Font*() const { return &*font; }
};

#include "interface/draw.h"
//struct RelativeConversion {
//	virtual const Position convert(const RelativePosition&) const = 0;
//	virtual const Size convert(const RelativeSize&) const = 0;
//	virtual const Area convert(const RelativeArea&) const = 0;
//	virtual const Line convert(const RelativeLine&) const = 0;
//};

using namespace ezGame;
class DrawI : public Draw {
	SDL_Renderer& renderer;
	TextureLoader textures;
	Font font;
	Color background = BLACK;
	const Size resolution;

	void set(const Color & c) {
		SDL_SetRenderDrawColor(&renderer, c.red, c.green, c.blue, Uint8(255 * c.opacity));
	}

	Size getResolution() {
		int w, h;
		SDL_RenderGetLogicalSize(&renderer, &w, &h);
		return Size( w,h);
	}

	virtual Position convert(const RelativePosition& rp) const{
		return { Pixel(rp.x * resolution.width), Pixel(rp.y * resolution.height) };
	}
	virtual Size convert(const RelativeSize& rs) const {
		return { Pixel(rs.width * resolution.width) , Pixel(rs.height * resolution.height) };
	}
	virtual Area convert(const RelativeArea& ra) const {
		return { convert(ra.topLeft), convert(ra.size) };
	}
	virtual Line convert(const RelativeLine& rl) const {
		return { convert(rl.start),convert(rl.end) };
	}


public:
	explicit DrawI(SDL_Renderer& renderer) : 
	renderer(renderer), 
	textures(renderer),
	resolution(getResolution()) {}

	void reset() {
		SDL_RenderPresent(&renderer);
		set(background);
		SDL_RenderClear(&renderer);
	}

	// Inherited via Draw
	void image(Path path, const Area &area, Degrees rotation = 0) override {
		SDL_Rect destination{ area.topLeft.x, area.topLeft.y, area.size.width, area.size.height };

		SDL_RenderCopyEx(&renderer, textures[path].data, nullptr, &destination,
			rotation, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	void image(Path path, const RelativeArea& rArea, Degrees rotation) override {
		image(path, convert(rArea), rotation);
	}

	void rectangle(const RelativeArea& rArea, const Color& color, Type type) override {
		rectangle(convert(rArea), color, type);
	}

	void rectangle(const Area &area, const Color& color, Type type) override {
		set(color);
		SDL_Rect rect{ area.topLeft.x, area.topLeft.y, area.size.width, area.size.height };
		type == Type::FILLED ?
			SDL_RenderFillRect(&renderer, &rect) : SDL_RenderDrawRect(&renderer, &rect);
	}

	void point(const Position &position, const Color&color) override {
		set(color);
		SDL_RenderDrawPoint(&renderer, position.x, position.y);
	}

	void point(const RelativePosition &rPos, const Color & color) override {
		point(convert(rPos),color);
	}

	void line(const Line& line, const Color &color) override {
		set(color);
		SDL_RenderDrawLine(&renderer, line.start.x, line.start.y, line.end.x, line.end.y);
	}

	void line(const RelativeLine& rLine, const Color &color) override {
		line(convert(rLine), color);
	}

	void text(Text text, const Area & maximal, const Color & font) override {
		auto co = font;
		auto c = SDL_Color{ co.red, co.green, co.blue , Uint8(255 * co.opacity) };
		auto textSurface = TTF_RenderText_Solid(this->font, text, c);
		auto mTexture = SDL_CreateTextureFromSurface(&renderer, textSurface);
		SDL_FreeSurface(textSurface);

		int w, h;
		SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);

		if (maximal.size.width < w || maximal.size.height < h) {
			const auto multiplier = __min(maximal.size.width / (double)w, maximal.size.height / (double)h);
			w = int(w*multiplier);
			h = int(h * multiplier);
		}

		auto rect = SDL_Rect{ (int)maximal.topLeft.x , (int)maximal.topLeft.y , (int)w, (int)h };
		SDL_RenderCopyEx(&renderer, mTexture, nullptr, &rect, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
		SDL_DestroyTexture(mTexture);
	}

	void text(Text text, const RelativeArea & maximal, const Color & font) override {
		this->text(text, convert(maximal), font);
	}
};