#include "windowSDL.h"

static void keyDown(KeyState& button){
	button.pressed = !button.down;
	button.down = true;
	button.released = false;
}
static void keyUp(KeyState& button) {
	button.released = true;
	button.down = false;
	button.pressed = false;
}
static void reset(std::vector<KeyState>& buttons) {
	for (auto& button : buttons) {
		button.released = false;
		button.pressed = false;
	}
}

void WindowSDL::setDraw(const Color & color) {
	SDL_SetRenderDrawColor(&sdlData.renderer, color.red, color.green, color.blue, Uint8(255 * color.alpha));
}

#define KeyboardKeysMaxSize 300
WindowSDL::WindowSDL(SDLData& sdlData) :
	sdlData(sdlData), loaders(sdlData.renderer), keyboard(KeyboardKeysMaxSize) {}

void WindowSDL::update() {
	drawAndClearRenderer();
	reset(keyboard);
	reset(mouse.buttons);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_KEYDOWN:
			keyDown((Keyboard::Key)event.key.keysym.scancode);
			break;

		case SDL_KEYUP:
			keyUp((Keyboard::Key)event.key.keysym.scancode);
			break;

		case SDL_MOUSEBUTTONUP:
			keyUp((Mouse::Key)event.button.button);
			break;

		case SDL_MOUSEBUTTONDOWN:
			keyDown((Mouse::Key)event.button.button);
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				quit();
			break;

		case SDL_MOUSEMOTION:
			setPointer(Position(event.motion.x+1, event.motion.y+1));
			break;
		}
	}
}

void WindowSDL::drawAndClearRenderer() {
	SDL_RenderPresent(&sdlData.renderer);
	setDraw(backgroundColor);
	SDL_RenderClear(&sdlData.renderer);
}

void WindowSDL::setPointer(Position & p) {
	mouse.pointer = p;
}

 void WindowSDL::keyUp(Keyboard::Key k) {
	::keyUp(keyboard[(int)k]);
}

 void WindowSDL::keyDown(Keyboard::Key k) {
	::keyDown(keyboard[(int)k]);
}

 void WindowSDL::keyUp(Mouse::Key m) {
	::keyUp(mouse.buttons[(int)m]);
}

 void WindowSDL::keyDown(Mouse::Key m) {
	::keyDown(mouse.buttons[(int)m]);
}
 
 bool WindowSDL::isRunning() {
	 return loopState == LoopState::Running;
 }

// Inherited via Window

 void WindowSDL::draw(const Rectangle & r)
{
	setDraw(r.color);
	SDL_Rect rect{ r.topLeft.x, r.topLeft.y, r.size.width, r.size.height };
	r.type == ShapeType::Filled ? SDL_RenderFillRect(&sdlData.renderer, &rect) : SDL_RenderDrawRect(&sdlData.renderer, &rect);
}

 void WindowSDL::draw(const Line & l)
{
	setDraw(l.color);
	SDL_RenderDrawLine(&sdlData.renderer, l.begin.x, l.begin.y, l.end.x, l.end.y);
}

 void WindowSDL::draw(const Point & p)
{
	setDraw(p.color);
	SDL_RenderDrawPoint(&sdlData.renderer, p.x, p.y);
}

