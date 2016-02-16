
/**
Implementa a movimentação simples de uma camera em primeira pessoa para um personagem que anda
sobre um plano.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <gl/glut.h>
#include <memory>
#include <vector>

//bitmap class to load bitmaps for textures
#include "bitmap.h"

#include "Map.h"

#include "TempMotionBlock.h"
#include "MotionBlock.h"

//handle generic obj models
#include "3DObject.h"

#include "Timer.h"

#include "Item.h"

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "alut.lib")

#define PI 3.14159265

void mainInit();
void initTexture();
void initLight();
void createGLUI();
void mainRender();
void mainCreateMenu();
void onMouseButton(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void onGLUIEvent(int id);
void onWindowReshape(int x, int y);
void mainIdle();
int main(int argc, char **argv);
void setWindow();
void setViewport(GLint left, GLint right, GLint bottom, GLint top);
void updateState();
void renderFloor();
void updateCam();
void renderMinimap();
void cutSceneMinimap(float x, float y, float w, float h);

void drawTextOnScene(char *string);
void glEnable2D();
void glDisable2D();
void renderMapAndTimer();
void initTempMBlocks();


/**
Screen dimensions
*/
int windowWidth = 600;
int windowHeight = 650;

/**
Screen position
*/
int windowXPos = 300;
int windowYPos = 50;

int mainWindowId = 0;

double xOffset = -1.9;
double yOffset = -1.3;
int mouseLastX = 0;
int mouseLastY = 0;
bool loadedModels = false;
float roty = 0.0f;
float rotx = 90.0f;

bool mBlockCreated[5] = {false, false, false, false, false};
bool activeMBlocks[5] = {false, false, false, false, false};

bool rightPressed = false;
bool leftPressed = false;
bool upPressed = false;
bool shiftPressed = false;
bool downPressed = false;
bool spacePressed = false;

bool changeCameraView = false;
int viewCameraNumber=3;
int gameResult =3;
int go = 0;

float speedX = 0.0f;
float speedY = 0.0f;
float speedZ = 0.0f;

float posX = 0.0f;
float posY = 0.0f;
float posZ = 2.0f;
float rotation = 0.0f;
GLfloat light_2_spot_direction[]={0.0,1.0,-1.0};

/*
variavel auxiliar pra dar variação na altura do ponto de vista ao andar.
*/
float headPosAux = 0.0f;

float maxSpeed = 0.25f;

float planeSize = 8.0f;



// parte de código extraído de "texture.c" por Michael Sweet (OpenGL SuperBible)
// texture buffers and stuff
int i;                       /* Looping var */
BITMAPINFO	*info;           /* Bitmap information */
GLubyte	    *bits;           /* Bitmap RGB pixels */
GLubyte	    *bitsOfModelMap;
GLubyte     *ptr;            /* Pointer into bit buffer */
GLubyte	    *rgba;           /* RGBA pixel buffer */
GLubyte	    *rgbaptr;        /* Pointer into RGBA buffer */
GLubyte     temp;            /* Swapping variable */
GLenum      type;            /* Texture type */
GLuint      texture[6];         /* Texture object */

bool crouched = false;
bool running = false;
bool jumping = false;
float jumpSpeed = 0.06;
float gravity = 0.004;
float heightLimit = 0.2;
float posYOffset = 1.0;

float backgrundColor[4] = {1.0f,1.0f,1.0f,1.0f};

Map map = Map();

CTimer goTimer[5];
Coordinate mBlocksCordinate[5];
clock_t beginBlockTime;
int numberOfCreatedMBlocks = 0;

Coordinate itemCoordinate;
TempMotionBlock tempMBlocks[5];

void setWindow()
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(posX+ 0.5*sin(roty*PI/180),posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)),posZ-0.5*cos(roty*PI/180),
		posX + 2*sin(roty*PI/180),posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),posZ -2*cos(roty*PI/180),
		0.0,1.0,0.0);
}

