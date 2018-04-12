#pragma once
#include "window.h"
#include "stage.h"

namespace ezGame {

	struct Ez {
		virtual void goTo(Stage*&&) = 0;

		using Seconds = double;
		virtual Seconds delta() const = 0;
	};	

	
}