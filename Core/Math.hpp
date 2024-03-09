#pragma once

#include <math.h>
#include "Type.hpp"

namespace dep {
	namespace math {
		export double distance(const dep::vec2D vec1, const dep::vec2D vec2) {
			return sqrt(pow(vec2.x - vec1.x, 2) + pow(vec2.y - vec1.y, 2));
		}
	}
}