/**
Atualiza a posição e orientação da camera
*/
void updateCam()
{
	if(viewCameraNumber==3) {
		gluLookAt(posX+ 0.5*sin(roty*PI/180),posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)),posZ-0.5*cos(roty*PI/180),
			posX + 2*sin(roty*PI/180),posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),posZ -2*cos(roty*PI/180),
			0.0,1.0,0.0);
	}else if(viewCameraNumber==1) {
		setViewport(0, windowWidth, 0, windowHeight);
		gluLookAt(0.0, map.width()+10, 0.001, //eye
			0.0, 1.0, 0.0, //center
			0.0, 1.0, 0.0); //up
	}else if(viewCameraNumber==2){
		gluLookAt(posX- 4.5*sin(roty*PI/180), posY + 3 + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)), posZ+4.5*cos(roty*PI/180),
			posX + 2*sin(roty*PI/180),posY + posYOffset + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),posZ -2*cos(roty*PI/180),
			0.0,1.0,0.0);
	}

}

void initLight()
{
	glEnable(GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHT1 );

	GLfloat light_ambient[] = { backgrundColor[0], backgrundColor[1], backgrundColor[2], backgrundColor[3] };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position1[] = {0.0, 0.0, 0.0, 1.0 };


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
}

void setViewport(GLint left, GLint right, GLint bottom, GLint top)
{
	glViewport(left, bottom, right - left, top - bottom);
}

void initTimer(){
	for(i=0; i<5; i++) {
		if (!goTimer[i].Init()) {
			exit(0);
		}
	}
}

/**
Initialize
*/
void mainInit()
{
	std::srand(std::time(0));

	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);

	// habilita remocao de faces ocultas
	glFrontFace (GL_CCW);
	glEnable(GL_CULL_FACE);

	// habilita o z-buffer
	glEnable(GL_DEPTH_TEST);

	// inicializa textura do chao
	initTexture();

	// carrega o mapa
	map.load();

	// carrega as texturas do mapa
	map.player()->loadTexture(&texture[1]);

	std::vector<std::shared_ptr<Machine>> enemies = map.enemies();
	std::vector<std::shared_ptr<Machine>>::iterator enemiesIt;
	for (enemiesIt = enemies.begin() ; enemiesIt != enemies.end(); ++enemiesIt) {
		(*enemiesIt)->loadTexture(&texture[1]);
	}

	std::vector<std::shared_ptr<MotionBlock>> motionBlocks = map.motionBlocks();
	std::vector<std::shared_ptr<MotionBlock>>::iterator mBlocksIt;
	for (mBlocksIt = motionBlocks.begin() ; mBlocksIt != motionBlocks.end(); ++mBlocksIt){
		(*mBlocksIt)->loadTexture(&texture[5]);
	}

	// configura o tamanho do plano conforme o mapa
	planeSize = map.width();

	posX = map.player()->position().x;
	posY = map.player()->position().y;
	posZ = map.player()->position().z;

	initTempMBlocks();

	initLight();

	initTimer();

	printf("W: Andar pra frente \n");
	printf("S: Andar pra tras \n");
	printf("A: Gira 90 graus anti-horario \n");
	printf("D: Gira 90 graus horario \n");
	printf("Tab: Cria Bloco\n");
	printf("Espaco: Empurra Bloco\n");

}

void initTempMBlocks(){
	for(int i=0; i < 5; i++){
		tempMBlocks[i] = TempMotionBlock();
		tempMBlocks[i].init();
		tempMBlocks[i].loadTexture(&texture[3]);
	}
}

void initTexture(void)
{

	printf ("\nLoading texture..\n");
	// Load a texture object (256x256 true color)
	bits = LoadDIBitmap("..\\resources\\models\\snowedGrass.bmp", &info);
	if (bits == (GLubyte *)0)
	{
		printf ("Error loading texture!\n\n");
		return;
	}


	// Figure out the type of texture
	if (info->bmiHeader.biHeight == 1)
		type = GL_TEXTURE_1D;
	else
		type = GL_TEXTURE_2D;

	// Create and bind a texture object
	glGenTextures(2, &texture[0]);
	glBindTexture(type, texture[0]);

	// Create an RGBA image
	rgba = (GLubyte *)malloc(info->bmiHeader.biWidth * info->bmiHeader.biHeight * 4);

	i = info->bmiHeader.biWidth * info->bmiHeader.biHeight;
	for( rgbaptr = rgba, ptr = bits;  i > 0; i--, rgbaptr += 4, ptr += 3)
	{
		rgbaptr[0] = ptr[2];     // windows BMP = BGR
		rgbaptr[1] = ptr[1];
		rgbaptr[2] = ptr[0];
		rgbaptr[3] = (ptr[0] + ptr[1] + ptr[2]) / 3;
	}

	// Set texture parameters
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(type, 0, 4, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, rgba );
}


