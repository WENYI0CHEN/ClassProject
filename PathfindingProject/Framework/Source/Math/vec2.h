#pragma once

#include <math.h>

namespace fw
{
	class vec2
	{
	public:
		static void Test();

		vec2() {}
        vec2(float nxy) : x(nxy), y(nxy) {}
		vec2(float nx, float ny) : x(nx), y(ny) {}

		vec2 operator+(const vec2& o) const { return vec2(x + o.x, y + o.y); }
		vec2 operator-(const vec2& o) const { return vec2(x - o.x, y - o.y); }
		vec2 operator*(const vec2& o) const { return vec2(x * o.x, y * o.y); }
		vec2 operator/(const vec2& o) const { return vec2(x / o.x, y / o.y); }

		vec2 operator*(const float& o) const { return vec2(x * o, y * o); }
		vec2 operator/(const float& o) const { return vec2(x / o, y / o); }

		void operator+=(const vec2& o) { x += o.x; y += o.y; }
		void operator-=(const vec2& o) { x -= o.x; y -= o.y; }
		void operator*=(const vec2& o) { x *= o.x; y *= o.y; }
		void operator/=(const vec2& o) { x /= o.x; y /= o.y; }

		void operator*=(const float& o) { x *= o; y *= o; }
		void operator/=(const float& o) { x /= o; y /= o; }

		float dot(const vec2& o) { return x*o.x + y*o.y; }
		float length() const { return sqrtf(x*x + y*y); }

		void normalize()
		{
			float len = length();
			if (len == 0)
			{
				return;
			}

			x /= len;
			y /= len;
		}

		vec2 getNormalized() const
		{
			float len = length();
			if (len == 0)
			{
				return { 0, 0 };
			}

			return { x / len, y / len };
		}

	public:
		float x = 0;
		float y = 0;
	};

	static vec2 operator*(float f, const vec2& o)
	{
		return { o.x*f, o.y*f };
	}
}
