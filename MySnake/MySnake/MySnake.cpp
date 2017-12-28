// MySnake.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>

#define KEYUP 72
#define KEYDOWN 80
#define KEYLEFT 75
#define KEYRIGHT 77
#define MAPX 20
#define MAPY 20
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
int map[MAPX + 2][MAPY + 2] = //{ 0 };
{
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 1,8,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1 },//起点
{ 1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1 },
{ 1,0,0,0,0,0,1,9,0,0,0,0,0,0,0,0,1,0,0,1 },//终点在本行
{ 1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1 },
{ 1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,1,0,1,0,0,0,0,0,5,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
{ 1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };
int AX, AY;
int maze[MAPX][MAPY]; //用于搜索
int autoRound = 0;
int step;
int kbhitCount;
int gameStatus;											   //在游戏开始时确认同时存在的最多食物数量
int score = 0;
int dimageShow = 0; //收到伤害时显示时间
int directionX[50] = { 0 }, directionY[50] = { 0 };
/*
*********
用于BFS
*********
*/

struct point { int row, col, predecessor; } queue[1000];
int front = 0, rear = 0;
//队列的操作
void enqueue(struct point p)//入队
{
	queue[rear++] = p;
}

struct point dequeue(void)//出队 从head开始
{
	return queue[front++];//此时队首也向后移了 eg：0 -> 1 先返回front front再自加
}

int is_empty(void)
{
	return front == rear;
}
//????????
void clean_queue()
{
	for (int i = 0; i < 500; i++)
	{
		queue[i].col = NULL;
		queue[i].row = NULL;
		queue[i].predecessor = NULL;
	}
	front = 0;
	rear = 0;
}

struct snake
{	
	int position = 0;
	int positionX = 0;
	int positionY = 0;
	struct snake* next = NULL;
	struct snake* previous = NULL;
	
};

struct food
{
	int foodX;
	int foodY;
	int foodLife;//持续时间
	int status = 0;//是否已经有食物
	//int kind; 种类 后期扩展
};
struct poison
{
	int poisonX;
	int poisonY;
	int posionLife;
	int status;
	//int kind 
};
int foodCount = 0;//统计已有的食物数
int poisonCount = 0;
int foodNum;
int poisonNum;
food * foodGroup;
poison * poisonGroup;
char* key,* autoKey;//按键
int isFinish = 1;
snake* head, *tail;
int walkMode = 1;//行走模式 0-手动,1-自动
void iniMap();
void iniSnake();
void printMap();
void addSnake(int x, int y);
void delTail();
void move(int dirX, int dirY);
void gotoxy(int x, int y);
void playGame();
void HideCursor();
void generateFood();
void generatePoison();
int rand_food_x(void);
int rand_food_y(void);
int judge();
void Clearer();
void printKey();
void loadKey();//加载计算出来的路径
void clearKey();//清除已有的路径
void BFS();
void theWall(int dirX,int dirY);//防止撞墙--在每次移动后进行
int main()
{		
	foodNum = (rand_food_x() *133) % 10 + 10;
	poisonNum = (rand_food_x() * 12) % 10 + 12;
	/*注意 全角字符使用goto语句时要*/
		HideCursor();
		iniMap();
		iniSnake();
		printMap();//关于gotoxy的显示问题 为什么不能goto到 地图右边？？


		playGame();

		
  return 0;
}

//地图初始化
void iniMap()
{	//边框
	for (int i = 0; i < MAPX; i++)
	{
		map[i][0] = 1;
		map[i][19] = 1;
	}

	for (int j = 0; j < MAPY; j++)
	{
		map[0][j] = 1;
		map[19][j] = 1;
	}
}

void printMap()
{	

	//似乎存在BUG
	gotoxy(5, 45);
	printf("当前得分： %d", score);
	gotoxy(7, 45);
	printf("最大食物数： %d", foodNum);
	int count = 0;
	//遍历链表打印蛇身体
	snake * cur = head;
	if (head == NULL)
	{
		printf("头指针为空");
	}
	else
	{
		
		while (cur != NULL)
		{
			count += 1;
			map[cur->positionX][cur->positionY] = 2;
			cur = cur->next;
		}
		if (count == 1)
		{
			gameStatus = 0;
		}

		map[head->positionX][head->positionY] = 6;//单独给蛇头一个值

		for (int i = 0; i < MAPX; i++)
		{
			for (int j = 0; j < MAPY; j++)
			{
				switch (map[i][j]) {
				case 0:
					gotoxy(i, 2 * j);
					printf("  ");
					break;
				case 1:
					gotoxy(i, 2 * j);
					printf("■");
					break;
				case 2:
					gotoxy(i, 2 * j);
					printf("□");
					break;
				case 4:
					gotoxy(i, 2 * j);
					printf("  ");
					break;
				case 5:
					gotoxy(i, 2 * j);
					printf("$$");
					break;
				case 6:
					gotoxy(i, 2 * j);
					printf("■");
					break;
				case 7:
					gotoxy(i, 2 * j);
					printf("XX");
					break;
				case 12:
					gotoxy(i, 2 * j);
					printf("??");
					break;
				}
			}
		}

		gotoxy(9, 45);
		printf("场上食物数： %d", foodCount);
		gotoxy(2, 45);
		printf("长度： %d", count);
	}
}

void iniSnake()
{
	/*对蛇进行初始化
	*初始长度为X节  蛇头坐标在中心
	*/
	//建立双向链表

	snake * current,*prev;
	for (int i = 0; i < 3; i++)
	{	
		current = (struct snake*) calloc(sizeof(struct snake),1);
		if (head == NULL)
		{
			current -> positionX = (MAPX / 2);
			current-> positionY = (MAPY / 2);
			prev = current;
			head = current;
		}
		else
		{	
			prev->next = current;
			current->previous = prev;
			current->positionX = (prev->positionX + 1);
			current->positionY = prev->positionY;
			prev = current;
		
		}
		
	}
	tail = current;//尾指针 始终指向最后出现的一个
	tail->next = NULL;
}


//增加头节点
void addSnake(int x, int y)
{
	snake* add = (snake*)calloc(sizeof(snake),1);
	add->positionX = head->positionX + x;
	add->positionY = head->positionY + y;
	add->next = head;
	head->previous = add;
	head = add;
}

//删除尾节点
void delTail()
{
	map[tail->positionX][tail->positionY] = 0;
	tail = tail->previous;
	free(tail->next);
	tail->next = NULL;
	
}

//蛇的移动,撞墙判断？
void move(int dirX,int dirY)
{
	int result = 0;
	theWall(dirX, dirY);
	addSnake(AX, AY);
	if (walkMode == 1)
	
	
	result = judge();
	if (result != 1)//不吃到食物时，每次移动都删掉尾节点
	{
		delTail();
		if(walkMode == 0)
		if (result == 2)
			delTail();//吃到毒 再减少一次
		result = 0;
	}

	
	Clearer();//清除过期的食物/清除过期的毒草
	if (dimageShow == 0) {
		gotoxy(21, 8);
		printf("                                          ");
	}
	else
	{
		dimageShow--;
	}
}


//游戏主循环
void playGame()
{

	 kbhitCount = 0;//统计按键次数
	foodNum = rand_food_y() % 10 + 6;//最大食物数量
	poisonNum = (rand_food_y() * 213) % 10 + 4;//最大毒数量
	foodGroup = (food*)calloc(sizeof(food), foodNum);
	poisonGroup = (poison*)calloc(sizeof(poison), poisonNum);

	gameStatus = 1;
	//这两个数组用来保存一系列指令的方向
	int directionX[20] = { 0 }, directionY[20] = { 0 };
	//初始方向向上
	directionX[0] = -1;
	directionY[0] = 0;

	key = (char*)calloc(sizeof(char), 50);//按键数组最多接受20个
	autoKey = (char*)calloc(sizeof(char), 50);

	for (int i = 0; i < 50; i++) {
		key[i] = 0;
		autoKey[i] = 0;
	}
	//test
	/*
	autoKey[0] =  KEYRIGHT ;
	autoKey[1] =  KEYUP;
	autoKey[2] = KEYLEFT;
	autoKey[3] = KEYUP;
	*/
	
	int step = 0;//走的步数
	char lastKey;
	while (gameStatus)
	{
		printKey();
		generatePoison();
		generateFood();//生成食物及去除过期食物  !!吃到智慧草 食物寿命无限


		if (!kbhit())
		{

			if (walkMode == 1)
			{
				gotoxy(1, 42);
				printf("AUTO MODE");
				if (autoRound != 0)
				{
					isFinish = 0;//不执行BFS
					autoRound--;
				}
				else
					isFinish = 1;

				if (isFinish) {
					BFS();				//BFS(); 计算   现在只能找一次。。。然后把自己撞死了
					loadKey();//加载-key 将autoKEY 转为 key
					kbhitCount = 0;
					step = 0;
				}
				

	  //下面的代码将key转化为具体的行走
				int compare;//比较的对象 的下标
					if (kbhitCount == 0){//此时比较对象为自己原有的值，否则是和上一项比较
						compare = 0;
					}
					else
					{
						compare = kbhitCount - 1;
					}


					if (directionX[compare] == 1)//down   原来的方向向下时！就不能向上
					{

						switch (key[kbhitCount])//获取当前输入
						{
						case KEYLEFT:
							directionY[kbhitCount] = -1, directionX[kbhitCount] = 0;
							break;
						case KEYRIGHT:
							directionY[kbhitCount] = 1, directionX[kbhitCount] = 0;
							break;
						case KEYDOWN:
							directionX[kbhitCount] = 1, directionY[kbhitCount] = 0;
							break;
						case KEYUP:
							directionX[kbhitCount] = 1, directionY[kbhitCount] = 0;
							break;
							//如果不写这个蛇会消失-没有移动但删除了尾巴
						}
					}


					if (directionX[compare] == -1) //up
					{
						switch (key[kbhitCount])
						{
						case KEYLEFT:
							directionY[kbhitCount] = -1, directionX[kbhitCount] = 0;
							break;
						case KEYRIGHT:
							directionY[kbhitCount] = 1, directionX[kbhitCount] = 0;
							break;
						case KEYDOWN:
							directionX[kbhitCount] = -1, directionY[kbhitCount] = 0;
							break;
						case KEYUP:
							directionX[kbhitCount] = -1, directionY[kbhitCount] = 0;
							break;
						}
					}
					if (directionY[compare] == 1)//向右移动
					{
						switch (key[kbhitCount])
						{

						case KEYUP:
							directionX[kbhitCount] = -1, directionY[kbhitCount] = 0;
							break;
						case KEYDOWN:
							directionX[kbhitCount] = 1, directionY[kbhitCount] = 0;
							break;
						case KEYRIGHT:
							directionX[kbhitCount] = 0, directionY[kbhitCount] = 1;
							break;
						case KEYLEFT:
							directionX[kbhitCount] = 0, directionY[kbhitCount] = 1;
							break;
						}
					}

					if (directionY[compare] == -1)
					{
						switch (key[kbhitCount])
						{
						case KEYUP:
							directionX[kbhitCount] = -1, directionY[kbhitCount] = 0;
							break;
						case KEYDOWN:
							directionX[kbhitCount] = 1, directionY[kbhitCount] = 0;
							break;
						case KEYLEFT:
							directionX[kbhitCount] = 0, directionY[kbhitCount] = -1;
							break;
						case KEYRIGHT:
							directionX[kbhitCount] = 0, directionY[kbhitCount] = -1;
							break;
						}
						
			}

			kbhitCount++;

			//	getch();
			//step++;
		}
		//当 key 的数组中还存在 按键时 读取按键
		move(directionX[step], directionY[step]);
		//走过后清除这一步的数据(有多条指令的时候)，并准备读取下一指令
		if (step != 0) {
			lastKey = key[step];//?
			key[step] = 0;
		}

		if (key[step + 1] == 0)//读取下一步发现没有按键时   //撞墙判断---------------
		{	
			clearKey();
			BFS();
			loadKey();
			//保留当前方向
			//directionX[0] = directionX[step];
			//directionY[0] = directionY[step];
			step = 0;//又返回第一步，第一步即等会输入的值
			kbhitCount = 0;//指令数变为1条
		}
		else {
			step += 1;
			//kbhitCount++;
		}//下一轮还是读取储存好的指令

		printMap();
		Sleep(100);
	}
		else//当检测到键盘输入时   如果本次输入与上次输入相同，不接受
		{       if(walkMode == 1)
			    clearKey();
				walkMode = 0;
				getch();//取得 方向键值 第一部分
				key[kbhitCount] = getch();
				//下面用于保存输入的值
				int compare;//比较的对象 的下标
				if (kbhitCount == 0) {//此时比较对象为自己原有的值，否则是和上一项比较
					compare = 0;
				}
				else
				{
					compare = kbhitCount - 1;
				}
				
				//if (key[kbhitCount] == 'c')
					//walkMode = 1;
				if (directionX[compare] == 1)//down   原来的方向向下时！就不能向上
				{

					switch (key[kbhitCount])//获取当前输入
					{
					case KEYLEFT:
						directionY[kbhitCount] = -1, directionX[kbhitCount] = 0;
						break;
					case KEYRIGHT:
						directionY[kbhitCount] = 1, directionX[kbhitCount] = 0;
						break;
					case KEYDOWN:
						directionX[kbhitCount] = 1, directionY[kbhitCount] = 0;
						break;
					case KEYUP:
						directionX[kbhitCount] = 1, directionY[kbhitCount] = 0;
						break;
						//如果不写这个蛇会消失-没有移动但删除了尾巴
					}
				}

				if (directionX[compare] == -1) //up
				{
					switch (key[kbhitCount])
					{
					case KEYLEFT:
						directionY[kbhitCount] = -1, directionX[kbhitCount] = 0;
						break;
					case KEYRIGHT:
						directionY[kbhitCount] = 1, directionX[kbhitCount] = 0;
						break;
					case KEYDOWN:
						directionX[kbhitCount] = -1, directionY[kbhitCount] = 0;
						break;
					case KEYUP:
						directionX[kbhitCount] = -1, directionY[kbhitCount] = 0;
						break;
					}
				}
				if (directionY[compare] == 1)//向右移动
				{
					switch (key[kbhitCount])
					{

					case KEYUP:
						directionX[kbhitCount] = -1, directionY[kbhitCount] = 0;
						break;
					case KEYDOWN:
						directionX[kbhitCount] = 1, directionY[kbhitCount] = 0;
						break;
					case KEYRIGHT:
						directionX[kbhitCount] = 0, directionY[kbhitCount] = 1;
						break;
					case KEYLEFT:
						directionX[kbhitCount] = 0, directionY[kbhitCount] = 1;
						break;
					}
				}

				if (directionY[compare] == -1)
				{
					switch (key[kbhitCount])
					{
					case KEYUP:
						directionX[kbhitCount] = -1, directionY[kbhitCount] = 0;
						break;
					case KEYDOWN:
						directionX[kbhitCount] = 1, directionY[kbhitCount] = 0;
						break;
					case KEYLEFT:
						directionX[kbhitCount] = 0, directionY[kbhitCount] = -1;
						break;
					case KEYRIGHT:
						directionX[kbhitCount] = 0, directionY[kbhitCount] = -1;
						break;
					}
					gotoxy(26, 0);


				}

				kbhitCount++;
				//gotoxy(21, 0);
				//printf("%按键%d\n", kbhitCount);
			
			/*else if(walkMode == 2)
			{
				kbhitCount--;
				if (kbhitCount == 0)//当走完计算出的路径时返回手动行走
					walkMode = 1;
			}*/
			
		}
		
	}
	system("cls");
	gotoxy(10, 10);
	printf("GAME OVER");
}


void gotoxy(int y, int x) {//x行坐标,y列坐标
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
	SetConsoleCursorPosition(hOut, pos);//两个参数分别是指定哪个窗体，具体位置
}
//隐藏光标
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}


