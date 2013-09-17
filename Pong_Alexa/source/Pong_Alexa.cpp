////////////////////////////////////////////////////////////
// File: Pong_Alexa.cpp
// Author: Alexandra McMannis
// Date Created: September 16, 2013
// Brief: Function Implementations for basic pong game, matching Pong_Alexa.h
////////////////////////////////////////////////////////////

#include "AIE.h"
#include "KeyCodes.h"
#include "Pong_Alexa.h"
#include <string>
#include <cmath>

//FUNCTION IMPLEMENTATIONS//////////////////////////////////////////////////////////

//global variables (shared between functions)///////////////////////////////////////
int g_iPlayerSpeed = 3;
bool g_bPowerUp = true;
bool g_bPowerUpON = false;
int g_iPowerUpCount = 0;

//subtract SCALAR from VECTOR2, return a VECTOR2
vector2 vectorSubtract(vector2 &v, float fS)
{
	vector2 vResult = {v.fX - fS, v.fY - fS};
	return vResult;
}

//adds a SCALAR from VECTOR2, return a VECTOR2
vector2 vectorAdd(vector2 &v, float fS)
{
	vector2 vResult = {v.fX + fS, v.fY + fS};
	return vResult;
}

//multiplies a VECTOR2 by a SCALAR, returns VECTOR2
vector2 multiplyScalar(vector2 &v, float fS)
{
	vector2 vResult = {v.fX * fS, v.fY * fS};
	return vResult;
}

//subtract VECTOR2 from VECTOR2, return a VECTOR2
vector2 vectorSubtract(vector2 &v, vector2 &v2)
{
	vector2 vResult = {v.fX - v2.fX, v.fY - v2.fY};
	return vResult;
}

//add a VECTOR2 to a VECTOR2, return a VECTOR2
vector2 vectorAdd(vector2 &v, vector2 &v2)
{
	vector2 vResult = {v.fX + v2.fX, v.fY + v2.fY};
	return vResult;
}

//get the normal of a VECTOR2, return a VECTOR2
vector2 getNormal(vector2 &v)
{
	float fMag = sqrt(v.fX*v.fX + v.fY*v.fY);
	vector2 vResult = {v.fX/fMag, v.fY/fMag};
	return vResult;
}

//get the magnitude of a VECTOR2
float getMagnitude(vector2 &v)
{
	return sqrt(v.fX*v.fX + v.fY*v.fY);
}

//sort array from high - low
void sort(int array[], int iSize)
{
	for( int i = 0; i < iSize; i++ )
	{
		int iLoop = i + 1;
		while( iLoop != iSize )
		{
			if( i < iSize - 1 && array[i] < array[iLoop] )
			{
				int iTemp = array[i];
				array[i] = array[iLoop];
				array[iLoop] = iTemp;
			}
			iLoop++;
		}
	}
}

//test if obj is on-screen or not
bool testOnScreen(movableObject& obj, int iW, int iH)
{
	if( obj.v2Position.fX > iW )
	{
		obj.v2Speed.fX *= -1;
		return true;
	}
	if( obj.v2Position.fX < 0 )
	{
		obj.v2Speed.fX *= -1;
		return true;
	}
	if( obj.v2Position.fY > iH )
	{
		obj.v2Speed.fY *= -1;
	}
	if( obj.v2Position.fY < 0 )
	{
		obj.v2Speed.fY *= -1;
	}
	return false;
}

//update the ball's position on screen
void updateBallPosition(movableObject &obj)
{
	obj.v2Position = vectorAdd(obj.v2Position, obj.v2Speed);
}

