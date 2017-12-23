// MySnake.cpp : �������̨Ӧ�ó������ڵ㡣
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
int map[MAPX][MAPY] = { 0 };
/*{
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 1,8,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },//���
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1 },
{ 1,0,0,0,1,0,0,9,0,0,0,0,0,0,0,0,1,0,0,1 },//�յ��ڱ���
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };
*/
int maze[MAPX][MAPY]; //��������
int step;
int gameStatus;											   //����Ϸ��ʼʱȷ��ͬʱ���ڵ����ʳ������
int score = 0;
int dimageShow = 0; //�յ��˺�ʱ��ʾʱ��
int directionX[20] = { 0 }, directionY[20] = { 0 };
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
	int foodLife;//����ʱ��
	int status = 0;//�Ƿ��Ѿ���ʳ��
	//int kind; ���� ������չ
};
struct poison
{
	int poisonX;
	int poisonY;
	int posionLife;
	int status;
	//int kind 
};
int foodCount = 0;//ͳ�����е�ʳ����
int poisonCount = 0;
int foodNum;
int poisonNum;
food * foodGroup;
poison * poisonGroup;
char* key,* autoKey;//����
snake* head, *tail;

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
void loadKey();//���ؼ��������·��
void clearKey();//������е�·��

int main()
{		
	foodNum = (rand_food_x() * 15) % 10 + 5;
	poisonNum = (rand_food_x() * 12) % 10 + 5;
	/*ע�� ȫ���ַ�ʹ��goto���ʱҪ*/
		HideCursor();
		iniMap();
		iniSnake();
		printMap();//����gotoxy����ʾ���� Ϊʲô����goto�� ��ͼ�ұߣ���


		playGame();

		
  return 0;
}

//��ͼ��ʼ��
void iniMap()
{	//�߿�
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

	//�ƺ�����BUG
	gotoxy(5, 45);
	printf("��ǰ�÷֣� %d", score);
	gotoxy(7, 45);
	printf("���ʳ������ %d", foodNum);
	int count = 0;
	//���������ӡ������
	snake * cur = head;
	if (head == NULL)
	{
		printf("ͷָ��Ϊ��");
	}
	else
	{
		
		while (cur != NULL)
		{
			count += 1;
			map[cur->positionX][cur->positionY] = 2;
			cur = cur->next;
		}
		map[head->positionX][head->positionY] = 6;//��������ͷһ��ֵ

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
					printf("��");
					break;
				case 2:
					gotoxy(i, 2 * j);
					printf("��");
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
					printf("��");
					break;
				case 7:
					gotoxy(i, 2 * j);
					printf("��");
					break;
				}
			}
		}

		gotoxy(9, 45);
		printf("����ʳ������ %d", foodCount);
		gotoxy(2, 45);
		printf("���ȣ� %d", count);
	}
}

void iniSnake()
{
	/*���߽��г�ʼ��
	*��ʼ����ΪX��  ��ͷ����������
	*/
	//����˫������

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
	tail = current;//βָ�� ʼ��ָ�������ֵ�һ��
	tail->next = NULL;
}


//����ͷ�ڵ�
void addSnake(int x, int y)
{
	snake* add = (snake*)calloc(sizeof(snake),1);
	add->positionX = head->positionX + x;
	add->positionY = head->positionY + y;
	add->next = head;
	head->previous = add;
	head = add;
}

//ɾ��β�ڵ�
void delTail()
{
	map[tail->positionX][tail->positionY] = 0;
	tail = tail->previous;
	free(tail->next);
	tail->next = NULL;
	
}

//�ߵ��ƶ�
void move(int dirX,int dirY)
{
	int result = 0;
	addSnake(dirX, dirY);
	result = judge();
	if (result != 1)//���Ե�ʳ��ʱ��ÿ���ƶ���ɾ��β�ڵ�
	{
		delTail();
		if (result == 2)
			delTail();//�Ե��� �ټ���һ��
		result = 0;
	}

	
	Clearer();//������ڵ�ʳ��/������ڵĶ���
	if (dimageShow == 0) {
		gotoxy(21, 8);
		printf("                                          ");
	}
	else
	{
		dimageShow--;
	}
}


