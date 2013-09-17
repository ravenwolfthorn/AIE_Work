////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Alexandra McMannis
// Date Created: September 16, 2013
// Brief: game dependent functions and main game loop for basic pong game
////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#include "AIE.h"
#include "KeyCodes.h"
#include "Pong_Alexa.h"
#include <string>
#include <cmath>
#include <time.h>
#include <assert.h>
#include <crtdbg.h>
#include <iostream>

//CONSTANTS////////////////////////////////////////////////////////////////////////////////////
const int SPEED = 0;

const int SCREEN_X = 1280;
const int SCREEN_Y = 780;

const int PLAYER1_X = 100;
const int PLAYER1_W = 100;
const int PLAYER1_H = 144;

const int PLAYER2_X = 1200;
const int PLAYER2_W = 83;
const int PLAYER2_H = 128;

const int BALL_W = 64;
const int BALL_H = 92;

//global variables TEMPORARY*******************************************************************
unsigned int g_uiBgImage = -1;
movableObject g_player1 = {PLAYER1_X, 100, 0, 20, -1 , PLAYER1_W, PLAYER1_H};
movableObject g_player2 = {PLAYER2_X, 100, 0, 20, -1, PLAYER2_W, PLAYER2_H};
movableObject g_ball = {500, 500, 2, 2, -1, BALL_W, BALL_H};
movableObject g_powerUp = {500, 500, 0, 0, -1, BALL_W, BALL_H};
int g_aiHighScores[5] = {0};
bool g_bGameOver = false;

int g_iPlayer1Score = 0;
int g_iPlayer2Score = 0;
int g_iFrameCounter = 0;

//TEMP set to true to make it show up immediately
bool g_bPowerUpVis = true;

//initialize all variables in the game
void initGame()
{
	srand(time(NULL));

	//load the sprites
	g_uiBgImage = CreateSprite("./images/bg.png", SCREEN_X, SCREEN_Y, true );
	MoveSprite(g_uiBgImage, SCREEN_X / 2, SCREEN_Y / 2);

	g_player1.iSprite = CreateSprite( "./images/player1.png", PLAYER1_W, PLAYER1_H, true );
	g_player2.iSprite = CreateSprite( "./images/player2.png", PLAYER2_W, PLAYER2_H, true );
	g_ball.iSprite = CreateSprite( "./images/ball.png", BALL_W, BALL_H, true );
	g_powerUp.iSprite = CreateSprite( "./images/ball.png", BALL_W, BALL_H, true );
}

//destroy all sprites and clean up the memory
void destroyGame()
{
	//DestroySprite(g_uiBgImage);
	DestroySprite(g_player1.iSprite);
	DestroySprite(g_player2.iSprite);
	DestroySprite(g_ball.iSprite);
	DestroySprite(g_powerUp.iSprite);
}

//update the game logic here
void updateGame()
{
	if( g_bGameOver != true )
	{
	updateBallPosition(g_ball);
	}

	if( !g_bGameOver )
	{
		//player 1 on auto
		seek(g_player1, g_ball);
	
		//input for player 2
		movePlayer(g_player2);
	}

	if( testOnScreen(g_ball, SCREEN_X, SCREEN_Y) )
	{
		//ball hit side of screen
		if(g_ball.v2Position.fX < 100 )
		{
			//player 1 lost
			g_iPlayer2Score++;
		}
		else
		{
			//player 2 lost
			g_iPlayer1Score++;
		}
	}

	if( g_bGameOver == false )
	{
		if( abs(g_iPlayer1Score - g_iPlayer2Score) >= 3 )
		{
			//game over
			g_bGameOver = true;

			int iHighestScore = g_iPlayer1Score;
			if( g_iPlayer2Score > g_iPlayer1Score )
			{
				iHighestScore = g_iPlayer2Score;
			}

			for(int i = 0; i < 5; i++ )
			{
				if( iHighestScore > g_aiHighScores[i])
				{
					g_aiHighScores[i] = iHighestScore;
					break;
				}
			}

			//sort the scores array
			sort(g_aiHighScores, 5);
		}
	}

	if( g_ball.v2Speed.fX < 0 )
	{
		if(checkPaddleCollision(g_ball, g_player1) == true)
		{
			g_ball.v2Speed.fX *= -1;
		}
	}
	else
	{
		if( checkPaddleCollision(g_ball, g_player2) == true)
		{
			g_ball.v2Speed.fX *= -1;
		}
	}

	RotateSprite(g_player1.iSprite, 0);
	MoveSprite(g_player1.iSprite, g_player1.v2Position.fX, g_player1.v2Position.fY );

	RotateSprite(g_player2.iSprite, 0);
	MoveSprite(g_player2.iSprite, g_player2.v2Position.fX, g_player2.v2Position.fY);

	MoveSprite(g_ball.iSprite, (int)g_ball.v2Position.fX, (int)g_ball.v2Position.fY);
}

//draw each frame of the game
void drawGame()
{
	static char s_cOutScore1[15] = {'\n'};
	static char s_cOutScore2[15] = {'\n'};

	//DrawSprite(g_uiBgImage);

	DrawSprite(g_ball.iSprite);
	DrawSprite(g_player1.iSprite);
	DrawSprite(g_player2.iSprite);

	sprintf(s_cOutScore1, "Player 1: %d", g_iPlayer1Score);
	sprintf(s_cOutScore2, "Player 2: %d", g_iPlayer2Score);
	//draw the scores
	DrawString(s_cOutScore1, 50, 50, SColour(255,0,0,255));
	DrawString(s_cOutScore2, 1000, 50, SColour(0,255,0,255));

	if(g_iFrameCounter >= 20 && g_bPowerUpVis != true )
	{
		g_bPowerUpVis = true;
	}

	if( g_bPowerUpVis = true )
	{
		if( updatePowerUp(g_powerUp,g_ball,g_player1,g_bPowerUpVis) )
		{
			DrawSprite(g_powerUp.iSprite);
		}
	}

	if(g_bGameOver == true)
	{
		DrawString("Game Over", SCREEN_X / 2, 300);

		char acScore[15];
		int iY = 350;
		for(int i = 0; i < 5; i++)
		{
			if(g_aiHighScores[i] != 0 )
			{
				sprintf(acScore, "player %d: %d", i+1, g_aiHighScores[i]);
				DrawString(acScore, SCREEN_X / 2, iY);
				iY += 30;
			}
		}

		vector2 v2BallPosition = {SCREEN_X / 2, 300};
		g_ball.v2Position = v2BallPosition;
		DestroySprite(g_ball.iSprite);
		DrawSprite(g_ball.iSprite);
		g_ball.v2Speed.fX = 0;
		g_ball.v2Speed.fY = 0;

	}

	
}

int main( int argc, char* argv[] )
{		
	//Create the game framework
	Initialise(SCREEN_X, SCREEN_Y, false);

	initGame();

	do {
		g_iFrameCounter++;

		if(g_iFrameCounter > 5000)
		{
			g_iFrameCounter = 0;
		}

		ClearScreen();
		updateGame();
		drawGame();

		if( checkExit() )
		{
			break;
		}
		
	} while( FrameworkUpdate() == false );

	destroyGame();

	Shutdown();

	return 0;
}