#pragma once
#include "ballControler.h"
#include "ball.h"
#include "LinkList.h"
#include "math.h"
#include "conf.h"


void drawBallList(Node* head);
void drawSingleBall(ball b);

void initBallList(Node* head);
void updateBallPos(Node* head);
void updateBallCurrnetPos(Node* head, int pulse);

void updateBallColPos(Node* ahead, int speed, int pulse);
void collisionDetection(Node* head, Node* chead);