void WindowSDL::draw(const Image & t)
{
	SDL_Rect destination{ t.topLeft.x, t.topLeft.y, t.size.width, t.size.height };

	SDL_RenderCopyEx(&sdlData.renderer, loaders.texture[t.cPath()].data, NULL,
		t.size == DEFAULT_SIZE ? nullptr : &destination,
		t.angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}

const KeyState WindowSDL::button(Mouse::Key button) const
{
	return mouse.buttons[(int)button];
}

const KeyState WindowSDL::button(Keyboard::Key button) const
{
	return keyboard[(int)button];
}

const Mouse::Position WindowSDL::cursor() const
{
	return mouse.pointer;
}

void WindowSDL::play(const Music& music)
{
	Mix_PlayMusic(loaders.music[music.cPath()].data, music.loopTimes == LoopTimes::Infinite ? -1 : 0);
}

void WindowSDL::play(const Effect& effect)
{
	static short channel = 0;
	channel = channel++ % 16;
	Mix_PlayChannel(channel, loaders.chunk[effect.cPath()].data, effect.loopTimes == LoopTimes::Infinite ? -1 : 0);
}

void WindowSDL::stopMusic()
{
	Mix_HaltMusic();
}

void WindowSDL::stopAllEffects()
{
	Mix_HaltChannel(-1);
}

void WindowSDL::stopAllSound()
{
	stopMusic();
	stopAllEffects();
}

void WindowSDL::quit()
{
	loopState = LoopState::Stopped;
}

void WindowSDL::draw(const Text& text)
{
	//max_Width max_height
	//color
	//wrap or not
	//quality
	auto co = text.color;
	auto c = SDL_Color{ co.red, co.green, co.blue , Uint8(255 * co.alpha) };
	auto textSurface = TTF_RenderText_Solid(loaders.text.font, text.cText(), c);
	auto mTexture = SDL_CreateTextureFromSurface(&sdlData.renderer, textSurface);
	SDL_FreeSurface(textSurface);

	int w, h;
	SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);

	if (text.max != DEFAULT_SIZE && text.max.width < w || text.max.height < h) {
		auto multiplier = __min(text.max.width / (double)w, text.max.height / (double)h);
		w =int( w * multiplier);
		h =int( h * multiplier);
	}

	auto rect = SDL_Rect{ text.topLeft.x ,text.topLeft.y , w, h };
	SDL_RenderCopyEx(&sdlData.renderer, mTexture, nullptr, &rect, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	SDL_DestroyTexture(mTexture);
}

const Resolution WindowSDL::resolution() const
{
	int w, h;
	SDL_RenderGetLogicalSize(&sdlData.renderer, &w, &h);
	return{ w,h };
}

Settings & WindowSDL::settings() const
{
	return (Settings&)*this;
}

void WindowSDL::set(Title title)
{
	SDL_SetWindowTitle(&sdlData.window, title);
}

void WindowSDL::set(Background bg)
{
	backgroundColor = std::move(bg);
}

const bool WindowSDL::pressed(Mouse::Key mk) const
{
	return button(mk).pressed;
}

const bool WindowSDL::pressed(Keyboard::Key kk) const
{
	return button(kk).pressed;
}

const bool WindowSDL::down(Mouse::Key mk) const
{
	return button(mk).down;
}

const bool WindowSDL::down(Keyboard::Key kk) const
{
	return button(kk).down;
}

const bool WindowSDL::released(Mouse::Key mk) const
{
	return button(mk).released;
}

const bool WindowSDL::released(Keyboard::Key kk) const
{
	return button(kk).released;
}

void WindowSDL::set(CursorVisibility cv)
{
	switch (cv) {
	case CursorVisibility::Hide: SDL_ShowCursor(0); break;
	case CursorVisibility::Show: SDL_ShowCursor(1); break;
	}
}

void WindowSDL::set(Window::Type wt)
{
	switch (wt) {
	case Window::Type::Fullscreen: SDL_SetWindowFullscreen(&sdlData.window, SDL_WINDOW_FULLSCREEN); break;
	case Window::Type::Windowed: SDL_SetWindowFullscreen(&sdlData.window,SDL_WINDOW_SHOWN); break;
	}
}

void WindowSDL::set(Resolution resolution)
{
	SDL_SetWindowSize(&sdlData.window, resolution.width, resolution.height);
	SDL_RenderSetLogicalSize(&sdlData.renderer, resolution.width, resolution.height);
}

const CursorVisibility WindowSDL::cursorVisibility() const
{
	return SDL_ShowCursor(-1) ? CursorVisibility::Show : CursorVisibility::Hide;
}

const Window::Type WindowSDL::windowType() const
{
	//SDL_DisplayMode dm;
	//SDL_GetWindowDisplayMode(&sdlData.window, &dm);
	SDL_GetWindowFlags(&sdlData.window);
	return SDL_GetWindowFlags(&sdlData.window) & SDL_WINDOW_FULLSCREEN ? Window::Type::Fullscreen : Window::Type::Windowed;

}

const Background WindowSDL::background() const
{
	return backgroundColor;
}

const Title WindowSDL::title() const
{
	return SDL_GetWindowTitle(&sdlData.window);
}