#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>
#include <gl/glut.h>
#include <utility>
#include <vector>
#include "ShaderUtil.h"
#include "VECTOR3D.h"
#include "path.h"
#include "QuadMesh.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Main.h"

#include <iostream>
using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"


#define MAX_ENEMIES 3
#define NUM_OF_TEXTURES 10
#define MAX_BULLETS 200

// A flat open mesh
QuadMesh* groundMesh = NULL;

Enemy enemyBots[MAX_ENEMIES] = {};
int numEnemies = 0;

bullet bullets[MAX_BULLETS] = {};
int numBullets = 0;

GLuint textures[NUM_OF_TEXTURES] = {};

// Default Mesh Size
int meshSize = 16;

//Shader
ShaderUtil shaderUtil;



int main(int argc, char** argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("Robot Shooter");

	// Initialize GL
	initOpenGL(vWidth, vHeight);

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);

	//
	GLint glewRes = glewInit();
	if (glewRes != GLEW_OK) {
		cout << "Error: " << glewGetErrorString(glewRes);

	}
	else {
		//shaderUtil.Load("vs.vert", "fs.frag");
		//shaderUtil.Use();	
	}


	// Start event loop, never returns
	glutMainLoop();

	return 0;
}

void initOpenGL(int w, int h)
{
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);   

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);  
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   

	importTexture(textures[0], "Textures/whiteCamo.png");
	importTexture(textures[1], "Textures/tire2.png");
	importTexture(textures[2], "Textures/snow.png");
	importTexture(textures[3], "Textures/metal.png");
	importTexture(textures[4], "Textures/eye.png");
	importTexture(textures[5], "Textures/blackLines.png");
	importTexture(textures[6], "Textures/roundmetal.png");
	importTexture(textures[7], "Textures/cannon.png");
	importTexture(textures[8], "Textures/stoneWall.png");
	importTexture(textures[9], "Textures/bulletTex.png");


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	// Other initializatuion
	// Set up ground quad mesh
	VECTOR3D origin = VECTOR3D(-16.0f, 0.0f, 16.0f);
	VECTOR3D dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
	VECTOR3D dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
	groundMesh = new QuadMesh(meshSize, 32.0);
	groundMesh->InitMesh(meshSize, origin, 62.0, 82.0, dir1v, dir2v);

	VECTOR3D ambient = VECTOR3D(1.0f, 1.0f, 1.0f);
	VECTOR3D diffuse = VECTOR3D(1.0f, 1.0f, 1.0f);
	VECTOR3D specular = VECTOR3D(1.0f, 1.0f, 1.0f);
	float shininess = 80;
	groundMesh->SetMaterial(ambient, diffuse, specular, shininess);

	srand(time(NULL));
	glutTimerFunc(1000, animationHandler, 0);
	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	

	glLoadIdentity();
	if (firstPerson) 
		gluLookAt(cannonPosX, -3.0, 18, cannonPosX + 20 * sin(gunAngle * M_PI / 180), 0.0, 0.0, 0.0, 1.0, 0.0);
	else gluLookAt(cannonPosX, 5.0, 41.0, cannonPosX, 0.0, 0.0, 0.0, 1.0, 0.0);
	

	enemyHandler();
	projectileHandler();
	drawCannonBase();


	// Draw Enemies and bullets
	for (int i = 0; i < numEnemies; i++) {
		enemyBots[i].drawRobot();
	}

	for (int i = 0; i < numBullets; i++) {
		bullets[i].drawBullet();
	}
	
	// Draw ground
	glPushMatrix();
	glTranslatef(-45.0, -14.0, -7.0);
	glScalef(3, 4.0, 1.5);
	groundMesh->DrawMesh(meshSize);
	glPopMatrix();
	

	glutSwapBuffers();   

}

