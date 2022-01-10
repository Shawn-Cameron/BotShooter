#pragma once

const int vWidth = 1200;    // Viewport width in pixels
const int vHeight = 800;    // Viewport height in pixels


bool stop = true;
int enemySpawnWait = 0;
int shootWait = 0;
bool firstPerson = false;
float posLimit = 30;
float gunAngLim = 60;



//Cannon properties
float cannonRad = 2;
float cannonHeight = 8;//4;
float cannonPosX = 0;
float cannonPosZ = 6;
float cannonHeadRad = cannonRad * 1.5;
float cannonHeadHight = 1.2;
float barrelLen = 5;
float barrelRad = cannonHeadHight * 0.5;
float gunAngle = 0;
bool cannonHit = false;


// Light properties

GLfloat light_position0[] = { -4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_position1[] = { 4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

GLfloat cannon_mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat cannon_mat_diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat cannon_mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat cannon_mat_shininess[] = { 100.0f };

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);
void computePath(Vector2D *path, float startX);
void createEnemy(float startX, int index);
void enemyHandler(void);
void importTexture(GLuint texture, const char* file);
void drawCannonBase();
void drawCannonHead();
void projectileHandler();
void shootCannon(void);
void addBullet(bullet obj);
void reset();
