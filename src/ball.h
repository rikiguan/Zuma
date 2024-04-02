
#pragma once

#ifndef _BALL_H_
#define _BALL_H_

#include <graphics.h>

#define BALLRADIUS 15

struct ball
{
	float x;
	float y;
	int c;
	float cx;
	float cy;
	float ballDirction;
};


#endif