void reshape(int w, int h)
{
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 120.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 41.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {
	
	switch (key) {

	case 'a':
	case 'A':
		if (cannonHit) return;
		cannonPosX -= 0.65;
		if (cannonPosX < -posLimit) cannonPosX = -posLimit;
		break;

	case 'd':
	case 'D':
		if (cannonHit) return;
		cannonPosX += 0.65;
		if (cannonPosX > posLimit) cannonPosX = posLimit;
		break;
	case ' ':
		shootCannon();
		break;

	case 'r':
	case 'R':
		reset();
		break;

	case 'f':
	case 'F':
		if (firstPerson) firstPerson = false;
		else firstPerson = true;
	}
	glutPostRedisplay();
}

void animationHandler(int param) {

	for (int i = 0; i < numEnemies; i++) {
		enemyBots[i].animate(cannonPosX,cannonPosZ+10);
	}

	for (int i = 0; i < numBullets; i++) {
		bullets[i].animate();
	}
	
	glutPostRedisplay();
	glutTimerFunc(20, animationHandler, 0);
	
}

void functionKeys(int key, int x, int y)
{

	// Help key
	if (key == GLUT_KEY_F1)
	{
		printf("\n------Controls------");
		printf(
			"\n A     -- Move Left"
			"\n D     -- Move Right"
			"\n Left  -- Turn Gun Left"
			"\n Right -- Turn Gun Right"
			"\n F     -- Toggle First Person"
			"\n R     -- Reset Game\n"
		);
	}

	//Cannon barrel movement
	else if (key == GLUT_KEY_LEFT) {
		if (cannonHit) return;
		gunAngle -= 2.0;
		if (gunAngle < -gunAngLim) gunAngle = -gunAngLim;
	}

	else if (key == GLUT_KEY_RIGHT) {
		if (cannonHit) return;
		gunAngle += 2.0;
		if (gunAngle > gunAngLim) gunAngle = gunAngLim;
	}


	glutPostRedisplay();  
}

void computePath(Vector2D *path,float startX) {
	float startZ = -82;
	float endZ = 2;
	float x, z;
	

	float deltaZ = (endZ - startZ) / (NUM_PATH_POINTS - 1);
	float period = (2* M_PI) /(endZ - startZ);
	float ampl = rand() % 31 - 15;
	
	if (ampl == 0) ampl ++;

	for (int i = 0; i < NUM_PATH_POINTS; i++) {
		z = startZ + deltaZ * i;
		x = ampl * sin(period * z) + startX;
		Vector2D temp(x,z);
		path[i] = temp;
	}

}

void createEnemy(float startX, int enemyNum = numEnemies) {
	if (enemyNum >= MAX_ENEMIES) return;

	Enemy temp;
	computePath(temp.path, startX);
	temp.initEnemy();

	enemyBots[enemyNum] = temp;

	if (enemyNum == numEnemies) numEnemies++;

}

void enemyHandler() {
	
	float startX;

	if (enemySpawnWait == 0 && numEnemies < MAX_ENEMIES) {
		startX = rand() % 120 - 60;

		createEnemy(startX);
		enemySpawnWait = 100;
	}

	for (int i = 0; i < numEnemies; i++) {
		if (enemyBots[i].done && enemySpawnWait <= 0) {
			startX = rand() % 120 - 60;
			createEnemy(startX,i);
			enemySpawnWait = 100;
		}

		int num = rand() % 100 + 1;
		if (num <= 1) {
			float angleLR, angleUD, x, y, z, offset;
			if (!enemyBots[i].shoot(&angleLR, &angleUD, &x, &y, &z, &offset)) continue;
			bullet newBullet(angleLR, angleUD, x, y, z, offset);
			addBullet(newBullet);
			
		}

	}
	
	enemySpawnWait--;
}

void importTexture(GLuint texture, const char* file) {
	int width, height, channels;
	unsigned char* img = stbi_load(file, &width, &height, &channels, STBI_rgb_alpha);

	if (img == NULL) {
		printf("Failed to load image");
		exit(1);
	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D, 0);


	stbi_image_free(img);

}

void drawCannonBase() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, cannon_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cannon_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cannon_mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, cannon_mat_shininess);

	glPushMatrix();
	
	glRotatef(90, 1, 0, 0);
	glTranslatef(cannonPosX, 18, cannonPosZ); 

	glBindTexture(GL_TEXTURE_2D, 9);
	GLUquadric* base = gluNewQuadric();
	gluQuadricTexture(base, GL_TRUE);
	gluCylinder(base, cannonRad, cannonRad, cannonHeight, 20, 20);
	gluDeleteQuadric(base);
	glBindTexture(GL_TEXTURE_2D, 0);

	drawCannonHead();
	glPopMatrix();
}

void drawCannonHead() {

	glPushMatrix();
	
	glBindTexture(GL_TEXTURE_2D, 9);
	GLUquadric* head = gluNewQuadric();
	gluQuadricTexture(head, GL_TRUE);

	glPushMatrix();
	glTranslatef(0, 0, -cannonHeadHight);
	
	gluDisk(head, 0, cannonHeadRad, 20, 20);
	gluCylinder(head, cannonHeadRad, cannonHeadRad, cannonHeadHight, 20, 20);

	//gun
	glPushMatrix();
	if (cannonHit) glRotatef(-45, 1, 0, 0);

	glRotatef(gunAngle, 0, 0, 1);
	glTranslatef(0, -cannonHeadRad * 0.88, barrelRad);
	glRotatef(90, 1, 0, 0);

	gluCylinder(head, barrelRad, barrelRad, barrelLen, 20, 20);
	
	glPopMatrix();
	glPopMatrix();

	gluDisk(head, 0, cannonHeadRad, 20, 20);

	gluDeleteQuadric(head);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	
}

void shootCannon() {
	if (cannonHit || shootWait > 0) return;
	bullet newBullet(gunAngle, 0, cannonPosX, -5.5, 18, barrelLen+1); //18
	addBullet(newBullet);
	shootWait = 40;
}

void addBullet(bullet obj) {
	for (int i = 0; i < numBullets; i++) {
		if (bullets[i].dropped()) {
			bullets[i] = obj;
			return;
		}
	}
	if (numBullets >= MAX_BULLETS) return;
	bullets[numBullets] = obj;
	numBullets++;
}

void projectileHandler() {
	shootWait--;
	for (int i = 0; i < numBullets; i++) {
		if (bullets[i].dropped()) continue;
		if (bullets[i].collision(cannonPosX, cannonRad, -9, cannonHeight, 18, cannonRad)) {
			bullets[i].active = false;
			cannonHit = true;
			continue;
		}
		for (int j = 0; j < numEnemies; j++) {
			Enemy temp = enemyBots[j];
			if (temp.dead) continue;
			if (bullets[i].collision(temp.robotXPos + 2.3, 3, -5.5, 1, temp.robotZPos,2)) {
				bullets[i].active = false;
				enemyBots[j].dead = true;
			}
		}
	}
}

void reset() {
	for (int i = 0; i < numEnemies; i++) {
		enemyBots[i].done = true;
	}
	for (int i = 0; i < numBullets; i++) {
		bullets[i].active = false;
	}
	gunAngle = 0;
	cannonPosX = 0;
	cannonHit = false;

	
}