void generateFood()
{	
	srand(time(NULL));
//	gotoxy(24, 0);
//	printf("FOOD NUM:%d", foodNum);
	int x, y;
	x = (rand() * 465) % MAPX + 1;
	y = (rand() * 124) % MAPY + 1;
	int isGenerate = 0;
	//调节生成食物的速率
	isGenerate = (rand() * 100) % 10;
	if (isGenerate < 4)
	{
		if (x > 0 && x < MAPX && y>0 && y < MAPY && map[x][y] != 1 && foodCount < (foodNum) && map[x][y] != 2 && map[x][y] != 5 && map[x][y] != 6 && map[x][y] != 7)
		{	
			for (int i = 0; i < foodNum; i++) {//找一个位置储存食物
				if (foodGroup[i].status == 0)//该位置本来不存在食物的时候
				{
					foodGroup[i].foodX = x;
					foodGroup[i].foodY = y;
					foodGroup[i].foodLife = (rand_food_x() * 100) % 50 + 50;//食物的寿命
					foodGroup[i].status = 1;
					map[x][y] = 5;
					i = foodNum;
					foodCount += 1;
				}
			}
			

			
			}	    
		}	
	}


void generatePoison()
{
	srand(time(NULL) + 100);
	int x, y;
	x = (rand() * 287) % MAPX + 1;
	y = (rand() * 99) % MAPY + 1;
	int isGenerate = 0;
	//调节生成的速率
	isGenerate = (rand() * 100) % 18;
	if (isGenerate < 12)
	{
		if (x > 0 && x < MAPX && y>0 && y < MAPY && map[x][y] != 1 && poisonCount < (poisonNum - 1) && map[x][y] != 2)
		{
			for (int i = 0; i < poisonNum; i++) {//找一个位置储存食物
				if (poisonGroup[i].status == 0)//该位置本来不存在食物的时候
				{
					poisonGroup[i].poisonX = x;
					poisonGroup[i].poisonY = y;
					poisonGroup[i].posionLife = (rand_food_x() * 100) % 22 + 20;//寿命
					poisonGroup[i].status = 1;
					map[x][y] = 7;
					i = poisonNum;
				}
			}
			poisonCount += 1;

			gotoxy(7, 45);
			printf("场上毒数： %d", poisonCount);
		}
	}
}

