#include "ballControler.h"
#include "ball.h"
#include "LinkList.h"
#include "math.h"

COLORREF ballColorTable[] = {
MAGENTA, BROWN,BLUE, GREEN, RED, YELLOW
};

int length = 0;


//初始化球链表
void initBallList(Node* head)
{
	int i;
	ball b;
	for (i = 0; i < 10; ++i) {
		//b.c = rand() % 6;
		b.c = i % 6;
		ListInsert(head, 0, b);
		length++;
	}
	updateBallPos(head);

	Node* p = head;
	while (p->next != NULL)
	{
		p = p->next;
		p->data.y = p->data.cy;
		p->data.x = p->data.cx;
	}
}
float func(float x) {
	return  -270 * sin(x / 96);
}
float funcD(float x) {
	return  -270 * cos(x / 96) / 96;
}
//更新球链中球的目标位置
void updateBallPos(Node* head)
{
	Node* p = head;
	float y = 10;
	while (p->next != NULL)
	{
		p = p->next;
		p->data.cy = y;
		p->data.cx = func(y) + WINDOWHEIGHT / 2;
		y = y + BALLRADIUS * 2 / sqrt(1 + funcD(y) * funcD(y));
	}
	
}

void updateBallCurrnetPos(Node* head,int pulse)
{
	Node* p = head;
	while (p->next != NULL)
	{
		p = p->next;
		if ((p->data.cy - p->data.y > 1e-3 ) || (p->data.cy - p->data.y <  -1e-3)) {
		if (p->data.cy > p->data.y){
			p->data.y += (double)pulse / 10000.0;
		}
		else {
			p->data.y -= (double)pulse / 10000.0;
		}
		}
		p->data.x = func(p->data.y) + WINDOWHEIGHT / 2;
	}

}


void eliminateBall(Node* head, int index, int c) {
	Node* p = head;
	Node* q;
	int j = 0;
	while (p->next && j < index)
	{
		p = p->next;
		++j;
	}
	if (j != index) return;
	q = p;
	Node* tmp = q->next;
	while (tmp) {
		q = tmp;
		tmp = q->next;
		if (q->data.c == c) {
			ListDeleteAddr(q);
			length--;
		}
		else {
			break;
		}
	}
	tmp = p;
	while (tmp) {
		q = tmp;
		tmp = q->prev;
		if (q->data.c == c) {
			ListDeleteAddr(q);
			length--;
		}
		else {
			break;
		}
	}

}

//球碰撞检测
void collisionDetection(Node* head, Node* chead)
{
	ball b;

	int dist = 0;
	int i = 0;
	Node* cp = chead->next;
	while (cp != NULL)
	{
		b = cp->data;
		Node* p = head->next;
		int index = 0;
		while (p != NULL)
		{

			dist = (p->data.x - b.x) * (p->data.x - b.x) + (p->data.y - b.y) * (p->data.y - b.y);

			if (dist < BALLRADIUS* BALLRADIUS)
			{
				if (b.c == p->data.c) {
					//同色情况
					eliminateBall(head, index, b.c);
					//updateBallPos(head);
				}
				else {
					ListInsert(head, index, b);
					length++;
					//updateBallPos(head);
				}
				ListDelete(chead, i);
				length--;
				return;
			}

			p = p->next;
			index++;
		}
		cp = cp->next;
		i++;
	}
	return;

}

void updateBallColPos(Node* ahead, int speed, int pulse)
{

	Node* p = ahead;
	float y = 10;
	int i = 0;
	while (p->next != NULL)
	{
		p = p->next;
		p->data.y -= speed * (double)pulse / 10000.0;
		p->data.x += (speed * p->data.ballDirction * (double)pulse / 10000.0);
		//检测碰撞球是否超出范围
		if (p->data.x > WINDOWWITH || p->data.x  <0 || p->data.y > WINDOWHEIGHT || p->data.y < 0)
		{
			ListDelete(ahead, i);
			length--;
			return;
		}
		i++;
	}
}

//绘制球表
void drawBallList(Node* head)
{
	Node* p;
	ball b;
	p = head;
	while (p->next != NULL)
	{
		p = p->next;
		b = p->data;
		drawSingleBall(b);
	}
}

//绘制碰撞球
void drawSingleBall(ball b)
{
	setcolor(ballColorTable[b.c]);
	setfillcolor(ballColorTable[b.c]);
	solidcircle(b.x, b.y, BALLRADIUS);
}