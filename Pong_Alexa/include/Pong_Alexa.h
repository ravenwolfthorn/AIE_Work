////////////////////////////////////////////////////////////
// File: Pong_Alexa.h
// Author: Alexandra McMannis
// Date Created: September 16, 2013
// Brief: Function declarations for basic pong game, matching Pong_Alexa.cpp
////////////////////////////////////////////////////////////

#ifndef _PONG_ALEXA_H_
#define _PONG_ALEXA_H_

//STRUCTURES///////////////////////////////////////////////////////////////////////////////////////////
struct vector2
{
	float fX;
	float fY;
};

struct movableObject
{
	vector2 v2Position;
	vector2 v2Speed;
	int iSprite;
	int iWidth;
	int iHeight;
};
//END STRUCTURES***************************************************************************************

//FUNCTION DECLARATION/////////////////////////////////////////////////////////////////////////////////
vector2 vectorSubtract(vector2 &v, float fS);
vector2 vectorAdd(vector2 &v, float fS);
vector2 multiplyScalar(vector2 &v, float fS);
vector2 vectorSubtract(vector2 &v, vector2 &v2);
vector2 vectorAdd(vector2 &v, vector2 &v2);
vector2 getNormal(vector2 &v);
float getMagnitude(vector2 &v);

void sort(int array[], int iSize);
bool testOnScreen(movableObject& obj, int iW, int iH);
void updateBallPosition(movableObject &obj);
bool updatePowerUp(movableObject &objPowerUp, movableObject &objBall, movableObject &objPlayer, bool bPowerUpVisible);
bool checkPaddleCollision(movableObject& objBall, movableObject& objPlayer);
bool checkCollision(movableObject& obj1, movableObject& obj2, bool bBall);
void seek(movableObject &objPlayer, movableObject& objBall);
void movePlayer(movableObject& objPlayer);
bool checkExit();
//END FUNCTION DECLARATION****************************************************************************
#endif