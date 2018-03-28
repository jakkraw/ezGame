#pragma once

namespace ezGame {

	struct ResolutionInfo {
		virtual const Resolution resolution() const = 0;
	};

}