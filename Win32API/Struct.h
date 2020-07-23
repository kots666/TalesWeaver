#pragma once

typedef struct tagInfo
{
	float xPos;
	float yPos;
	int xSize;
	int ySize;
}INFO;

typedef struct tagFrame
{
	int startFrame;
	int endFrame;
	int sceneFrame;
	DWORD frameTime;
	DWORD frameSpeed;
}FRAME;