#pragma once

#define NUM_PATH_POINTS 300

class Vector2D {
public:
	float x, y;

	Vector2D() {
		x = 0;
		y = 0;
	}

	Vector2D(float xn, float yn) {
		x = xn;
		y = yn;
	}
};