//生成食物的随机数//
int rand_food_x(void)
{
	int x;
	srand(time(NULL));
	x = (rand() * rand()) % MAPX;

	return x;
}

int rand_food_y(void)
{
	int y;
	srand(time(NULL));
	y = rand() % MAPY;
	return y;
}


int judge()
{	//？？
	/*if (head->positionX == tail->positionX && head->positionY == tail->positionY) {
		gameStatus = 0;
		gotoxy(21, 0);
		printf("你只剩一节了！！！！！！");
	}*/

	int result = 0;
	int x = head->positionX;
	int y = head->positionY;
	//对蛇头进行判断,吃到食物后不删除尾巴
	if (map[x][y] == 5)//头的坐标和食物的坐标重合
	{
		isFinish = 1;
		for (int i = 0; i < foodNum; i++)
			if (foodGroup[i].foodX == x && foodGroup[i].foodY == y)
			{
				gotoxy(21, 0);
				printf("Good You eat a food !");
				foodGroup[i].status = 0;
				foodCount -= 1;//有问题，不能这样。。这样写吃到中间一段 导致最后一个遍历不到了
				score += 100;
				result = 1;
				return result;

			}
	}

	if (map[x][y] == 7)//头的坐标和毒物的坐标重合
	{
		for (int i = 0; i < poisonNum; i++)
			if (poisonGroup[i].poisonX == x && poisonGroup[i].poisonY == y)
			{	//找到吃到的坐标
				gotoxy(21, 0);
				printf("No no no!!      ");
				poisonGroup[i].status = 0;
				poisonCount -= 1;
				score -= 100;
				result = 2;
				return result;

			}
	}
	//撞墙了！
	else if (map[x][y] == 1) {
		if(walkMode == 0)
		gameStatus = 0;
		else
		{
			delTail();
			gotoxy(25, 0);
			printf("撞墙了，痛！！！");
		}
	}

	//咬到自己
	else if (map[x][y] == 2)//蛇头坐标和身体坐标重合
	{
		//先找到咬到的地方
		snake * find;
		find = head->next;
		int loseNum = 0;//被咬掉的节数
		while (find != NULL)//遍历链表
		{
			if (find->positionX == x && find->positionY == y)
			{
				gotoxy(21, 8);
				printf("！！痛！！你失去了%d节", loseNum+1);
				dimageShow = 5;
				//删除之后的节点
				snake * del;
				int count = 0;
				//从尾节点开始删除
				do
				{	
					count++;
					map[tail->positionX][tail->positionY] = 0;
					tail = tail->previous;
					free(tail->next);
					tail->next = NULL;
				} while (tail->positionX == find->positionX && tail->positionY == find->positionY);
				//再清除掉一段
				//gotoxy(22, 0);
				//printf("痛！！！！！你失去了%d",count+1);
				break;
			}
			find = find->next;
		}
	}
			
	
	return result;
}

