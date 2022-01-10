#pragma once
#include "path.h"


class Enemy {

public:
	Vector2D path[NUM_PATH_POINTS] = {};
	bool done = false;
	float robotXPos = 0;
	float robotZPos = 0;
	bool dead = false;
	

private:
	int shootWait = 0;
	int nextPoint = 0;

	float sizeFactor = 0.5;

	float robotBodyWidth = 8.0;
	float robotBodyLength = 18.0;
	float robotBodyDepth = 6.0;

	float movementSpeed = 0.2;
	float robotDirAngle = 0;
	float directionVectX = 0;
	float directionVectZ = 0;
	

	float cannonAngle = 0;
	float cannonLimit = 50;

	float rodWidth = 0.1 * robotBodyWidth;
	float wheelRad = 0.4 * robotBodyWidth;
	float wheelWidth = 0.25 * robotBodyWidth;
	float wheelRotationVal = 0;

	float headWidth = robotBodyWidth * 0.16;
	float headLength = robotBodyLength * 0.04;
	float headDepth = robotBodyDepth * 0.19;

	float cannonBaseWidth = headWidth * 0.32; 
	float cannonBaseLength = headLength * 0.6; 
	float cannonBaseDepth = headDepth * 0.26;  

	float cannonRad = cannonBaseWidth * 4.36;
	float cannonLength = robotBodyLength * 0.89;

	float deathAngle = 0;
	float deathWait = 50;

	



public:
	void drawRobot();
	void animate(float canX, float canZ);
	void initEnemy();
	bool shoot(float *angleLR, float *angleUD,float *x, float *y,float *z, float *offset);

private:
	void solidCube(float size, int textID);
	void solidTorus(float r, float R, int sides, int rings, int textID);
	void drawBody();
	void drawWheel();
	void drawWheelCovers();
	void drawWheelCover();
	void drawHead();
	void drawHeadmain();
	void drawEye();
	void drawCannon();
	void computeAngle(float nextX, float nextZ);
	void computeCannonAngle(float canX, float canZ);
	

 
};