void renderFloor()
{
	glPushMatrix();
	glBindTexture(type, texture[0]);

	glShadeModel(GL_SMOOTH);
	glEnable(type);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTranslatef(-(float)planeSize/2.0f, 0.0f, -(float)planeSize/2.0f);

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	int xQuads = 16;
	int zQuads = 16;
	for (int i = 0; i < xQuads; i++)
	{
		for (int j = 0; j < zQuads; j++)
		{
			glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f);   // coords for the texture
			glNormal3f(0.0f,1.0f,0.0f);
			glVertex3f(i * (float)planeSize/xQuads, 0.0f, (j+1) * (float)planeSize/zQuads);

			glTexCoord2f(0.0f, 0.0f);  // coords for the texture
			glNormal3f(0.0f,1.0f,0.0f);
			glVertex3f((i+1) * (float)planeSize/xQuads, 0.0f, (j+1) * (float)planeSize/zQuads);

			glTexCoord2f(0.0f, 1.0f);  // coords for the texture
			glNormal3f(0.0f,1.0f,0.0f);
			glVertex3f((i+1) * (float)planeSize/xQuads, 0.0f, j * (float)planeSize/zQuads);

			glTexCoord2f(1.0f, 1.0f);  // coords for the texture
			glNormal3f(0.0f,1.0f,0.0f);
			glVertex3f(i * (float)planeSize/xQuads, 0.0f, j * (float)planeSize/zQuads);

			glEnd();
		}
	}

	glDisable(type);
	glPopMatrix();
}

void renderScene()
{
	glClearColor(backgrundColor[0],backgrundColor[1],backgrundColor[2],backgrundColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // limpar o depth buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//minimap rendering	
	renderMinimap();

	setViewport(0, windowWidth, 0, 480);
	updateCam();
	map.collectItems();
	renderFloor();
	map.draw();
	renderMapAndTimer();

	if(map.enemies().size()==0){
		gameResult = 0;
		glEnable2D();
			glColor3f(0.2, 0.8, 0);
			drawTextOnScene("You Won!");
		glDisable2D();
	}

	if( map.player() && map.collidesWithEnemy(*(map.player()))) {
		gameResult=1;
		glEnable2D();
			glColor3f(0.2, 0.8, 0);
			drawTextOnScene("Game Over!");
		glDisable2D();
	}
}

void renderMapAndTimer() {

	//blocks timer
	int nSeconds[5];
	
	for(int i = 0; i < 5; i++) {
		nSeconds[i] = (int)goTimer[i].GetTimeSeconds();
	}


	for(int i = 0; i < 5; i++) {
		if(activeMBlocks[i]) {
			goTimer[i].Update();
			if(nSeconds[i] < 2.0 ){
				tempMBlocks[i].setPosition(mBlocksCordinate[i]);
				tempMBlocks[i].draw();
			} else{
				if (!map.collidesWithSomething(tempMBlocks[i])){
					if(mBlockCreated[i] == false){
						map.initMotionBlock(tempMBlocks[i].position(), i)->loadTexture(&texture[5]);	
						mBlockCreated[i]=true;
					}
				} else if (!mBlockCreated[i]){
					activeMBlocks[i] = false;
					numberOfCreatedMBlocks -= 1;
				}
			}
		}
	}
}

void renderMinimap(){
	if(viewCameraNumber==3 || viewCameraNumber==2){
		/* Perspective view */
		//minimap
		glPushMatrix();
		glViewport(windowWidth/2-100, windowHeight-windowHeight/4, windowWidth/3.5, windowHeight/4);
		gluLookAt(0.0, map.width()+10, 0.001, //eye
			0.0, 1.0, 0.0, //center
			0.0, 1.0, 0.0); //up
		renderFloor();
		map.draw();
		renderMapAndTimer();
		glPopMatrix();

	}	
}

void cutSceneMinimap(float x, float y, float w, float h){
	glScissor(x,y,w,h);
    glViewport(x,y,w,h);
}

void drawTextOnScene(char *string)
{
	//glPushMatrix();

	// Posição onde o texto será colocado
	glRasterPos2f(-7.4, 5);

	// Exibe caracter a caracter
	while(*string)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
	}

	//glPopMatrix();
}