void Clearer()
{
	//所有存在的食物的生命 -1
	for (int i = 0; i < foodNum; i++)
	{
		if (foodGroup[i].status == 1)
		{
			foodGroup[i].foodLife--;

			if (foodGroup[i].foodLife <= 0)
			{
				foodGroup[i].status = 0;
				map[foodGroup[i].foodX][foodGroup[i].foodY] = 0;
				foodCount--;
			}
		}
	}

	//所有存在的毒物的生命 -1
	for (int i = 0; i < poisonNum; i++)
	{
		if (poisonGroup[i].status == 1)
		{
			poisonGroup[i].posionLife--;

			if (poisonGroup[i].posionLife <= 0)
			{
				poisonGroup[i].status = 0;
				map[poisonGroup[i].poisonX][poisonGroup[i].poisonY] = 0;
				poisonCount--;
			}
		}
	}
}

void printKey()
{
	gotoxy(25, 0);
		printf(" ");//清除走过的
		gotoxy(25, 0);
	for (int i = 0; i < 10; i++) {//打印指令
		char k = key[i];
		switch (k)
		{
		case KEYUP:
			printf(" ↑ ");
			break;

		case KEYDOWN:
			printf(" ↓ ");
			break;

		case KEYLEFT:
			printf(" ← ");
			break;
		case KEYRIGHT:
			printf(" → ");
			break;
		case 0:
			printf(" ");
			break;

		default:
			break;

		}
		//printf("\n");
	}
}
/*
----------------BFS---------------
*/

