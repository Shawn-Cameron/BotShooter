#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gl/glut.h>
#include <utility>
#include <vector>
#include "Projectile.h"



void bullet::drawBullet() {
	if (!active) return;

	glBindTexture(GL_TEXTURE_2D, 10);
	GLUquadric* bullet = gluNewQuadric();
	gluQuadricTexture(bullet, GL_TRUE);


	glPushMatrix();

	glTranslatef(posX,posY,posZ); 
	gluSphere(bullet, size,20,20);

	glPopMatrix();


	gluDeleteQuadric(bullet);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void bullet::animate() {
	if (limit) {
		posX += dirX * speed;
		posY += dirY * speed;
		posZ += dirZ * speed;
		limit--;
	}
	else {
		active = false;
	}
}

bool bullet::dropped() {
	if (!active) return true;
	return false;
}

bool bullet::collision(float x, float xRange, float y, float yRange, float z, float zRange) {
	bool xCol = posX >= x - xRange && posX <= x + xRange;
	bool yCol = posY >= y - yRange && posY <= y + yRange;
	bool zCol = posZ >= z - zRange && posZ <= z + zRange;
	bool origCoor = (orgX - posX) < 2 && (orgY - posY) < 2 && (orgZ - posZ) < 2;

	return (xCol && yCol && zCol && !origCoor);
}