void glEnable2D()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix;
	glLoadIdentity;
	gluOrtho2D(0, windowWidth, 0, windowHeight);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix;
	glLoadIdentity;

	glDisable(GL_DEPTH_TEST);
}

void glDisable2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix;
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix;

	glEnable(GL_DEPTH_TEST);
}

void updateState()
{
	if (upPressed || downPressed)
	{
		// efeito de "sobe e desce" ao andar
		headPosAux += 8.5f;
		if (headPosAux > 180.0f)
		{
			headPosAux = 0.0f;
		}

		if (upPressed)
		{
			map.player()->moveForward();
		}
		else
		{
			map.player()->moveBackward();
		}

		if (spacePressed){
			map.player()->moveForward();
			if (map.collidesWithMotionBlock(*map.player()) && !map.getCollidedMotionBlock(*map.player())->isMoving()){
				//destroy motionBlock
				std::shared_ptr<MotionBlock> collidedMotionBlock = map.getCollidedMotionBlock(*map.player());
				collidedMotionBlock->setAngleToGo(map.player()->angle());
				collidedMotionBlock->moveForward();
				if (map.collidesWithWall(*collidedMotionBlock) || map.collidesWithAnotherMBlock(collidedMotionBlock)){
					if (!collidedMotionBlock->isFixed()){
						mBlockCreated[map.getCollidedMotionBlock(*map.player())->index()] = false;
						activeMBlocks[collidedMotionBlock->index()] = false;
						numberOfCreatedMBlocks -= 1;
					}
					if (collidedMotionBlock->hasItem()){
						map.initItem(collidedMotionBlock->position(), collidedMotionBlock->itemType());
					}
					collidedMotionBlock->setIsFixed(false);
					map.eraseCollidedMotionBlock(collidedMotionBlock);
					map.player()->moveBackward();
					map.player()->moveBackward();
				} else {
					collidedMotionBlock->moveBackward();
					map.player()->moveBackward();
					collidedMotionBlock->setIsMoving(true);
				}
			} else {
				map.player()->moveBackward();
			}
		}

		if(map.collidesWithWall(*map.player()) || map.collidesWithEnemy(*map.player()) || map.collidesWithMotionBlock(*map.player())) {
				
			if (upPressed)
			{
				map.player()->moveBackward();
			}
			else
			{
				map.player()->moveForward();
			}
		}

		posX = map.player()->position().x;
		posZ = map.player()->position().z;
	}
	else
	{
		// parou de andar, para com o efeito de "sobe e desce"
		headPosAux = fmod(headPosAux, 90) - 1 * headPosAux / 90;
		headPosAux -= 4.0f;
		if (headPosAux < 0.0f)
		{
			headPosAux = 0.0f;
		}
	}

	posY += speedY;
	posYOffset += 0.01;
	if (posYOffset > 0.2)
	{
		posYOffset = 0.2;
	}

	if(changeCameraView)
	{
		changeCameraView=false;
		if(viewCameraNumber==3) {
			viewCameraNumber=1;
		}else if(viewCameraNumber==1){
			viewCameraNumber=2;
		}else if(viewCameraNumber==2){
			viewCameraNumber=3;
		}
	}

	if(leftPressed)
	{
		map.player()->rotateModel(-90.0f);
		roty -= 90;
		leftPressed=false;
	}

	if(rightPressed)
	{
		map.player()->rotateModel(90.0f);
		roty += 90;
		rightPressed=false;
	}

	if(shiftPressed){
		if(map.player()) {
			if(numberOfCreatedMBlocks < map.player()->mBlocks() ){
				int freePosition = 0;
				for(int i=0; i<5; i++) {
					if(!activeMBlocks[i]) {
						freePosition = i;
						break;
					}
				}
				mBlocksCordinate[freePosition].x = map.player()->position().x + sin(map.player()->angle()*PI/180) * 2;
				mBlocksCordinate[freePosition].y = map.player()->position().y;
				mBlocksCordinate[freePosition].z = map.player()->position().z - cos(map.player()->angle()*PI/180) * 2;
				Block mBlock = Block(mBlocksCordinate[freePosition], 1.0f);
				if (!map.collidesWithSomething(mBlock)){
					goTimer[freePosition].Init();
					activeMBlocks[freePosition] = true;
					numberOfCreatedMBlocks += 1;
				}
			}
		}
		spacePressed=false;
	}

	std::vector<std::shared_ptr<MotionBlock>> motionBlocks = map.motionBlocks();
	std::vector<std::shared_ptr<MotionBlock>>::iterator mBlocksIt;
	for (mBlocksIt = motionBlocks.begin() ; mBlocksIt != motionBlocks.end(); ++mBlocksIt){
		if ((*mBlocksIt)->isMoving()){
			if(map.collidesWithSomething(**mBlocksIt) || map.collidesWithAnotherMBlock(*mBlocksIt)){
				if (map.collidesWithEnemy(**mBlocksIt)){
					map.killWhatCollidesWith(**mBlocksIt);
				} else{
				(*mBlocksIt)->setIsMoving(false);
				(*mBlocksIt)->moveBackward();
				}
			} else {
				(*mBlocksIt)->moveForward();
			}
		}
	}
	map.executeMachine();
	
}