void visit(int row, int col)//下一步尝试的位置
{
	struct point visit_point = { row, col, front - 1 };
	if (maze[row][col] == 5)
		maze[row][col] = 5;
	else if (maze[row][col] == 1)
		maze[row][col] = 1;
	else if(maze[row][col] == 7)
		maze[row][col] = 7;
	else
	{
		maze[row][col] = 10;
	}
	//maze[row][col] = 11;//搜索标记 防止重复搜索
	enqueue(visit_point);
}

void BFS()
{
	int isSearch = 0;
	//加载地图 用于搜索路径
	for (int i = 0; i < MAPX; i++)
	{
		

		for (int j = 0; j < MAPY; j++)
		{

			maze[i][j] = map[i][j];//将地图拷贝
		}
	}


	//确认目标坐标   loop
	for (int i = 0; i < foodNum; i++)
	{
		if (foodGroup[i].status == 1) {
			int tarX = foodGroup[i].foodX;
			int tarY = foodGroup[i].foodY;
			maze[tarX][tarY] = 5;//打上记号--终点
		}
	}
	int x, y;
	clean_queue();
	struct point p;//起始点  蛇头---------------------------------怎么在第二期
	gotoxy(10, 42);
	printf("SNAKE HEAD POI:%d,%d", head->positionX, head->positionY);
	p.col = head->positionY;
	p.row = head->positionX;
	x = head->positionX;
	y = head->positionY;
	p.predecessor = -1;
	enqueue(p);//入队,rear --1
	
	
	while (!is_empty()) {//队列不为空的时候，循环
		p = dequeue();//出队，p等于队首后一??   front----1
		if (maze[p.row][p.col] == 5) {			
			isSearch = 1;
			break;//到达目标点的时候   maze[p.row][p.col] == 5
		}

		if (p.col + 1 < MAPY - 1 /* right 向右不撞边界*/
			&& (maze[p.row][p.col + 1] == 0 || maze[p.row][p.col + 1] == 5) && maze[p.row][p.col + 1] != 1 && maze[p.row][p.col + 1] != 7 && maze[p.row][p.col + 1] != 6)//地图首先要可以走
			visit(p.row, p.col + 1);//visit这一区域

		if (p.row + 1 < MAPX - 1 /* down */
			&& (maze[p.row + 1][p.col] == 0 || maze[p.row][p.col + 1] == 5) && maze[p.row + 1][p.col] != 1 && maze[p.row + 1][p.col] != 7 && maze[p.row + 1][p.col] != 6)
			visit(p.row + 1, p.col);

		if (p.col - 1 >= 0 /* left */
			&& (maze[p.row][p.col - 1] == 0 || maze[p.row][p.col + 1] == 5) && maze[p.row][p.col - 1] != 1 && maze[p.row][p.col - 1] != 7 && maze[p.row][p.col - 1] != 6)
			visit(p.row, p.col - 1);
		if (p.row - 1 >= 0 /* up */
			&& (maze[p.row - 1][p.col] == 0 || maze[p.row][p.col + 1] == 5) && maze[p.row - 1][p.col] != 1 && maze[p.row - 1][p.col] != 7 && maze[p.row - 1][p.col] != 1 && maze[p.row - 1][p.col] != 6)
			visit(p.row - 1, p.col);
	}
	

		if (isSearch == 1)//当搜索到食物的时候
		{
			isSearch = 0;

			int keyCount = 49;
			while (p.predecessor != -1) {//从最后一步一直打到第一步
			//	map[p.row][p.col] = 12;
				if (p.row != queue[p.predecessor].row)//如果行数发生了改变
				{
					if (p.row == (queue[p.predecessor].row - 1))//本行在上一行的上面
					{
						autoKey[keyCount] = KEYUP;
					}
					else {
						autoKey[keyCount] = KEYDOWN;
					}
				}

				if (p.col != queue[p.predecessor].col)//如果行数发生了改变
				{
					if (p.col == (queue[p.predecessor].col - 1))//本列在上一列的左边
					{
						autoKey[keyCount] = KEYLEFT;//下移
					}
					else {
						autoKey[keyCount] = KEYRIGHT;
					}
				}
				p = queue[p.predecessor];
				keyCount--;
			}
			
			//result = 0;
		}
}

