#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glut.h>
#include "Enemy.h"

#include <iostream>
using namespace std;

GLfloat robotRed_mat_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 0.55f };
GLfloat robotRed_mat_diffuse[] = { 0.5f,0.0f,0.0f,1.0f };
GLfloat robotRed_mat_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat robotRed_mat_shininess[] = { 32.0F };

GLfloat robotSilver_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotSilver_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotSilver_mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat robotSilver_mat_shininess[] = { 76.8F };

GLfloat robotBlack_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat robotBlack_mat_diffuse[] = { 0.001f, 0.001f, 0.001f, 1.0f };
GLfloat robotBlack_mat_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat robotBlack_mat_shininess[] = { 75.0f };


void Enemy::drawRobot() {
	if (done) return;
	if (shootWait > 0) shootWait--;
	int transY = -12;

	glPushMatrix();

	glTranslatef(0, transY, robotZPos);
	glRotatef(deathAngle, 1, 0, 0);
	glTranslatef(0, -transY, -robotZPos);

	//Controls movement 
	glTranslatef(robotXPos, -8.5, robotZPos);
	
	//Turns Robot
	glTranslatef(2.5, 0, 0);
	glRotatef(robotDirAngle, 0.0, 1.0, 0.0);
	glTranslatef(-2.5, 0, 0);

	glScalef(sizeFactor, sizeFactor, sizeFactor);

	//Draws parts
	drawBody();
	drawHead();
	drawWheel();

	glPopMatrix();
}

void Enemy::drawHead() {

	glPushMatrix();

	glTranslatef(headWidth * 3.3, robotBodyLength / 3 + 0.8, 0.0);
	glScalef(headWidth, headLength, headDepth);
	solidCube(6.0,1);

	
	drawHeadmain();

	glPopMatrix();

}

void Enemy::drawHeadmain() {
	//Draws the red part of the head

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotRed_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotRed_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotRed_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotRed_mat_shininess);

	glPushMatrix();

	glScalef(0.75, 1.2, 1.2);
	solidCube(6.0, 1);

	drawEye();
	drawCannon();

	glPopMatrix();
}

void Enemy::drawEye() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotSilver_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotSilver_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotSilver_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotSilver_mat_shininess);

	float eyeRad = headWidth * 1.5;
	float eyeWidth = 0.4;

	glPushMatrix();

	glTranslatef(0, 0, headDepth * 2.6);

	glBindTexture(GL_TEXTURE_2D, 6);
	GLUquadric* cyl = gluNewQuadric();
	gluQuadricTexture(cyl, GL_TRUE);
	gluCylinder(cyl, eyeRad, eyeRad, eyeWidth, 20, 10);
	gluDeleteQuadric(cyl);
	glBindTexture(GL_TEXTURE_2D, 0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBlack_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBlack_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBlack_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBlack_mat_shininess);

	glPushMatrix();

	//draws circle part of eye
	glTranslatef(0, 0, eyeWidth);

	glBindTexture(GL_TEXTURE_2D, 5);
	GLUquadric* eye = gluNewQuadric();
	gluQuadricTexture(eye, GL_TRUE);
	gluDisk(eye, 0.0, eyeRad, 20, 1);
	gluDeleteQuadric(eye);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
	glPopMatrix();
}

void Enemy::drawCannon() {

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotSilver_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotSilver_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotSilver_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotSilver_mat_shininess);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 8);
	GLUquadric* cannon = gluNewQuadric();
	gluQuadricTexture(cannon, GL_TRUE);


	glTranslatef(0, headLength * 4.8, 0);
	glScalef(cannonBaseWidth, cannonBaseLength, cannonBaseDepth);
	gluSphere(cannon, headWidth * 3.125, 20, 20);


	glPushMatrix();
	
	glRotatef(cannonAngle - robotDirAngle, 0, 1, 0);
	glTranslatef(0, cannonBaseLength * 4, 0);
	gluCylinder(cannon, cannonRad, cannonRad, cannonLength, 20, 20);

	gluDeleteQuadric(cannon);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	glPopMatrix();
}

void Enemy::drawBody() {
	//Draws the connecting rod
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotSilver_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotSilver_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotSilver_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotSilver_mat_shininess);

	glPushMatrix();

	glScalef(1.0, robotBodyLength, 1.0);
	solidCube(rodWidth,4);
	glPopMatrix();
}

