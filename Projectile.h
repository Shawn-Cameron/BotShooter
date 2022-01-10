#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
using namespace std;

class bullet {
private:
	float size = 0.5;
	float speed = 0.8;
	float limit = 150;
	float orgX, orgY, orgZ;

public:
	float angle;
	float dirX;
	float dirY;
	float dirZ;
	float posX;
	float posY;
	float posZ;
	bool active = true;

	void drawBullet();
	void animate();
	bool dropped();
	bool collision(float x, float xRange,float y ,float yRange, float z, float zRange);

	bullet(){}

	bullet(float ang, float vertAngle,float x, float y, float z, float offset) {
		angle = ang;
		posY = y;
		dirX = sin(ang * M_PI/180);
		dirY = sin(vertAngle * M_PI / 180);
		dirZ = -cos(ang * M_PI/180);
		posX = x + dirX * offset;
		posZ = z + dirZ * offset;
		orgX = posX;
		orgY = posY;
		orgZ = posZ;

		
	}


};