//update the power-up's status: Is it activated or not?
bool updatePowerUp(movableObject &objPowerUp, movableObject &objBall, movableObject &objPlayer, bool bPowerUpVisible)
{
	g_bPowerUp = bPowerUpVisible;

	if( g_bPowerUpON == true )
	{
		g_iPlayerSpeed = 8;
		g_iPowerUpCount++;
	}
	else
	{
		g_iPlayerSpeed = 5;
	}

	//is the ball and powerup colliding?
	if( checkCollision(objPowerUp, objBall, true) && g_bPowerUp == true )
	{
		//we've collided, so get rid of it
		DestroySprite(objPowerUp.iSprite);
		//powerup is currently running
		g_bPowerUpON = true;
		//powerup is not visible
		g_bPowerUp = false;
	}

	if( g_iPowerUpCount >= 20000 )
	{
		//turn off the powerup
		g_bPowerUpON = false;
		//reset the counter
		g_iPowerUpCount = 0;
	}

	//return whether the powerup is visible or not so we know whether to draw it
	return g_bPowerUp;
}

//check to see if the ball has collided with the paddle, return true if it did collide
bool checkPaddleCollision(movableObject& objBall, movableObject& objPlayer)
{
	int iSpeed = objBall.v2Speed.fX;

	int iBallHalfW = objBall.iWidth/2;
	int iPlayerHalfW = objPlayer.iWidth/2;
	int iPlayerHalfH = objPlayer.iHeight/2;

	//if the ball isn't moving, it's obviously not about to collide
	if( iSpeed == 0 )
	{
		return false;
	}

	//ball is moving left
	if( iSpeed < 0 )
	{
		if(objBall.v2Position.fX - iBallHalfW < objPlayer.v2Position.fX + iPlayerHalfW &&
			objBall.v2Position.fY > objPlayer.v2Position.fY - iPlayerHalfH &&
			objBall.v2Position.fY < objPlayer.v2Position.fY + objPlayer.iHeight)
		{
			return true;
		}
	}
	//ball is moving right
	else
	{
		if(objBall.v2Position.fX + iBallHalfW > objPlayer.v2Position.fX - iPlayerHalfW &&
			objBall.v2Position.fY > objPlayer.v2Position.fY - iPlayerHalfH &&
			objBall.v2Position.fY < objPlayer.v2Position.fY + objPlayer.iHeight)
		{
			return true;
		}
	}
	return false;
}

//check if two objects have collided with one another, return true if collided
bool checkCollision(movableObject& obj1, movableObject& obj2, bool bBall)
{
	vector2 vDifference = vectorSubtract(obj1.v2Position, obj2.v2Position);
	float fMag = getMagnitude(vDifference);

	if( fMag > 0 && fMag < obj1.iHeight )
	{
		if( !bBall )
		{
			//collide
			obj1.v2Speed = multiplyScalar( getNormal(vDifference), 5 );
			obj2.v2Speed = multiplyScalar( getNormal(vDifference), -5 );
		}
		return true;
	}
	return false;
}

//ai tracking of ball
void seek(movableObject &objPlayer, movableObject& objBall)
{
	float fSqrt = sqrt(objBall.v2Speed.fX*objBall.v2Speed.fX + objBall.v2Speed.fY*objBall.v2Speed.fY);
	int iSpeed = 3;

	if( objBall.v2Position.fY - fSqrt > objPlayer.v2Position.fY )
	{
		objPlayer.v2Speed.fY =  iSpeed;
	}
	else if( objBall.v2Position.fY + fSqrt < objPlayer.v2Position.fY )
	{
		objPlayer.v2Speed.fY = -1 * iSpeed;
	}
	else
	{
		objPlayer.v2Speed.fY = 0;
	}

	objPlayer.v2Position.fY += objPlayer.v2Speed.fY;

}

void movePlayer(movableObject& objPlayer)
{

	if( IsKeyDown(KEY_UP))
	{
		//UP key is pressed
		objPlayer.v2Speed.fY = -1 * g_iPlayerSpeed;
	}
	else if( IsKeyDown(KEY_DOWN) )
	{
		//DOWN key is pressed
		objPlayer.v2Speed.fY =  g_iPlayerSpeed;
	}
	else
	{
		objPlayer.v2Speed.fY = 0;
	}

	objPlayer.v2Position = vectorAdd(objPlayer.v2Position,objPlayer.v2Speed);

}

bool checkExit()
{
	if( IsKeyDown(KEY_ESC) )
	{
		//ESCAPE key is pressed
		return true;
	}
	return false;
}