void Enemy::drawWheel() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotRed_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotRed_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotRed_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotRed_mat_shininess);

	float transVal = robotBodyLength / 2.7;
	glPushMatrix();

	//Rotate Wheel
	glTranslatef(0, -transVal, 0);
	glRotatef(wheelRotationVal, 1, 0, 0);
	glTranslatef(0, transVal, 0);

	//Position Wheel
	glTranslatef(wheelWidth * 2 + 0.5, -transVal, 0.0);
	glScalef(wheelWidth, 1.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	solidTorus(wheelRad * 0.6, wheelRad, 20, 20, 2);

	drawWheelCovers();

	glPopMatrix();
}

void Enemy::drawWheelCovers() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotSilver_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotSilver_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotSilver_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotSilver_mat_shininess);

	//positions and draws each cover
	glPushMatrix();
	glTranslatef(0.0, 0.0, wheelWidth * 0.87);
	drawWheelCover();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0, 0.0, -wheelWidth * 1.05);
	drawWheelCover();
	glPopMatrix();
}

void Enemy::drawWheelCover() {
	float coverRad = wheelRad * 1.2;
	float coverWidth = wheelWidth / 6;

	glBindTexture(GL_TEXTURE_2D, 6);
	GLUquadric* cyl = gluNewQuadric();
	gluQuadricTexture(cyl, GL_TRUE);
	gluCylinder(cyl, coverRad, coverRad, coverWidth, 200, 10);
	gluDeleteQuadric(cyl);


	glBindTexture(GL_TEXTURE_2D, 7);
	GLUquadric* disk = gluNewQuadric();
	gluQuadricTexture(disk, GL_TRUE);

	glPushMatrix();
	glRotatef(180, 0.0, 1.0, 0.0);
	gluDisk(disk, 0.0, coverRad, 40, 1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.0, 0.0, coverWidth);
	gluDisk(disk, 0.0, coverRad, 40, 1);
	glPopMatrix();

	gluDeleteQuadric(disk);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Enemy::animate(float canX, float canZ) {
	if (dead) {
		if (deathWait <= 0) done = true;
		if (deathAngle >= 90) deathWait--;
		else deathAngle += 2;
		return;
	}

	if (nextPoint >= NUM_PATH_POINTS - 1 ) {
		done = true;
		return;
	}
	float nextX = path[nextPoint].x;
	float nextZ = path[nextPoint].y;

	computeAngle(nextX, nextZ);
	computeCannonAngle(canX,canZ);

	wheelRotationVal += movementSpeed * 20;
	robotXPos = nextX;
	robotZPos = nextZ;
	nextPoint++;
}

void Enemy::computeAngle(float nextX, float nextZ) {
	
	if (nextPoint == 0) {
		robotXPos = path[0].x;
		robotZPos = path[0].y;
		nextX = path[1].x;
		nextZ = path[1].y;
	}
	
	float deltaX = nextX - robotXPos;
	float deltaZ = nextZ - robotZPos;
	float d = sqrt(pow(deltaX, 2) + pow(deltaZ, 2));
	robotDirAngle = asin(deltaX / d) * 180 / M_PI;
	
	
}

void Enemy::initEnemy() {
	animate(0,0);
}

void Enemy::solidCube(float fullsize, int textID) {

	float size = fullsize / 2;
	float d = sqrt(pow(size, 2) * 3);

	glBindTexture(GL_TEXTURE_2D, textID);
	glBegin(GL_QUADS); //top

	glTexCoord2f(0, 0);
	glNormal3f(-size / d, size / d, -size / d);
	glVertex3f(-size, size, -size);

	glTexCoord2f(0, 1);
	glNormal3f(-size / d, size / d, size / d);
	glVertex3f(-size, size, size);

	glTexCoord2f(1, 1);
	glNormal3f(size / d, size / d, size / d);
	glVertex3f(size, size, size);

	glTexCoord2f(1, 0);
	glNormal3f(size / d, size / d, -size / d);
	glVertex3f(size, size, -size);
	glEnd();

	
	glBegin(GL_QUADS); //right

	glTexCoord2f(0, 0);
	glNormal3f(size / d, size / d, -size / d);
	glVertex3f(size, size, -size);

	glTexCoord2f(0, 1);
	glNormal3f(size / d, size / d, size / d);
	glVertex3f(size, size, size);

	glTexCoord2f(1, 1);
	glNormal3f(size / d, -size / d, size / d);
	glVertex3f(size, -size, size);

	glTexCoord2f(1, 0);
	glNormal3f(size / d, -size / d, -size / d);
	glVertex3f(size, -size, -size);
	glEnd();


	glBegin(GL_QUADS); //left

	glTexCoord2f(0, 0);
	glNormal3f(-size / d, size / d, -size / d);
	glVertex3f(-size, size, -size);

	glTexCoord2f(0, 1);
	glNormal3f(-size / d, -size / d, -size / d);
	glVertex3f(-size, -size, -size);

	glTexCoord2f(1, 1);
	glNormal3f(-size / d, -size / d, size / d);
	glVertex3f(-size, -size, size);

	glTexCoord2f(1, 0);
	glNormal3f(-size / d, size / d, size / d);
	glVertex3f(-size, size, size);

	glEnd();



	glBegin(GL_QUADS); //bottom

	glTexCoord2f(0, 0);
	glNormal3f(-size / d, -size / d, -size / d);
	glVertex3f(-size, -size, -size);

	glTexCoord2f(0, 1);
	glNormal3f(-size / d, -size / d, size / d);
	glVertex3f(-size, -size, size);

	glTexCoord2f(1, 1);
	glNormal3f(size / d, -size / d, size / d);
	glVertex3f(size, -size, size);

	glTexCoord2f(1, 0);
	glNormal3f(size / d, -size / d, -size / d);
	glVertex3f(size, -size, -size);
	glEnd();

	
	glBegin(GL_QUADS); //back

	glTexCoord2f(0, 0);
	glNormal3f(-size / d, -size / d, -size / d);
	glVertex3f(-size, -size, -size);

	glTexCoord2f(0, 1);
	glNormal3f(-size / d, size / d, -size / d);
	glVertex3f(-size, size, -size);

	glTexCoord2f(1, 1);
	glNormal3f(size / d, size / d, -size / d);
	glVertex3f(size, size, -size);

	glTexCoord2f(1, 0);
	glNormal3f(size / d, -size / d, -size / d);
	glVertex3f(size, -size, -size);
	glEnd();

	
	glBegin(GL_QUADS); //front
	glTexCoord2f(0, 0);
	glNormal3f(-size / d, -size / d, size / d);
	glVertex3f(-size, -size, size);

	glTexCoord2f(0, 1);
	glNormal3f(-size / d, size / d, size / d);
	glVertex3f(-size, size, size);

	glTexCoord2f(1, 1);
	glNormal3f(size / d, size / d, size / d);
	glVertex3f(size, size, size);

	glTexCoord2f(1, 0);
	glNormal3f(size / d, -size / d, size / d);
	glVertex3f(size, -size, size);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Enemy::solidTorus(float r, float R, int sides, int rings, int textID)
{
	//Sourced from: https://gist.github.com/gyng/8939105

	glBindTexture(GL_TEXTURE_2D, textID);

	const double pi2 = 2 * M_PI;

	for (int i = 0; i < sides; i++) {

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= rings; j++) {
			for (int k = 0; k <= 1; k++) {
				float s = (i + k) % sides + 0.5;
				float t = j % (rings + 1);

				float x = (R + r * cos(s * pi2 / sides)) * cos(t * pi2 / rings);
				float y = (R + r * cos(s * pi2 / sides)) * sin(t * pi2 / rings);
				float z = r * sin(s * pi2 / sides);

				float u = (i + k) / (float)sides;
				float v = t / (float)rings;

				glTexCoord2d(u, v);
				glNormal3f(x, y, z);
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);


}

void Enemy::computeCannonAngle(float canX, float canZ) {
	float deltaX = canX - 2 - robotXPos;
	float deltaZ = canZ - robotZPos;
	float d = sqrt(pow(deltaX, 2) + pow(deltaZ, 2));
	cannonAngle = asin(deltaX / d) * 180 / M_PI; 
	
	
}

bool Enemy::shoot(float* angleLR, float* angleUD, float* x, float* y, float* z, float* offset) {
	if (dead || done || shootWait > 0) return false;
	*angleLR = 180 - cannonAngle;
	*angleUD = -3;
	*x = robotXPos + 2.3;
	*y = -4;
	*z = robotZPos + 1.5;
	*offset = 0;
	shootWait = 60;
	return true;



}




