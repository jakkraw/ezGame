#pragma once
#include "draw.h"
namespace ezGame {

	struct DrawFragment : public Draw {
		const Position offset;
		const Size size;
		const Ratio resRatio;
		const Resolution res;
		Draw& draw;
		DrawFragment(Draw& draw, const Position& offset, const Size& size, const Resolution& res)
			:
			draw(draw),
			offset(offset),
			size(size),
			resRatio(draw.resolution()/res),
			res(res) {}


		// Inherited via Draw
		const Resolution resolution() const override {
			return res;
		}

		virtual void draw(const Rectangle &r) override {
			Rectangle nr = r;
			//nr.topLeft += offset;
		}

		virtual void draw(const Line &) override;

		virtual void draw(const Point &) override;

		virtual void draw(const Image &) override;

		virtual void draw(const Text &) override;

	};

}