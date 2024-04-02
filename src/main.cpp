#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "LinkList.h"
#include "ball.h"
#include <iostream>
#include <chrono>
#include "ballControler.h"

float speed = 5;
time_t pulse;
extern int length;
using namespace std;
ball nextBall;

time_t GetCurrentTimeMsec() {
	auto time = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now());
	time_t timestamp = time.time_since_epoch().count();
	return timestamp;
}

void genNextBall(ball * next,int x,int y) {
	next->c = rand() % 6;
	next->x = x;
	next->y = y;
}

void drawFps(int fps) {
	char fpsDisplay[200];
	sprintf_s(fpsDisplay, "fps: %d | length:%d",fps, length);
	settextstyle(16, 0, _T("Consolas"));
	settextcolor(WHITE);
	wchar_t wstr[100];
	mbstowcs_s(0, wstr, fpsDisplay, 100);
	outtextxy(10, 5, wstr);
}

void GameOver(long score) {
	char Display[200];
	sprintf_s(Display, "gameover within %ld s", score);
	settextcolor(WHITE);
	settextstyle(30, 0, _T("Consolas"));
	wchar_t wstr[100];
	mbstowcs_s(0, wstr, Display, 100);
	outtextxy(WINDOWWITH/2*0.3, WINDOWHEIGHT*0.7, wstr);
}
void addGenBall(Node* head) {
	ball b;
	genNextBall(&b, WINDOWHEIGHT / 2, 0);
	ListInsert(head, 0, b);
	length++;
}

bool tellGameOver(int length) {
	if (length >= 44) {
		return true;
	}
	return false;
}


int main()
{
	int fps = 0;

	// 初始化绘图窗口
	initgraph(WINDOWWITH, WINDOWHEIGHT);
	setbkcolor(BLACK);
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

	genNextBall(&nextBall, WINDOWWITH / 2, WINDOWHEIGHT);
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

		if (delayForSec > 1000000) {
			addGenBall(head);
			delayForSec %= 1000000;
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
				genNextBall(&nextBall, WINDOWWITH / 2,WINDOWHEIGHT);
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

		while (tellGameOver(length)) {
			GameOver((time-timeBegin)/1e6);
			FlushBatchDraw();
		}

		FlushBatchDraw();
	}
	EndBatchDraw();
}

