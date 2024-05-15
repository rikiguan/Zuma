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
float funcCos(float x) {
	return   1 / sqrt(1 + funcD(x) * funcD(x));
}
//更新球链中球的目标位置
void updateBallPos(Node* head)
{
	Node* p = head;
	float y = BALLRADIUS;
	while (p->next != NULL)
	{
		p = p->next;
		p->data.cy = y;
		p->data.cx = func(y) + WINDOWHEIGHT / 2;
		y = y + BALLRADIUS * 2 * funcCos(y);
	}
	
}

void updateBallCurrnetPos(Node* head,int pulse)
{
	Node* p = head;
	while (p->next != NULL)
	{
		p = p->next;
		if ((p->data.cy - p->data.y > 1 ) || (p->data.cy - p->data.y <  -1)) {
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



void drawAlpha( int  picture_x, int picture_y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if ((ix + picture_x) >= 0 && (ix + picture_x) <= graphWidth && (iy + picture_y) >= 0 && (iy + picture_y) <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

//绘制碰撞球
void drawSingleBall(ball b)
{
#ifdef PICGAME

	//p1.png
	IMAGE img_mm;
	//loadimage(&img_mm, _T("D:\\workspace\\oj\\zum\\src\\p2.ico"));
	char file_name[200];
	sprintf_s(file_name, "../src/p%d.ico", b.c);
	wchar_t wstr[100];
	mbstowcs_s(0, wstr, file_name, 100);
	loadimage(&img_mm, wstr, BALLRADIUS * 2, BALLRADIUS * 2);
	//putimage(b.x, b.y, &img_mm);
	drawAlpha(b.x - BALLRADIUS, b.y - BALLRADIUS, &img_mm);

#endif // PICGAME

#ifndef PICGAME
	setcolor(ballColorTable[b.c]);
	setfillcolor(ballColorTable[b.c]);
	solidcircle(b.x, b.y, BALLRADIUS);
#endif
}