/**
Render scene
*/
void mainRender()
{
	updateState();
	renderScene();
	glFlush();
	glutPostRedisplay();
	Sleep(40);
}

/**
Handles events from the mouse right button menu
*/
void mainHandleMouseRightButtonMenuEvent(int option)
{
	switch (option)
	{
	case 1 :
		exit(0);
		break;
	default:
		break;
	}
}

/**
Create mouse button menu
*/
void mainCreateMenu()
{
	glutCreateMenu(mainHandleMouseRightButtonMenuEvent);
	glutAddMenuEntry("Quit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/**
Key press event handler
*/
void onKeyDown(unsigned char key, int x, int y)
{

	//printf("%d \n", key);
	switch (key)
	{
	case 32: //space
		spacePressed = true;
		break;
	case 119: //w
		upPressed = true;
		break;
	case 115: //s
		downPressed = true;
		break;
	case 97: //a
		leftPressed = true;
		break;
	case 100: //d
		rightPressed = true;
		break;
	case 99: //c
		crouched = true;
		break;
	case 114: //r
		running = true;
		break;
	case 118: //v
		changeCameraView = true;
		break;
	case 9:
		shiftPressed = true;
		break;
	default:
		break;
	}

	//glutPostRedisplay();
}

/**
Key release event handler
*/
void onKeyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 32: //space
		spacePressed = false;
		break;
	case 119: //w
		upPressed = false;
		break;
	case 115: //s
		downPressed = false;
		break;
	case 97: //a
		leftPressed = false;
		break;
	case 100: //d
		rightPressed = false;
		break;
	case 99: //c
		crouched = false;
		break;
	case 114: //r
		running = false;
		break;
	case 27:
		exit(0);
		break;
	case 118: //v
		changeCameraView = false;
		break;
	case 9:
		shiftPressed = false;
		break;
	default:
		break;
	}

	//glutPostRedisplay();
}

void SpecialInput(int key, int x, int y) {

	switch(key) {

	case GLUT_KEY_UP:
		upPressed = true;
		break;	
	case GLUT_KEY_DOWN:
		downPressed = true;
		break;
	case GLUT_KEY_LEFT:
		leftPressed = true;
		break;
	case GLUT_KEY_RIGHT:
		rightPressed = true;
		break;
	}
}

void SpecialInputUp(int key, int x, int y) {
	
	switch(key) {
	case GLUT_KEY_UP:
		upPressed = false;
		break;	
	case GLUT_KEY_DOWN:
		downPressed = false;
		break;
	case GLUT_KEY_LEFT:
		leftPressed = false;
		break;
	case GLUT_KEY_RIGHT:
		rightPressed = false;
		break;
	}
}

void onWindowReshape(int x, int y)
{
	windowWidth = x;
	windowHeight = y;
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);
}

/**
Glut idle funtion
*/
void mainIdle()
{
	/**
	Set the active window before send an glutPostRedisplay call
	so it wont be accidently sent to the glui window
	*/
	glutSetWindow(mainWindowId);
	glutPostRedisplay();


}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(windowXPos,windowYPos);

	/**
	Store main window id so that glui can send it redisplay events
	*/
	mainWindowId = glutCreateWindow("Pengo - Adolfo Schneider");

	glutDisplayFunc(mainRender);

	glutReshapeFunc(onWindowReshape);


	/**
	Register keyboard events handlers
	*/
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialFunc(SpecialInput);
	glutSpecialUpFunc(SpecialInputUp); 

	mainInit();

	glutMainLoop();

	return 0;
}