//��Ϸ��ѭ��
void playGame()
{	

	int kbhitCount = 0;//ͳ�ư�������
	 foodNum = rand_food_y() % 10 + 4;//���ʳ������
	 poisonNum = (rand_food_y() * 213) % 10 + 4;//�������
	foodGroup = (food*)calloc(sizeof(food), foodNum);
	poisonGroup = (poison*)calloc(sizeof(poison), poisonNum);

	gameStatus = 1;
	//������������������һϵ��ָ��ķ���
	int directionX[20] = { 0 }, directionY[20] = { 0};
	 //��ʼ��������
	 directionX[0] = -1;
	 directionY[0] = 0;

 key = (char*)calloc(sizeof(char),20);//��������������20��
 autoKey = (char*)calloc(sizeof(char), 20);

 for (int i = 0; i < 10; i++) {
	 key[i] = 0;
	 autoKey[i] = 0;
 }
 //test
 autoKey[0] =  KEYRIGHT ;
 autoKey[1] =  KEYUP;
 autoKey[2] = KEYLEFT;
 autoKey[3] = KEYUP;

 int walkMode = 0;//����ģʽ 0-�ֶ�,1-�Զ�
	int step = 0;//�ߵĲ���
	char lastKey;
	while (gameStatus)
	{
		printKey();
		generatePoison();
		generateFood();//����ʳ�Ｐȥ������ʳ��  !!�Ե��ǻ۲� ʳ����������
		int hit;
	/*	if (walkMode == 0)
			hit = !kbhit();
		else
			hit = kbhit();*/
		if (!kbhit())
		{	

			//�� key �������л����� ����ʱ ��ȡ����
			move(directionX[step], directionY[step]);
			//�߹��������һ��������(�ж���ָ���ʱ��)����׼����ȡ��һָ��
			if (step != 0) {
				lastKey = key[step];//?
				key[step] = 0;
			}
	
			if (key[step+1] == 0)//��ȡ��һ������û�а���ʱ
			{   
				//������ǰ����
				directionX[0] = directionX[step];
				directionY[0] = directionY[step];
				step = 0;//�ַ��ص�һ������һ�����Ȼ������ֵ
				kbhitCount = 0;//ָ������Ϊ1��
			}
			else { step += 1;
			//kbhitCount++;
			}//��һ�ֻ��Ƕ�ȡ����õ�ָ��
			
			printMap();
			Sleep(400);


		}
		else//����⵽��������ʱ   ��������������ϴ�������ͬ��������
		{
			if (walkMode == 0) {
				getch();//ȡ�� �����ֵ ��һ����
				key[kbhitCount] = getch();
				//�������ڱ��������ֵ
				int compare;//�ȽϵĶ��� ���±�
				if (kbhitCount == 0) {//��ʱ�Ƚ϶���Ϊ�Լ�ԭ�е�ֵ�������Ǻ���һ��Ƚ�
					compare = 0;
				}
				else
				{
					compare = kbhitCount - 1;
				}

				if (directionX[compare] == 1)//down   ԭ���ķ�������ʱ���Ͳ�������
				{

					switch (key[kbhitCount])//��ȡ��ǰ����
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
						//�����д����߻���ʧ-û���ƶ���ɾ����β��
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
				if (directionY[compare] == 1)//�����ƶ�
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
				//printf("%����%d\n", kbhitCount);
			}
			else if (walkMode == 1){//����ģʽΪ�Զ���ʱ��
				getch();//�������
				getch();
								//BFS(); ����
				loadKey();//����-key
				kbhitCount = 0;
				step = 0;
				walkMode = 0;//�Զ����ߵ���ʱ

				//����Ĵ��뽫keyת��Ϊ���������
				int compare;//�ȽϵĶ��� ���±�
				while (key[kbhitCount] != 0) {
					if (kbhitCount == 0) {//��ʱ�Ƚ϶���Ϊ�Լ�ԭ�е�ֵ�������Ǻ���һ��Ƚ�
						compare = 0;
					}
					else
					{
						compare = kbhitCount - 1;
					}

					if (directionX[compare] == 1)//down   ԭ���ķ�������ʱ���Ͳ�������
					{

						switch (key[kbhitCount])//��ȡ��ǰ����
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
							//�����д����߻���ʧ-û���ƶ���ɾ����β��
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
					if (directionY[compare] == 1)//�����ƶ�
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
				}
			//	getch();
			}
			/*else if(walkMode == 2)
			{
				kbhitCount--;
				if (kbhitCount == 0)//������������·��ʱ�����ֶ�����
					walkMode = 1;
			}*/
			
		}
		
	}
	system("cls");
	gotoxy(10, 10);
	printf("GAME OVER");
}


void gotoxy(int y, int x) {//x������,y������
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// ��ȡ��׼����豸���
	SetConsoleCursorPosition(hOut, pos);//���������ֱ���ָ���ĸ����壬����λ��
}
//���ع��
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
	//��������ʳ�������
	isGenerate = (rand() * 100) % 10;
	if (isGenerate < 4)
	{
		if (x > 0 && x < MAPX && y>0 && y < MAPY && map[x][y] != 1 && foodCount < (foodNum) && map[x][y] != 2 && map[x][y] != 5 && map[x][y] != 6 && map[x][y] != 7)
		{	
			for (int i = 0; i < foodNum; i++) {//��һ��λ�ô���ʳ��
				if (foodGroup[i].status == 0)//��λ�ñ���������ʳ���ʱ��
				{
					foodGroup[i].foodX = x;
					foodGroup[i].foodY = y;
					foodGroup[i].foodLife = (rand_food_x() * 100) % 22 + 7;//ʳ�������
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
	//�������ɵ�����
	isGenerate = (rand() * 100) % 18;
	if (isGenerate < 12)
	{
		if (x > 0 && x < MAPX && y>0 && y < MAPY && map[x][y] != 1 && poisonCount < (poisonNum - 1) && map[x][y] != 2)
		{
			for (int i = 0; i < poisonNum; i++) {//��һ��λ�ô���ʳ��
				if (poisonGroup[i].status == 0)//��λ�ñ���������ʳ���ʱ��
				{
					poisonGroup[i].poisonX = x;
					poisonGroup[i].poisonY = y;
					poisonGroup[i].posionLife = (rand_food_x() * 100) % 22 + 7;//����
					poisonGroup[i].status = 1;
					map[x][y] = 7;
					i = poisonNum;
				}
			}
			poisonCount += 1;

			gotoxy(7, 45);
			printf("���϶����� %d", poisonCount);
		}
	}
}

//����ʳ��������//
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
{	//����
	/*if (head->positionX == tail->positionX && head->positionY == tail->positionY) {
		gameStatus = 0;
		gotoxy(21, 0);
		printf("��ֻʣһ���ˣ�����������");
	}*/

	int result = 0;
	int x = head->positionX;
	int y = head->positionY;
	//����ͷ�����ж�,�Ե�ʳ���ɾ��β��
	if (map[x][y] == 5)//ͷ�������ʳ��������غ�
	{
		for (int i = 0; i < foodNum; i++)
			if (foodGroup[i].foodX == x && foodGroup[i].foodY == y)
			{
				gotoxy(21, 0);
				printf("Good You eat a food !");
				foodGroup[i].status = 0;
				foodCount -= 1;//�����⣬����������������д�Ե��м�һ�� �������һ������������
				score += 100;
				result = 1;
				return result;

			}
	}

	if (map[x][y] == 7)//ͷ������Ͷ���������غ�
	{
		for (int i = 0; i < poisonNum; i++)
			if (poisonGroup[i].poisonX == x && poisonGroup[i].poisonY == y)
			{	//�ҵ��Ե�������
				gotoxy(21, 0);
				printf("No no no!!      ");
				poisonGroup[i].status = 0;
				poisonCount -= 1;
				score -= 100;
				result = 2;
				return result;

			}
	}
	//ײǽ�ˣ�
	else if (map[x][y] == 1)
		gameStatus = 0;

	//ҧ���Լ�
	else if (map[x][y] == 2)//��ͷ��������������غ�
	{
		//���ҵ�ҧ���ĵط�
		snake * find;
		find = head->next;
		int loseNum = 0;//��ҧ���Ľ���
		while (find != NULL)//��������
		{
			if (find->positionX == x && find->positionY == y)
			{
				gotoxy(21, 8);
				printf("����ʹ������ʧȥ��%d��", loseNum+1);
				dimageShow = 5;
				//ɾ��֮��Ľڵ�
				snake * del;
				int count = 0;
				//��β�ڵ㿪ʼɾ��
				do
				{	
					count++;
					map[tail->positionX][tail->positionY] = 0;
					tail = tail->previous;
					free(tail->next);
					tail->next = NULL;
				} while (tail->positionX == find->positionX && tail->positionY == find->positionY);
				//�������һ��
				//gotoxy(22, 0);
				//printf("ʹ������������ʧȥ��%d",count+1);
				break;
			}
			find = find->next;
		}
	}
			
	
	return result;
}

void Clearer()
{
	//���д��ڵ�ʳ������� -1
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

	//���д��ڵĶ�������� -1
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
		printf(" ");//����߹���
		gotoxy(25, 0);
	for (int i = 0; i < 10; i++) {//��ӡָ��
		char k = key[i];
		switch (k)
		{
		case KEYUP:
			printf(" �� ");
			break;

		case KEYDOWN:
			printf(" �� ");
			break;

		case KEYLEFT:
			printf(" �� ");
			break;

		case KEYRIGHT:
			printf(" �� ");
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


void BFS()
{
	//���ص�ͼ
	
	for (int i = 0; i < MAPX; i++)
	{
		for (int j = 0; j < MAPY; j++)
		{
			maze[i][j] = map[i][j];//����ͼ����һ��
		}
	}
	//ȷ��Ŀ������
	int tarX = foodGroup[0].foodX;
	int tarY = foodGroup[0].foodY;
	maze[tarX][tarY] = 9;//���ϼǺ�--�յ�
	maze[head->positionX][head->positionY] = 8;//��ͷ--���
	int result = 0;
}

void DFSmove(int curx, int cury,int endX,int endY)//��Ӧ�� autokey
{
	int result = 0;
	maze[curx][cury] = 10;//�㼣
	if (curx == endX && cury == endY) //�ߵ����յ�ʱ��
		result = 1;

	if (result != 1 && maze[curx][cury + 1] == 0)//�ұ�Ϊ�� ������
		move(curx, cury + 1);

	if (result != 1 && maze[curx][cury - 1] == 0)//���Ϊ�� ������
		move(curx, cury - 1);

	if (result != 1 && maze[curx + 1][cury] == 0)
		move(curx + 1, cury);

	if (result != 1 && maze[curx - 1][cury] == 0)
		move(curx - 1, cury);
	if (result == 0)//δ�ߵ��յ�
		maze[curx][cury] = 1;//�߹���·��ȫ����������

}

void loadKey()
{
	int i = 0;
	while (autoKey[i] != 0)
	{	
		step = 0;
		key[i] = autoKey[i];
		autoKey[i] = 0;
		i++;
	}
}

void clearKey() {  //������а���
	int i = 1;
	while (key[i] != 0)
	{
		key[i] = 0;
		directionX[i] = 0;
		directionY[i] = 0;
		i++;
	}

}