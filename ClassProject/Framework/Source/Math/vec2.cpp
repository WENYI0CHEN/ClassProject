#include "GameCore.h"
#include <assert.h>
#include "vec2.h"

namespace fw
{
	void vec2::Test()
	{
		vec2 a(2, 5);
		assert(a.x == 2 && a.y == 5);

		const vec2 b(3, -2);
		vec2 c = a + b;
		assert(c.x == 5 && c.y == 3);

		a += b;

		c *= 5;
		assert(c.x == 25 && c.y == 15);

		vec2 d = c;
		vec2 e(c);

		vec2 f = 5 * d;
	}
}