void loadKey()
{   
	int i = 0;
	for (int j = 0;j < 50;j++)
	{	
		if (autoKey[j] != 0) {
			
			//printf("load key");
			key[i] = autoKey[j];
			autoKey[j] = 0;
			i++;
		}
	}
	autoRound = i;
	step = 0;
	isFinish = 0;
}

void clearKey() {  //清空所有按键
	int i = 0;
	while (key[i] != 0)
	{
		key[i] = 0;
		directionX[i] = 0;
		directionY[i] = 0;
		i++;
	}
	step = 0;
	kbhitCount = 0;

}

void theWall(int dirX,int dirY)
{
	
	AX = dirX;
	AY = dirY;
	if (map[head->positionX + AX][head->positionY + AY] == 1|| map[head->positionX + AX][head->positionY + AY] == 6) {
		clearKey();
		if (AX == 1 || AX == -1)
		{
			AX = 0;
			AY = 1;
			if (map[head->positionX + AX][head->positionY + AY] == 1 || map[head->positionX + AX][head->positionY + AY] == 2)
			{
				AX = 0;
				AY = -1;
			}
		}
		if (AY == 1 || AY == -1) {
			AY = 0;
			AX = 1;
			if (map[head->positionX + AX][head->positionY + AY] == 1 || map[head->positionX + AX][head->positionY + AY] == 2)
			{
				AY = 0;
				AX = -1;
			}
		}
		clearKey();
		BFS();
		loadKey();
	}
	

	/*if(map[x + 1][y] == 1||map[x - 1][y] == 1||map[x][y + 1]== 1||map[x][y - 1] == 1)
	{
		clearKey();
		BFS();
		loadKey();
	}*//*

	for (int i = 0; i < 5; i++)
	{
		int addx = 0;
		int addy = 0;
		switch (key[i])
		{
		case KEYUP:
			addx = -1;
			addy = 0;
			break;
		case KEYDOWN:
			addx = 1;
			addy = 0;
			break;
		case KEYLEFT:
			addx = 0;
			addy = -1;
			break;
		case KEYRIGHT:
			addx = 0;
			addy = 1;
		case 0:
			i = 5;
			break;
		}
		
		if (map[x][y] == 1 )
		{

			clearKey();
			BFS();
			loadKey();
			break;

		}

		x += addx;
		y += addy;
		
	}*/
}