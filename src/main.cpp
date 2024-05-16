#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "LinkList.h"
#include "ball.h"
#include <iostream>
#include <chrono>
#include "ballControler.h"
#include "conf.h"

float speed = ballSpeed;
float difficulty = defultDifficult;
int addBallSpeed = addBallSpeedD;
time_t pulse;
extern int length;
using namespace std;
ball nextBall;

time_t GetCurrentTimeMsec() {
	auto time = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now());
	time_t timestamp = time.time_since_epoch().count();
	return timestamp;
}

int randomSmooth = 0;
int randomSmoothC = 0;

void genNextBallSmooth(ball * next,int x,int y,int random) {
	if ((!randomSmooth--)&& int(6 /difficulty)) {
		randomSmoothC= rand() % 6;
		randomSmooth= rand() % (int(3 / difficulty));
	}
	//randomSmooth--;
	next->c = randomSmoothC;
	if (random) {
		next->c = rand() % 6;
	}
	next->x = x;
	next->y = y;
}


void drawFps(int fps) {
	char fpsDisplay[200];
	sprintf_s(fpsDisplay, "fps: %d | length:%d                           difficulty:%.2f",fps, length,difficulty);
	settextstyle(16, 0, _T("Consolas"));
	settextcolor(txC);
	wchar_t wstr[100];
	mbstowcs_s(0, wstr, fpsDisplay, 100);
	outtextxy(10, 5, wstr);
}


void GameOver(long score) {
	char Display[200];
	sprintf_s(Display, "gameover within %ld s", score);
	settextcolor(txC);
	settextstyle(30, 0, _T("Consolas"));
	wchar_t wstr[100];
	mbstowcs_s(0, wstr, Display, 100);
	outtextxy(WINDOWWITH/2*0.3, WINDOWHEIGHT*0.7, wstr);
}
void addGenBall(Node* head,int random) {
	ball b;
	genNextBallSmooth(&b, WINDOWHEIGHT / 2, 0, random);
	ListInsert(head, 0, b);
	length++;
}

bool tellGameOver(int length) {
	if (length >= 44) {
		return true;
	}
	return false;
}

void difficultyControl(time_t time, time_t timeBegin) {
	addBallSpeed = addBallSpeedD / sqrt(difficulty);
	if (difficulty >= 0.85) {
		difficulty += sqrt((30-length)) * (time - timeBegin) / 1e13;
		//difficulty +=(30 - length) * (time - timeBegin) / 1e13;
	}
	
	speed = ballSpeed / sqrt(difficulty);
}

int main()
{
	int fps = 0;

	// 初始化绘图窗口
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	//setbkcolor(BLACK);
	setbkcolor(bgC);
	cleardevice();

	//初始化球链
	Node* head = NULL;
	head = CreateEmptyList();
	initBallList(head);
	updateBallPos(head);
	drawBallList(head);

	//初始化碰撞球链
	Node* headCol = NULL;
	headCol = CreateEmptyList();

	//事件循环，处理鼠标、时间事件
	MOUSEMSG m;

	time_t time = GetCurrentTimeMsec();
	time_t time1;
	time_t timeBegin=time;
	int delayFor20kMilisec = 0;
	int delayForSec = 0;

	long AvgFps = 0;

	genNextBallSmooth(&nextBall, WINDOWWITH / 2, WINDOWHEIGHT,1);
	while (true)
	{
		//帧率处理函数
		time1 = GetCurrentTimeMsec();
		pulse = time1 - time;
		//pulse = 100;
		time = time1;

		//fps统计
		//fps = 1e6 / pulse;
		AvgFps += 1;
		delayFor20kMilisec += pulse;
		delayForSec += pulse;
		if (delayFor20kMilisec > 500000) {
			fps = AvgFps * 2;
			delayFor20kMilisec %= 500000;
			AvgFps = 0;

			
		}

		if (delayForSec > addBallSpeed) {
			addGenBall(head,0);
			delayForSec %= addBallSpeed;
		}



		// 鼠标消息处理
		if (MouseHit())
		{
			m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_MOUSEMOVE:
				//TODO炮台方向
				//line(m.x, m.y, nextBall.x, nextBall.y);
				break;
			case WM_LBUTTONDOWN:
				ball tempB;
				tempB.c = nextBall.c;
				tempB.ballDirction = float(m.x - nextBall.x) / float(nextBall.y - m.y);
				tempB.x = nextBall.x;
				tempB.y = nextBall.y;
				ListInsert(headCol, 0, tempB);
				genNextBallSmooth(&nextBall, WINDOWWITH / 2,WINDOWHEIGHT,1);
				length++;
				break;

			case WM_RBUTTONDOWN:
				// 按鼠标右键退出程序
				DestroyList(head);
				closegraph();
				return 0;
			}
		}

		//刷新屏幕
		BeginBatchDraw();
		cleardevice();


		drawFps(fps);

		updateBallPos(head);
		updateBallCurrnetPos(head, pulse);
		drawBallList(head);

		drawSingleBall(nextBall);
		updateBallColPos(headCol, speed, pulse);
		drawBallList(headCol);
		collisionDetection(head, headCol);

		difficultyControl(time,timeBegin);
		

		while (tellGameOver(length)) {
			GameOver((time-timeBegin)/1e6);
			FlushBatchDraw();
		}

		FlushBatchDraw();
	}
	EndBatchDraw();
}

