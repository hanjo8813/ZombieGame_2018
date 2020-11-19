#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<mmsystem.h>
#include "stage.h"
#include "UI.h"
#pragma comment(lib,"winmm.lib")

#define ONE 49
#define TWO 50   
#define THREE 51

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACEBAR 32
#define f 102
#define q 113
#define w 119

unsigned int intextS_t, intextE_t;
unsigned int textS_t, textE_t;
unsigned int keyS_t, keyE_t;
unsigned int comboStart_t, comboEnd_t;
unsigned int old_t, now_t;
int stagetime = 0;
int increasetime;

int opentest = 0;
int in = 0;
int p = 0;
int sign = 0;
int character = 0;
int gameover = 0;
int stageclear = 0;
int stage = 0;
int speed = 30;
int life;
int score = 0;
int combo = 0;
int num_gun = 0;
int jj = 0; //장전
int num_bomb = 0;
int keychange = 0;
int cnt7 = 0;

int move_A = 0;
int curPosX, curPosY;
int curPosX_GUN, curPosY_GUN;
int curPosX_ANPC[10], curPosY_ANPC[10];
int curPosX_BNPC[10], curPosY_BNPC[10];
int curPosX_CNPC[10], curPosY_CNPC[10];
int curPosX_DNPC[10], curPosY_DNPC[10];

int gameBoardInfo[GBOARD_HEIGHT][GBOARD_WIDTH] = { 0 };

void Delete_GUN();
void Show_GUN();
void gun();
void bomb();
void num_jj();
void ProcessKeyInput();
void DrawExplainboard(int sign);
void DrawCharacterboard(int sign);
void DeletePC();
void Delete_ANPC(int n);
void Delete_BNPC(int n);
void Delete_CNPC(int n);
void Delete_DNPC(int n);
void ShowPC();

void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD GetCurrentCursorPos()
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void DrawStartboard(int sign)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	int key;
	int i, j;
	system("cls");
	SetConsoleTextAttribute(hC, 15);
	for (j = 0; j < GBOARD_HEIGHT; j++) {
		for (i = 0; i < GBOARD_WIDTH; i++) {
			if (gamestartboard[j][i] == 1) {
				printf("■");
			}
			else if (gamestartboard[j][i] == 2) {
				SetConsoleTextAttribute(hC, 12);
				printf("■");
				SetConsoleTextAttribute(hC, 15);
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}

	SetConsoleTextAttribute(hC, 14);
	printf("\n\n\t\t\tC H A R A C T E R  : ");
	SetConsoleTextAttribute(hC, 15);

	if (character == 0)
	{
		SetConsoleTextAttribute(hC, 14);
		printf("●\n\n");
		SetConsoleTextAttribute(hC, 15);
	}
	if (character == 1)
	{
		SetConsoleTextAttribute(hC, 14);
		printf("♠\n\n");
		SetConsoleTextAttribute(hC, 15);
	}
	if (character == 2)
	{
		SetConsoleTextAttribute(hC, 14);
		printf("♣\n\n");
		SetConsoleTextAttribute(hC, 15);
	}
	printf("\t\t\t  ①   S  T  A  R  T\n\n");
	printf("\t\t\t  ②   게  임  설  명\n\n");
	printf("\t\t\t③   캐  릭  터  설  정\n\n");

	while (1)
	{
		if (_kbhit() != 0)
		{
			key = _getch();

			switch (key)
			{
			case ONE:
				sign = 1;
				break;

			case TWO:
				DrawExplainboard(sign);
				break;

			case THREE:
				DrawCharacterboard(sign);
				break;
			}
		}
		if (sign == 1)
			return;
	}
}

void DrawExplainboard(int sign)
{
	int key;

	system("cls");
	printf("\n    <게임 목적>\n    60초 내에 NPC를 피해 오염된 땅을 모두 정화하는 것\n\n");
	printf("    <게임 기본 규칙>\n    목숨 3개, NPC와 부딪히면 목숨이 1개 사라짐\n    스테이지에서 남은 시간은 1초당 100점 씩 점수를 얻음\n    스테이지에서 정화시킨 땅은 땅의 한 칸당 50점 씩 점수를 얻음\n\n");
	printf("    <NPC>\n    좀비 ▲ : 아무 기능 없이 특정 구역만 반복해서 움직임\n    좀비 ■ : 아무 기능 없이 맵 전체를 움직임\n    좀비 ◆ : 땅을 오염시키면서 맵 전체를 움직임\n    좀비 ◈ : 나를 추적하면서 맵 전체를 움직임\n\n");
	printf("    <아이템>\n    땅을 정화시키면 랜덤으로 아이템이 나옴\n    - 종류\n    시간 증가 아이템 : 스테이지 시간 10초 증가\n    목숨 증가 아이템 : 목숨 1개 증가\n    총 아이템 : 스페이스 바를 누르고 원하는 방향의 방향키를 누르면 총을 쏴서 NPC를 죽일 수 있는 아이템\n    방향키 전환 아이템 : 7초동안 PC의 방향키가 상하, 좌우가 바뀜\n\n\n");
	printf("    1 누르면 게임시작 \n\n    2 누르면 시작화면");

	while (1)
	{
		if (_kbhit() != 0)
		{
			key = _getch();

			switch (key)
			{
			case ONE:
				sign = 1;
				break;

			case TWO:
				DrawStartboard(sign);
				break;
			}
		}
		if (sign == 1)
			return;
	}
}

void DrawCharacterboard(int sign)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);

	int key;
	system("cls");

	SetConsoleTextAttribute(hC, 14);
	printf("\n   ●");
	SetConsoleTextAttribute(hC, 15);
	printf("   < 방향키 입력\n\n");
	SetConsoleTextAttribute(hC, 14);
	printf("   ♠");
	SetConsoleTextAttribute(hC, 15);
	printf("   > 방향키 입력\n\n");
	SetConsoleTextAttribute(hC, 14);
	printf("   ♣");
	SetConsoleTextAttribute(hC, 15);
	printf("   ＾ 방향키 입력\n\n");

	printf("\n    1 누르면 게임시작 \n\n    2 누르면 시작화면");

	while (1)
	{
		if (_kbhit() != 0)
		{
			key = _getch();

			switch (key)
			{
			case ONE:
				sign = 1;
				break;
			case TWO:
				DrawStartboard(sign);
				break;
			case LEFT:
				character = 0;
				DrawStartboard(sign);
				break;
			case RIGHT:
				character = 1;
				DrawStartboard(sign);
				break;
			case UP:
				character = 2;
				DrawStartboard(sign);
			}
		}
		if (sign == 1)
			return;
	}
}

void printstory()
{
	system("cls");
	PlaySound(TEXT("키보드.wav"), NULL, SND_ASYNC);
	char story[8][150] = {
	{"                    < 게 임  시 나 리 오 >" },
	{" "},
	{ "      인간의 과도한 유전자 조작으로 지구에는 좀비가 나타났다!" },
	{ "            이 때문에 많은 사람들은 좀비가 되었고," },
	{ "지구를 구하기 위해서는 남은 사람들이 좀비 바이러스를 정화시켜야 한다." },
	{ "             하지만 돌아온 사람은 아무도 없었다." },
	{ "               모두 두려움에 떨고 있는 상황..." },
	{ "        당신이 좀비 바이러스를 정화시키러 가시겠습니까?" },
	};

	int x = 4;
	int y = 1;
	int i, j;
	SetCurrentCursorPos(x, y);

	for (i = 0; i < 8; i++)
	{
		SetCurrentCursorPos(x, y + 2 * i);

		if (i == 1) {
			SetCurrentCursorPos(x, 25);
			printf("                시작하려면 아무 키나 누르십시오");
			SetCurrentCursorPos(x, y + 2 * i);
		}

		for (j = 0; j < strlen(story[i]); j++) {
			if (_kbhit()) {
				break;
			}
			else {
				printf("%c", story[i][j]);
				Sleep(50);
			}
		}
	}
	Sleep(500);
}

void DrawStage()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");

	int i, j;

	for (j = 0; j < GBOARD_HEIGHT; j++)
	{
		for (i = 0; i < GBOARD_WIDTH; i++)
		{
			if (stagemap[stage][j][i] == 0) // 빈공간
			{
				SetCurrentCursorPos(i * 2, j);
				printf(" ");
			}
			else if (stagemap[stage][j][i] == 4)
			{
				SetCurrentCursorPos(i * 2, j);
				SetConsoleTextAttribute(hC, 10);
				printf("■");
				SetConsoleTextAttribute(hC, 15);
			}
			else if (stagemap[stage][j][i] == 1) // 벽
			{
				SetCurrentCursorPos(i * 2, j);
				SetConsoleTextAttribute(hC, 15);
				printf("▒");
			}
			else if (stagemap[stage][j][i] == 2)  // 정화되기전 땅
			{
				SetCurrentCursorPos(i * 2, j);
				SetConsoleTextAttribute(hC, 15);
				printf("■");
			}
			else if (stagemap[stage][j][i] == 3) // 바이러스
			{
				SetCurrentCursorPos(i * 2, j);
				SetConsoleTextAttribute(hC, 8);
				printf("○");
			}
			else if (stagemap[stage][j][i] == 5) // 폭탄
			{
				SetCurrentCursorPos(i * 2, j);
				SetConsoleTextAttribute(hC, 6);
				printf("◐");
			}
			else if (stagemap[stage][j][i] == 6) // 문
			{
				SetCurrentCursorPos(i * 2, j);
				SetConsoleTextAttribute(hC, 5);
				printf("▦");
			}
			else if (stagemap[stage][j][i] == 8) // 포탈
			{
				SetCurrentCursorPos(i * 2, j);
				SetConsoleTextAttribute(hC, 9);
				printf("＠");
			}
		}
	}
}

void DrawGameBoard()
{
	for (int i = 0; i < GBOARD_HEIGHT; i++)
		memmove(gameBoardInfo[i], stagemap[stage][i], GBOARD_WIDTH * 4);
	DrawStage();
}

void DrawUIBoard()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	int x, y;

	for (y = 0; y <= 29; y++)
	{
		SetCurrentCursorPos(72, y);
		SetConsoleTextAttribute(hC, 15);
		printf("▒");
	}

	for (x = 72; x <= 102; x += 2)
	{
		SetCurrentCursorPos(x, 0);
		SetConsoleTextAttribute(hC, 15);
		printf("▒");
	}

	for (y = 1; y <= 29; y++)
	{
		SetCurrentCursorPos(102, y);
		SetConsoleTextAttribute(hC, 15);
		printf("▒");
	}

	for (x = 72; x <= 102; x += 2)
	{
		SetCurrentCursorPos(x, 29);
		SetConsoleTextAttribute(hC, 15);
		printf("▒");
	}
}

void UI_Stage()
{
	SetCurrentCursorPos(76, 2);
	printf("STAGE %d", stage + 1);
}

void UI_TimeLimit()
{
	now_t = clock();
	int consume_t = (now_t - old_t) / 1000;
	consume_t = stagetime - consume_t;

	SetCurrentCursorPos(76, 5);
	printf("< 남은 시간 >");

	if (consume_t >= 0)
	{
		SetCurrentCursorPos(76, 7);
		printf("%02d:%02d", consume_t / 60, consume_t % 60);

		if ((consume_t / 60) == 0 && (consume_t % 60 == 0))
			gameover = 1;
	}
	SetCurrentCursorPos(76, 9);
	printf("------------------------");
}

void UI_Score()
{
	SetCurrentCursorPos(76, 11);
	printf("< 점수 >");
	SetCurrentCursorPos(76, 13);
	printf("COMBO : %d", combo);
	SetCurrentCursorPos(76, 15);
	printf("현재점수 : %d", score);
	SetCurrentCursorPos(76, 17);
	printf("------------------------");
}

void UI_Life()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);

	SetCurrentCursorPos(76, 19);
	printf("< 현재목숨 >");
	SetCurrentCursorPos(76, 21);
	SetConsoleTextAttribute(hC, 12);

	if (life > 5)
	{
		life = 5;
	}

	if (life == 5) {
		printf("♥♥♥♥♥");
	}
	if (life == 4) {
		printf("♥♥♥♥♡");
	}
	if (life == 3) {
		printf("♥♥♥♡♡");
	}
	if (life == 2) {
		printf("♥♥♡♡♡");
	}
	if (life == 1) {
		printf("♥♡♡♡♡");
	}
	SetConsoleTextAttribute(hC, 15);
	SetCurrentCursorPos(76, 23);
	printf("------------------------");
}

void UI_Gun()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentCursorPos(76, 25);
	SetConsoleTextAttribute(hC, 10);
	printf("( Q )");
	SetConsoleTextAttribute(hC, 15);
	printf(" 총알 개수 : %d", num_gun);

	SetCurrentCursorPos(76, 27);
	SetConsoleTextAttribute(hC, 10);
	printf("( W )");
	SetConsoleTextAttribute(hC, 15);
	printf(" 폭탄 개수 : %d", num_bomb);

}

void DrawGameUI()
{
	UI_Stage();
	UI_TimeLimit();
	UI_Score();
	UI_Life();
	UI_Gun();
}

void DrawMessage()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	int x, y;

	for (x = 16; x <= 52; x += 2)
	{
		SetCurrentCursorPos(x, 23);
		printf("─");
	}
	for (x = 16; x <= 52; x += 2)
	{
		SetCurrentCursorPos(x, 29);
		printf("─");
	}
	for (y = 24; y <= 28; y++)
	{
		SetCurrentCursorPos(16, y);
		printf("│");
	}
	for (y = 24; y <= 28; y++)
	{
		SetCurrentCursorPos(52, y);
		printf("│");
	}

	SetCurrentCursorPos(16, 23);
	printf("┌");
	SetCurrentCursorPos(52, 23);
	printf("┐");
	SetCurrentCursorPos(16, 29);
	printf("└");
	SetCurrentCursorPos(52, 29);
	printf("┘");
}

void DeleteMessage1()
{
	for (int x = 18; x < 50; x++)
	{
		SetCurrentCursorPos(x, 26);
		printf("  ");
	}
}

void DeleteMessage2()
{
	for (int x = 18; x < 50; x++)
	{
		SetCurrentCursorPos(x, 24);
		printf("  ");
		SetCurrentCursorPos(x, 25);
		printf("  ");
	}
}

// --------------------------------------------------------------------------------------

void StageClearTest()
{
	int i, j;
	for (i = 0; i < GBOARD_HEIGHT; i++)
	{
		for (j = 0; j < GBOARD_WIDTH; j++)
		{
			if (gameBoardInfo[i][j] == 3)
				return;
		}
	}

	stageclear = 1;
}

void PrintClear()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	int i, j;
	system("cls");

	for (j = 0; j < GBOARD_HEIGHT; j++) {
		for (i = 0; i < GBOARD_WIDTH; i++) {
			if (gameclearboard[j][i] == 1) {
				printf("■");
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}
	SetConsoleTextAttribute(hC, 14);
	printf("\t\t\t   S C O R E  :  %d", score);
	SetConsoleTextAttribute(hC, 15);

	Sleep(2000);
}
void GameClear()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	PlaySound(TEXT("20 - eien no yami.wav"), NULL, SND_ASYNC);
	int key;
	system("cls");

	char story[9][150] =
	{
	{ " 좀비 바이러스가 모두 제거되었다 .... !" },
	{" ....."},
	{" ....."},

	{ " 핏빛으로 물들었던 온 세상은 다시 돌아오고" },
	{ " 사람들은 다시 행복한 삶을 되찾았다" },
	{" ....."},
	{" ....."},
	{" ....."},
	{ " 나는 영웅이다...! " }
	};

	int x = 4;
	int y = 1;
	int i, j;
	SetCurrentCursorPos(x, y);

	for (i = 0; i < 9; i++)
	{
		SetCurrentCursorPos(x, y + 2 * i);

		for (j = 0; j < strlen(story[i]); j++) 
		{
				printf("%c", story[i][j]);
				Sleep(50);
		}
	}

	SetConsoleTextAttribute(hC, 14);
	printf("\n\n\n\n\t 최  종  점  수  :  %d", score);
	SetConsoleTextAttribute(hC, 15);
	printf("\n\n\n      시작화면으로 가려면 1번을 누르시오");

	while (1)
	{
		if (_kbhit() != 0)
		{
			key = _getch();

			switch (key)
			{
			case ONE:
				return;
			}
		}
	}
}

void GameOver()
{
	PlaySound(TEXT("게임오버.wav"), NULL, SND_ASYNC);

	system("cls");

	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	int i, j, key;

	for (j = 0; j < GBOARD_HEIGHT - 5; j++) {
		for (i = 0; i < GBOARD_WIDTH; i++) {
			if (gameoverboard[j][i] == 1) {
				printf("■");
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}

	char story[70] = { "      결국 세상은 좀비 바이러스에 의해 멸망하고 말았다 . . . . . " };

	for (int i = 0; i < 70; i++)
	{
		SetConsoleTextAttribute(hC, 12);
		printf("%c", story[i]);
		Sleep(30);
	}
	Sleep(500);

	SetConsoleTextAttribute(hC, 14);
	printf("\n\n\n\n\t\t      S C O R E  :  %d", score);
	SetConsoleTextAttribute(hC, 15);
	printf("\n\n\t      시작화면으로 가려면 1번을 누르시오");

	while (1)
	{
		if (_kbhit() != 0)
		{
			key = _getch();

			switch (key)
			{
			case ONE:
				return;
			}
		}
	}
}


// --------------------------------------------------------------------------------------

int DetectCollision(int curPosX, int curPosY)
{
	if (gameBoardInfo[curPosY][curPosX / 2] == 0 || gameBoardInfo[curPosY][curPosX / 2] == 3
		|| gameBoardInfo[curPosY][curPosX / 2] == 7 || gameBoardInfo[curPosY][curPosX / 2] == 8 || gameBoardInfo[curPosY][curPosX / 2] == 10 || gameBoardInfo[curPosY][curPosX / 2] == 5)
		return 0;
	if (gameBoardInfo[curPosY][curPosX / 2] == 6)
		return 2;
	return 1;
}
void PC_Bomb_Collision()
{
	if (gameBoardInfo[curPosY][curPosX / 2] == 5)
	{
		life--;

		ShowPC();
		gameBoardInfo[curPosY][curPosX / 2] = 0;
		SetCurrentCursorPos(20, 26);
		printf("폭탄을 밟아서 목숨이 줄었다.  ");
		Sleep(150);
	}
}
int DetectCollision_Gun(int curPosX, int curPosY)
{
	if (gameBoardInfo[curPosY][curPosX / 2] == 0 || gameBoardInfo[curPosY][curPosX / 2] == 3
		|| gameBoardInfo[curPosY][curPosX / 2] == 6 || gameBoardInfo[curPosY][curPosX / 2] == 7 || gameBoardInfo[curPosY][curPosX / 2] == 8)
		return 0;
	return 1;
}

void NPC_Bomb_Collision()
{
	for (int a = 0; a < 5;a++)
	{
		if (gameBoardInfo[curPosY_ANPC[a]][curPosX_ANPC[a] / 2] == 5)
		{
			Delete_ANPC(a);
			gameBoardInfo[curPosY_ANPC[a]][curPosX_ANPC[a] / 2] = 0;
			curPosX_ANPC[a] = 2;
			curPosY_ANPC[a] = 20;
			SetCurrentCursorPos(curPosX_ANPC[a], curPosY_ANPC[a]);
			SetCurrentCursorPos(20, 26);
			printf("NPC가 폭탄을 밟았다!!      ");
			Sleep(150);
			//   Delete_ANPC(a);
		}

	}

	for (int b = 0; b < 5;b++)
	{
		if (gameBoardInfo[curPosY_BNPC[b]][curPosX_BNPC[b] / 2] == 5)
		{
			Delete_BNPC(b);
			gameBoardInfo[curPosY_BNPC[b]][curPosX_BNPC[b] / 2] = 0;
			curPosX_BNPC[b] = 2;
			curPosY_BNPC[b] = 20;
			SetCurrentCursorPos(curPosX_BNPC[b], curPosY_BNPC[b]);
			Delete_BNPC(b);
			SetCurrentCursorPos(20, 26);
			printf("NPC가 폭탄을 밟았다!!      ");
			Sleep(150);
		}

	}

	for (int c = 0; c < 5;c++)
	{
		if (gameBoardInfo[curPosY_CNPC[c]][curPosX_CNPC[c] / 2] == 5)
		{
			Delete_CNPC(c);
			gameBoardInfo[curPosY_CNPC[c]][curPosX_CNPC[c] / 2] = 0;

			curPosX_CNPC[c] = 2;
			curPosY_CNPC[c] = 20;
			SetCurrentCursorPos(curPosX_CNPC[c], curPosY_CNPC[c]);
			Delete_CNPC(c);
			SetCurrentCursorPos(20, 26);
			printf("NPC가 폭탄을 밟았다!!      ");
			Sleep(150);
		}

	}

	for (int d = 0; d < 5;d++)
	{
		if (gameBoardInfo[curPosY_DNPC[d]][curPosX_DNPC[d] / 2] == 5)
		{
			Delete_DNPC(d);
			gameBoardInfo[curPosY_DNPC[d]][curPosX_DNPC[d] / 2] = 0;

			curPosX_DNPC[d] = 2;
			curPosY_DNPC[d] = 20;
			SetCurrentCursorPos(curPosX_DNPC[d], curPosY_DNPC[d]);
			Delete_DNPC(d);
			SetCurrentCursorPos(20, 26);
			printf("NPC가 폭탄을 밟았다!!      ");
			Sleep(150);
		}

	}

}
void PC_NPC_Collision()
{
	for (int a = 0; a < 5; a++)
	{
		if (curPosX == curPosX_ANPC[a] && curPosY == curPosY_ANPC[a])
		{
			life--;
			SetCurrentCursorPos(20, 26);
			printf("앗 조심해! 목숨이 줄어들었다!");
		}
	}
	for (int b = 0; b < 5; b++)
	{
		if (curPosX == curPosX_BNPC[b] && curPosY == curPosY_BNPC[b])
		{
			life--;
			SetCurrentCursorPos(20, 26);
			printf("앗 조심해! 목숨이 줄어들었다!");
		}
	}
	for (int c = 0; c < 5; c++)
	{
		if (curPosX == curPosX_CNPC[c] && curPosY == curPosY_CNPC[c])
		{
			SetCurrentCursorPos(20, 26);
			printf("앗 조심해! 목숨이 줄어들었다!");
			life--;
		}
	}
	for (int d = 0; d < 5; d++)
	{
		if (curPosX == curPosX_DNPC[d] && curPosY == curPosY_DNPC[d])
		{
			life--;
			SetCurrentCursorPos(20, 26);
			printf("앗 조심해! 목숨이 줄어들었다!");
		}
	}
}

void ShowPC()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentCursorPos(curPosX, curPosY);
	if (character == 0)
	{
		if (jj == 0)
		{
			SetConsoleTextAttribute(hC, 14);
			printf("●");
			SetConsoleTextAttribute(hC, 15);
		}
		else
		{
			SetConsoleTextAttribute(hC, 11);
			printf("●");
			SetConsoleTextAttribute(hC, 15);
		}
	}
	if (character == 1)
	{
		if (jj == 0)
		{
			SetConsoleTextAttribute(hC, 14);
			printf("♠");
			SetConsoleTextAttribute(hC, 15);
		}
		else
		{
			SetConsoleTextAttribute(hC, 11);
			printf("♠");
			SetConsoleTextAttribute(hC, 15);
		}
	}
	if (character == 2)
	{
		if (jj == 0)
		{
			SetConsoleTextAttribute(hC, 14);
			printf("♣");
			SetConsoleTextAttribute(hC, 15);
		}
		else
		{
			SetConsoleTextAttribute(hC, 11);
			printf("♣");
			SetConsoleTextAttribute(hC, 15);
		}
	}
	SetCurrentCursorPos(curPosX, curPosY);
}

void DeletePC()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);

	SetCurrentCursorPos(curPosX, curPosY);
	if (gameBoardInfo[curPosY][curPosX / 2] == 3 || gameBoardInfo[curPosY][curPosX / 2] == 7) // PC 이동할 때 gameBoardInfo에 0 저장
		gameBoardInfo[curPosY][curPosX / 2] = 0;

	if (gameBoardInfo[curPosY][curPosX / 2] == 8 || gameBoardInfo[curPosY][curPosX / 2] == 5)
	{
		if (gameBoardInfo[curPosY][curPosX / 2] == 8)
		{
			SetConsoleTextAttribute(hC, 9);
			printf("＠");
			SetConsoleTextAttribute(hC, 15);
		}
		if (gameBoardInfo[curPosY][curPosX / 2] == 5)
		{
			SetConsoleTextAttribute(hC, 6);
			printf("◐");
			SetConsoleTextAttribute(hC, 15);
		}
	}
	else
	{
		printf("  ");
		SetCurrentCursorPos(curPosX, curPosY);
	}
}

void MoveUp_PC()
{
	if (DetectCollision(curPosX, curPosY - 1) == 1)
		return;
	if (DetectCollision(curPosX, curPosY - 1) == 2)
	{
		SetCurrentCursorPos(20, 26);
		printf("총으로 벽을 뚫을수 있을것 같다");
		return;
	}

	DeletePC();
	curPosY -= 1;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowPC();
}

void MoveDown_PC()
{
	if (DetectCollision(curPosX, curPosY + 1) == 1)
		return;
	if (DetectCollision(curPosX, curPosY + 1) == 2)
	{
		SetCurrentCursorPos(20, 26);
		printf("총으로 벽을 뚫을수 있을것 같다");
		return;
	}
	DeletePC();
	curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowPC();
}

void MoveLeft_PC()
{
	if (DetectCollision(curPosX - 2, curPosY) == 1)
		return;
	if (DetectCollision(curPosX - 2, curPosY) == 2)
	{
		SetCurrentCursorPos(20, 26);
		printf("총으로 벽을 뚫을수 있을것 같다");
		return;
	}
	DeletePC();
	curPosX -= 2;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowPC();
}

void MoveRight_PC()
{
	if (DetectCollision(curPosX + 2, curPosY) == 1)
		return;
	if (DetectCollision(curPosX + 2, curPosY) == 2)
	{
		SetCurrentCursorPos(20, 26);
		printf("총으로 벽을 뚫을수 있을것 같다");
		return;
	}
	DeletePC();
	curPosX += 2;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowPC();
}

void Stage2_Open()
{
	//오른쪽 위 클리어 검사
	if (opentest == 0)
	{
		for (int x = 2; x < 17; x++)
		{
			for (int y = 1; y < 11; y++)
			{
				if (gameBoardInfo[y][x] == 3)
				{
					return;
				}
			}
		}
		opentest = 1;
	}

	//왼쪽 위 클리어 검사
	if (opentest == 2)
	{
		for (int x = 18; x < 35; x++)
		{
			for (int y = 1; y < 11; y++)
			{
				if (gameBoardInfo[y][x] == 3)
				{
					return;
				}
			}
		}
		opentest = 3;
	}

	//왼쪽 아래 클리어 검사
	if (opentest == 4)
	{
		for (int x = 18; x < 35; x++)
		{
			for (int y = 12; y < 22; y++)
			{
				if (gameBoardInfo[y][x] == 3)
				{
					return;
				}
			}
		}
		opentest = 5;
	}

	// 문열림 //
	if (opentest == 1)
	{
		gameBoardInfo[6][17] = 0;
		gameBoardInfo[5][17] = 0;
		SetCurrentCursorPos(34, 5);
		printf(" ");
		SetCurrentCursorPos(34, 6);
		printf(" ");
		opentest = 2;
	}
	if (opentest == 3)
	{
		gameBoardInfo[11][25] = 0;
		gameBoardInfo[11][26] = 0;
		SetCurrentCursorPos(50, 11);
		printf(" ");
		SetCurrentCursorPos(52, 11);
		printf(" ");
		opentest = 4;
	}
	if (opentest == 5)
	{
		gameBoardInfo[16][17] = 0;
		gameBoardInfo[17][17] = 0;
		SetCurrentCursorPos(34, 16);
		printf(" ");
		SetCurrentCursorPos(34, 17);
		printf(" ");
		opentest = 6;
	}
}

void Portal()
{
	if (curPosX == 14 && curPosY == 3) // 왼쪽 위 입구
	{
		p = 1;
		DeletePC();
		curPosX = 64;
		curPosY = 19;
		SetCurrentCursorPos(curPosX, curPosY);
		ShowPC();
		return;
	}
	if (curPosX == 4 && curPosY == 18) // 왼쪽 아래 입구
	{
		p = 2;
		DeletePC();
		curPosX = 64;
		curPosY = 19;
		SetCurrentCursorPos(curPosX, curPosY);
		ShowPC();
		return;
	}
	if (curPosX == 34 && curPosY == 16) // 가운데 입구
	{
		p = 3;
		DeletePC();
		curPosX = 64;
		curPosY = 19;
		SetCurrentCursorPos(curPosX, curPosY);
		ShowPC();
		return;
	}
	if (curPosX == 58 && curPosY == 3) // 오른쪽 위 입구
	{
		p = 4;
		DeletePC();
		curPosX = 64;
		curPosY = 19;
		SetCurrentCursorPos(curPosX, curPosY);
		ShowPC();
		return;
	}
	if (curPosX == 66 && curPosY == 17) // 오른쪽 아래 입구
	{
		p = 5;
		DeletePC();
		curPosX = 64;
		curPosY = 19;
		SetCurrentCursorPos(curPosX, curPosY);
		ShowPC();
		return;
	}

	// 출구
	if (p == 1)
	{
		if (curPosX == 66 && curPosY == 20)
		{
			DeletePC();
			curPosX = 14;
			curPosY = 3;
			SetCurrentCursorPos(curPosX, curPosY);
			ShowPC();
		}
	}
	if (p == 2)
	{
		if (curPosX == 66 && curPosY == 20)
		{
			DeletePC();
			curPosX = 4;
			curPosY = 18;
			SetCurrentCursorPos(curPosX, curPosY);
			ShowPC();
		}
	}
	if (p == 3)
	{
		if (curPosX == 66 && curPosY == 20)
		{
			DeletePC();
			curPosX = 34;
			curPosY = 16;
			SetCurrentCursorPos(curPosX, curPosY);
			ShowPC();
		}
	}
	if (p == 4)
	{
		if (curPosX == 66 && curPosY == 20)
		{
			DeletePC();
			curPosX = 58;
			curPosY = 3;
			SetCurrentCursorPos(curPosX, curPosY);
			ShowPC();
		}
	}
	if (p == 5)
	{
		if (curPosX == 66 && curPosY == 20)
		{
			DeletePC();
			curPosX = 66;
			curPosY = 17;
			SetCurrentCursorPos(curPosX, curPosY);
			ShowPC();
		}
	}
}

// --------------------------------------------------------------------------------------

void Show_ANPC(int n)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentCursorPos(curPosX_ANPC[n], curPosY_ANPC[n]);
	SetConsoleTextAttribute(hC, 12);
	printf("▲");
	SetConsoleTextAttribute(hC, 15);
	SetCurrentCursorPos(curPosX_ANPC[n], curPosY_ANPC[n]);
}

void Delete_ANPC(int n)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);

	SetCurrentCursorPos(curPosX_ANPC[n], curPosY_ANPC[n]);

	if (gameBoardInfo[curPosY_ANPC[n]][curPosX_ANPC[n] / 2] == 7)
		gameBoardInfo[curPosY_ANPC[n]][curPosX_ANPC[n] / 2] = 0;

	if (gameBoardInfo[curPosY_ANPC[n]][curPosX_ANPC[n] / 2] == 3)
	{
		SetConsoleTextAttribute(hC, 8);
		printf("○");
		SetConsoleTextAttribute(hC, 15);
	}

	else
		printf(" ");
	SetCurrentCursorPos(curPosX_ANPC[n], curPosY_ANPC[n]);
}

void MoveUp_ANPC(int n)
{
	if (DetectCollision(curPosX_ANPC[n], curPosY_ANPC[n] - 1) != 0)
		return;
	Delete_ANPC(n);
	curPosY_ANPC[n] -= 1;
	SetCurrentCursorPos(curPosX_ANPC[n], curPosY_ANPC[n]);
	Show_ANPC(n);
}

void MoveDown_ANPC(int n)
{
	if (DetectCollision(curPosX_ANPC[n], curPosY_ANPC[n] + 1) != 0)
		return;
	Delete_ANPC(n);
	curPosY_ANPC[n] += 1;
	SetCurrentCursorPos(curPosX_ANPC[n], curPosY_ANPC[n]);
	Show_ANPC(n);
}

void MoveLeft_ANPC(int n)
{
	if (DetectCollision(curPosX_ANPC[n] - 2, curPosY_ANPC[n]) != 0)
		return;
	Delete_ANPC(n);
	curPosX_ANPC[n] -= 2;
	SetCurrentCursorPos(curPosX_ANPC[n], curPosY_ANPC[n]);
	Show_ANPC(n);
}

void MoveRight_ANPC(int n)
{
	if (DetectCollision(curPosX_ANPC[n] + 2, curPosY_ANPC[n]) != 0)
		return;
	Delete_ANPC(n);
	curPosX_ANPC[n] += 2;
	SetCurrentCursorPos(curPosX_ANPC[n], curPosY_ANPC[n]);
	Show_ANPC(n);
}

void Move_ANPC_LR(int n, int leftX, int rightX)
{
	switch (move_A)
	{
	case 0:
		if (curPosX_ANPC[n] == leftX)
		{
			move_A = 1;
			break;
		}
		else
		{
			MoveLeft_ANPC(n);
			break;
		}
	case 1:
		if (curPosX_ANPC[n] == rightX)
		{
			move_A = 0;
			break;
		}
		else
		{
			MoveRight_ANPC(n);
			break;
		}
	}
}

void Move_ANPC_RL(int n, int leftX, int rightX)
{
	switch (move_A)
	{
	case 0:
		if (curPosX_ANPC[n] == rightX)
		{
			move_A = 0;
			break;
		}
		else
		{
			MoveRight_ANPC(n);
			break;
		}
	case 1:
		if (curPosX_ANPC[n] == leftX)
		{
			move_A = 1;
			break;
		}
		else
		{
			MoveLeft_ANPC(n);
			break;
		}
	}
}

void Move_ANPC_UD(int n, int upY, int downY)
{
	switch (move_A)
	{
	case 0:
		if (curPosY_ANPC[n] == upY)
		{
			move_A = 1;
			break;
		}
		else
		{
			MoveUp_ANPC(n);
			break;
		}
	case 1:
		if (curPosY_ANPC[n] == downY)
		{
			move_A = 0;
			break;
		}
		else
		{
			MoveDown_ANPC(n);
			break;
		}
	}
}

void Move_ANPC_DU(int n, int upY, int downY)
{
	switch (move_A)
	{
	case 0:
		if (curPosY_ANPC[n] == downY)
		{
			move_A = 0;
			break;
		}
		else
		{
			MoveDown_ANPC(n);
			break;
		}
	case 1:
		if (curPosY_ANPC[n] == upY)
		{
			move_A = 1;
			break;
		}
		else
		{
			MoveUp_ANPC(n);
			break;
		}
	}
}

// --------------------------------------------------------------------------------------

void Show_BNPC(int n)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentCursorPos(curPosX_BNPC[n], curPosY_BNPC[n]);
	SetConsoleTextAttribute(hC, 12);
	printf("■");
	SetConsoleTextAttribute(hC, 15);
	SetCurrentCursorPos(curPosX_BNPC[n], curPosY_BNPC[n]);
}

void Delete_BNPC(int n)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);

	SetCurrentCursorPos(curPosX_BNPC[n], curPosY_BNPC[n]);
	if (gameBoardInfo[curPosY_BNPC[n]][curPosX_BNPC[n] / 2] == 7)
		gameBoardInfo[curPosY_BNPC[n]][curPosX_BNPC[n] / 2] = 0;

	if (gameBoardInfo[curPosY_BNPC[n]][curPosX_BNPC[n] / 2] == 3)
	{
		SetConsoleTextAttribute(hC, 8);
		printf("○");
		SetConsoleTextAttribute(hC, 15);
	}

	else
		printf(" ");
	SetCurrentCursorPos(curPosX_BNPC[n], curPosY_BNPC[n]);
}

void MoveUp_BNPC(int n)
{
	if (DetectCollision(curPosX_BNPC[n], curPosY_BNPC[n] - 1) != 0)
		return;
	Delete_BNPC(n);
	curPosY_BNPC[n] -= 1;
	SetCurrentCursorPos(curPosX_BNPC[n], curPosY_BNPC[n]);
	Show_BNPC(n);
}

void MoveDown_BNPC(int n)
{
	if (DetectCollision(curPosX_BNPC[n], curPosY_BNPC[n] + 1) != 0)
		return;
	Delete_BNPC(n);
	curPosY_BNPC[n] += 1;
	SetCurrentCursorPos(curPosX_BNPC[n], curPosY_BNPC[n]);
	Show_BNPC(n);
}

void MoveLeft_BNPC(int n)
{
	if (DetectCollision(curPosX_BNPC[n] - 2, curPosY_BNPC[n]) != 0)
		return;
	Delete_BNPC(n);
	curPosX_BNPC[n] -= 2;
	SetCurrentCursorPos(curPosX_BNPC[n], curPosY_BNPC[n]);
	Show_BNPC(n);
}

void MoveRight_BNPC(int n)
{
	if (DetectCollision(curPosX_BNPC[n] + 2, curPosY_BNPC[n]) != 0)
		return;
	Delete_BNPC(n);
	curPosX_BNPC[n] += 2;
	SetCurrentCursorPos(curPosX_BNPC[n], curPosY_BNPC[n]);
	Show_BNPC(n);
}

void Move_BNPC(int n)
{
	int move = rand() % 4;

	switch (move)
	{
	case 0:
		MoveLeft_BNPC(n);
		break;
	case 1:
		MoveRight_BNPC(n);
		break;
	case 2:
		MoveUp_BNPC(n);
		break;
	case 3:
		MoveDown_BNPC(n);
		break;
	}
}

// --------------------------------------------------------------------------------------

void Show_CNPC(int n)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
	SetConsoleTextAttribute(hC, 12);
	printf("◆");
	SetConsoleTextAttribute(hC, 15);
	SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
}

void Delete_CNPC(int n)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	if (gameBoardInfo[curPosY_CNPC[n]][curPosX_CNPC[n] / 2] == 7)
		gameBoardInfo[curPosY_CNPC[n]][curPosX_CNPC[n] / 2] = 0;
	if (gameBoardInfo[curPosY_CNPC[n]][(curPosX_CNPC[n] - 2) / 2] == 2 ||  // 왼쪽에 오염된 땅이 있을때
		gameBoardInfo[curPosY_CNPC[n]][(curPosX_CNPC[n] + 2) / 2] == 2 ||  // 오른쪽에 오염된 땅이 있을때
		gameBoardInfo[curPosY_CNPC[n] - 1][curPosX_CNPC[n] / 2] == 2 ||  // 위쪽에 오염된 땅이 있을때 
		gameBoardInfo[curPosY_CNPC[n] + 1][curPosX_CNPC[n] / 2] == 2 ||  // 아래쪽에 오염된 땅이 있을때

		gameBoardInfo[curPosY_CNPC[n] - 1][(curPosX_CNPC[n] - 2) / 2] == 2 ||  // 왼쪽위에 오염된 땅이 있을때
		gameBoardInfo[curPosY_CNPC[n] - 1][(curPosX_CNPC[n] + 2) / 2] == 2 ||  // 오른쪽위에 오염된 땅이 있을때
		gameBoardInfo[curPosY_CNPC[n] + 1][(curPosX_CNPC[n] - 2) / 2] == 2 ||  // 왼쪽아래에 오염된 땅이 있을때
		gameBoardInfo[curPosY_CNPC[n] + 1][(curPosX_CNPC[n] + 2) / 2] == 2)  // 오른쪽아래에 오염된 땅이 있을때
	{
		SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
		gameBoardInfo[curPosY_CNPC[n]][curPosX_CNPC[n] / 2] = 3; // gameBoardInfo에 생성된 바이러스 정보 저장
		SetConsoleTextAttribute(hC, 8);
		printf("○");
		SetConsoleTextAttribute(hC, 15);
		SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
	}

	else
	{
		SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
		printf("  ");
		SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
	}
}

void MoveUp_CNPC(int n)
{
	if (DetectCollision(curPosX_CNPC[n], curPosY_CNPC[n] - 1) != 0)
		return;
	Delete_CNPC(n);
	curPosY_CNPC[n] -= 1;
	SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
	Show_CNPC(n);
}

void MoveDown_CNPC(int n)
{
	if (DetectCollision(curPosX_CNPC[n], curPosY_CNPC[n] + 1) != 0)
		return;
	Delete_CNPC(n);
	curPosY_CNPC[n] += 1;
	SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
	Show_CNPC(n);
}

void MoveLeft_CNPC(int n)
{
	if (DetectCollision(curPosX_CNPC[n] - 2, curPosY_CNPC[n]) != 0)
		return;
	Delete_CNPC(n);
	curPosX_CNPC[n] -= 2;
	SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
	Show_CNPC(n);
}

void MoveRight_CNPC(int n)
{
	if (DetectCollision(curPosX_CNPC[n] + 2, curPosY_CNPC[n]) != 0)
		return;
	Delete_CNPC(n);
	curPosX_CNPC[n] += 2;
	SetCurrentCursorPos(curPosX_CNPC[n], curPosY_CNPC[n]);
	Show_CNPC(n);
}

void Move_CNPC(int n)
{
	int move = rand() % 4;

	switch (move)
	{
	case 0:
		MoveLeft_CNPC(n);
		break;
	case 1:
		MoveRight_CNPC(n);
		break;
	case 2:
		MoveUp_CNPC(n);
		break;
	case 3:
		MoveDown_CNPC(n);
		break;
	}
}

// --------------------------------------------------------------------------------------

void Show_DNPC(int n)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentCursorPos(curPosX_DNPC[n], curPosY_DNPC[n]);
	SetConsoleTextAttribute(hC, 12);
	printf("◈");
	SetConsoleTextAttribute(hC, 15);
	SetCurrentCursorPos(curPosX_DNPC[n], curPosY_DNPC[n]);
}

void Delete_DNPC(int n)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);

	SetCurrentCursorPos(curPosX_DNPC[n], curPosY_DNPC[n]);
	if (gameBoardInfo[curPosY_DNPC[n]][curPosX_DNPC[n] / 2] == 7)
		gameBoardInfo[curPosY_DNPC[n]][curPosX_DNPC[n] / 2] = 0;

	if (gameBoardInfo[curPosY_DNPC[n]][curPosX_DNPC[n] / 2] == 3)
	{
		SetConsoleTextAttribute(hC, 8);
		printf("○");
		SetConsoleTextAttribute(hC, 15);
	}

	else
		printf(" ");
	SetCurrentCursorPos(curPosX_DNPC[n], curPosY_DNPC[n]);
}

void MoveUp_DNPC(int n)
{
	if (DetectCollision(curPosX_DNPC[n], curPosY_DNPC[n] - 1) != 0)
		return;
	Delete_DNPC(n);
	curPosY_DNPC[n] -= 1;
	SetCurrentCursorPos(curPosX_DNPC[n], curPosY_DNPC[n]);
	Show_DNPC(n);
}

void MoveDown_DNPC(int n)
{
	if (DetectCollision(curPosX_DNPC[n], curPosY_DNPC[n] + 1) != 0)
		return;
	Delete_DNPC(n);
	curPosY_DNPC[n] += 1;
	SetCurrentCursorPos(curPosX_DNPC[n], curPosY_DNPC[n]);
	Show_DNPC(n);
}

void MoveLeft_DNPC(int n)
{
	if (DetectCollision(curPosX_DNPC[n] - 2, curPosY_DNPC[n]) != 0)
		return;
	Delete_DNPC(n);
	curPosX_DNPC[n] -= 2;
	SetCurrentCursorPos(curPosX_DNPC[n], curPosY_DNPC[n]);
	Show_DNPC(n);
}

void MoveRight_DNPC(int n)
{
	if (DetectCollision(curPosX_DNPC[n] + 2, curPosY_DNPC[n]) != 0)
		return;
	Delete_DNPC(n);
	curPosX_DNPC[n] += 2;
	SetCurrentCursorPos(curPosX_DNPC[n], curPosY_DNPC[n]);
	Show_DNPC(n);
}

void Move_DNPC(int n)
{
	SetCurrentCursorPos(curPosX_DNPC[n], curPosY_DNPC[n]);

	int dx = curPosX_DNPC[n] - curPosX;
	int dy = curPosY_DNPC[n] - curPosY;

	if (dx > 0 && dy > 0) // goLeftUp
	{
		MoveUp_DNPC(n);
		MoveLeft_DNPC(n);
	}
	else if (dx == 0 && dy > 0)
		MoveUp_DNPC(n);
	else if (dx > 0 && dy == 0)
		MoveLeft_DNPC(n);


	else if (dx < 0 && dy < 0) // goRightDOWN
	{
		MoveDown_DNPC(n);
		MoveRight_DNPC(n);
	}
	else if (dx == 0 && dy < 0)
		MoveDown_DNPC(n);
	else if (dx > 0 && dy == 0)
		MoveRight_DNPC(n);

	else if (dx > 0 && dy < 0) // goLeftDown
	{
		MoveDown_DNPC(n);
		MoveLeft_DNPC(n);
	}
	else if (dx == 0 && dy < 0)
		MoveDown_DNPC(n);
	else if (dx > 0 && dy == 0)
		MoveLeft_DNPC(n);

	else if (dx < 0 && dy > 0) // goRightUp
	{
		MoveRight_DNPC(n);
		MoveUp_DNPC(n);
	}
	else if (dx == 0 && dy > 0)
		MoveUp_DNPC(n);
	else if (dx < 0 && dy == 0)
		MoveRight_DNPC(n);
}

// --------------------------------------------------------------------------------------

void gun()
{
	num_gun++;
}
void bomb()
{
	num_bomb++;
}
void num_jj()
{
	jj++;
}

void Show_GUN()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	SetConsoleTextAttribute(hC, 11);
	printf("*");
	SetConsoleTextAttribute(hC, 15);
	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
}

void Delete_GUN()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);

	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	if (gameBoardInfo[curPosY_GUN][curPosX_GUN / 2] == 3)
	{
		SetConsoleTextAttribute(hC, 8);
		printf("○");
		SetConsoleTextAttribute(hC, 15);
	}

	else if (gameBoardInfo[curPosY_GUN][curPosX_GUN / 2] == 6)
	{
		gameBoardInfo[curPosY_GUN][curPosX_GUN / 2] = 0;
		printf(" ");
		SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	}
	else
	{
		printf(" ");
		SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	}
}

void MoveUp_GUN()
{
	if (DetectCollision_Gun(curPosX_GUN, curPosY_GUN - 1) == 1)
		return;

	Delete_GUN();
	ShowPC();
	curPosY_GUN -= 1;
	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	Show_GUN();
}

void MoveDown_GUN()
{
	if (DetectCollision_Gun(curPosX_GUN, curPosY_GUN + 1) == 1)
		return;

	Delete_GUN();
	ShowPC();
	curPosY_GUN += 1;
	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	Show_GUN();
}

void MoveLeft_GUN()
{
	if (DetectCollision_Gun(curPosX_GUN - 2, curPosY_GUN) == 1)
		return;

	Delete_GUN();
	ShowPC();
	curPosX_GUN -= 2;
	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	Show_GUN();
}

void MoveRight_GUN()
{
	if (DetectCollision_Gun(curPosX_GUN + 2, curPosY_GUN) == 1)
		return;

	Delete_GUN();
	ShowPC();
	curPosX_GUN += 2;
	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	Show_GUN();
}

void Gun_Up()
{
	curPosX_GUN = curPosX;
	curPosY_GUN = curPosY;

	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);

	for (int i = 0; i < GBOARD_HEIGHT - 5; i++)
	{
		MoveUp_GUN();
		Sleep(30);
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_ANPC[i] && curPosY_GUN == curPosY_ANPC[i])
			{
				curPosX_ANPC[i] = 2;
				curPosY_ANPC[i] = 20;
				SetCurrentCursorPos(curPosX_ANPC[i], curPosY_ANPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_BNPC[i] && curPosY_GUN == curPosY_BNPC[i])
			{
				curPosX_BNPC[i] = 2;
				curPosY_BNPC[i] = 20;
				SetCurrentCursorPos(curPosX_BNPC[i], curPosY_BNPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_CNPC[i] && curPosY_GUN == curPosY_CNPC[i])
			{
				curPosX_CNPC[i] = 2;
				curPosY_CNPC[i] = 20;
				SetCurrentCursorPos(curPosX_CNPC[i], curPosY_CNPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_DNPC[i] && curPosY_GUN == curPosY_DNPC[i])
			{
				curPosX_DNPC[i] = 2;
				curPosY_DNPC[i] = 20;
				SetCurrentCursorPos(curPosX_DNPC[i], curPosY_DNPC[i]);
			}
		}
	}
	Delete_GUN();
	ShowPC();
}

void Gun_Left()
{
	curPosX_GUN = curPosX;
	curPosY_GUN = curPosY;

	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	for (int i = 0; i < GBOARD_WIDTH / 2; i++)
	{
		MoveLeft_GUN();
		Sleep(30);
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_ANPC[i] && curPosY_GUN == curPosY_ANPC[i])
			{
				curPosX_ANPC[i] = 2;
				curPosY_ANPC[i] = 20;
				SetCurrentCursorPos(curPosX_ANPC[i], curPosY_ANPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_BNPC[i] && curPosY_GUN == curPosY_BNPC[i])
			{
				curPosX_BNPC[i] = 2;
				curPosY_BNPC[i] = 20;
				SetCurrentCursorPos(curPosX_BNPC[i], curPosY_BNPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_CNPC[i] && curPosY_GUN == curPosY_CNPC[i])
			{
				curPosX_CNPC[i] = 2;
				curPosY_CNPC[i] = 20;
				SetCurrentCursorPos(curPosX_CNPC[i], curPosY_CNPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_DNPC[i] && curPosY_GUN == curPosY_DNPC[i])
			{
				curPosX_DNPC[i] = 2;
				curPosY_DNPC[i] = 20;
				SetCurrentCursorPos(curPosX_DNPC[i], curPosY_DNPC[i]);
			}
		}
	}
	Delete_GUN();
	ShowPC();
}

void Gun_Down()
{
	curPosX_GUN = curPosX;
	curPosY_GUN = curPosY;

	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);
	for (int i = 0; i < GBOARD_HEIGHT - 5; i++)
	{
		MoveDown_GUN();
		Sleep(30);
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_ANPC[i] && curPosY_GUN == curPosY_ANPC[i])
			{
				curPosX_ANPC[i] = 2;
				curPosY_ANPC[i] = 20;
				SetCurrentCursorPos(curPosX_ANPC[i], curPosY_ANPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_BNPC[i] && curPosY_GUN == curPosY_BNPC[i])
			{
				curPosX_BNPC[i] = 2;
				curPosY_BNPC[i] = 20;
				SetCurrentCursorPos(curPosX_BNPC[i], curPosY_BNPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_CNPC[i] && curPosY_GUN == curPosY_CNPC[i])
			{
				curPosX_CNPC[i] = 2;
				curPosY_CNPC[i] = 20;
				SetCurrentCursorPos(curPosX_CNPC[i], curPosY_CNPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_DNPC[i] && curPosY_GUN == curPosY_DNPC[i])
			{
				curPosX_DNPC[i] = 2;
				curPosY_DNPC[i] = 20;
				SetCurrentCursorPos(curPosX_DNPC[i], curPosY_DNPC[i]);
			}
		}
	}
	Delete_GUN();
	ShowPC();
}

void Gun_Right()
{
	curPosX_GUN = curPosX;
	curPosY_GUN = curPosY;

	SetCurrentCursorPos(curPosX_GUN, curPosY_GUN);

	for (int i = 0; i < GBOARD_WIDTH / 2; i++)
	{
		MoveRight_GUN();
		Sleep(20);

		for (int i = 0; i < 5; i++)
			if (curPosX_GUN == curPosX_ANPC[i] && curPosY_GUN == curPosY_ANPC[i])
			{
				curPosX_ANPC[i] = 2;
				curPosY_ANPC[i] = 20;
				SetCurrentCursorPos(curPosX_ANPC[i], curPosY_ANPC[i]);
			}

		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_BNPC[i] && curPosY_GUN == curPosY_BNPC[i])
			{
				curPosX_BNPC[i] = 2;
				curPosY_BNPC[i] = 20;
				SetCurrentCursorPos(curPosX_BNPC[i], curPosY_BNPC[i]);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (curPosX_GUN == curPosX_CNPC[i] && curPosY_GUN == curPosY_CNPC[i])
			{
				curPosX_CNPC[i] = 2;
				curPosY_CNPC[i] = 20;
				SetCurrentCursorPos(curPosX_CNPC[i], curPosY_CNPC[i]);
			}
		}
		for (int i = 0; i < 3; i++)
		{
			if (curPosX_GUN == curPosX_DNPC[i] && curPosY_GUN == curPosY_DNPC[i])
			{
				curPosX_DNPC[i] = 2;
				curPosY_DNPC[i] = 20;
				SetCurrentCursorPos(curPosX_DNPC[i], curPosY_DNPC[i]);
			}
		}
	}
	Delete_GUN();
	ShowPC();
}

// --------------------------------------------------------------------------------------


void PurifyBlock()
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	int i, j;

	int y7 = rand() % GBOARD_HEIGHT;
	int x7 = rand() % GBOARD_WIDTH;
	if ((gameBoardInfo[y7][x7] == 0) && cnt7 == 0) {
		gameBoardInfo[y7][x7] = 7;
		cnt7 = 1;
	}

	for (j = 0; j < GBOARD_HEIGHT; j++)
	{
		for (i = 0; i < GBOARD_WIDTH; i++)
		{
			if (gameBoardInfo[j][i] == 4)
			{
				SetCurrentCursorPos(i * 2, j);
				SetConsoleTextAttribute(hC, 10);
				printf("■");
				SetConsoleTextAttribute(hC, 15);
			}

			if (gameBoardInfo[j][i] == 7)
			{
				SetCurrentCursorPos(i * 2, j);
				SetConsoleTextAttribute(hC, 3);
				printf("★");
				SetConsoleTextAttribute(hC, 15);
			}
		}
	}
}

void PurifyTest()
{
	int i, j, k, cnt = 0;
	int x1, x2, y1, y2;

	for (i = 0; i < 30; i++)
	{
		cnt = 0;
		y1 = block[stage][i][0];
		y2 = block[stage][i][1];
		x1 = block[stage][i][2];
		x2 = block[stage][i][3];

		for (k = x1; k <= x2; k++)
		{
			for (j = y1; j <= y2; j++)
			{
				if (gameBoardInfo[k][j] == 3)
					cnt++;
			}
		}

		if (cnt == 0)
		{
			for (k = x1 + 1; k < x2; k++)
			{
				for (j = y1 + 1; j < y2; j++)
				{
					if (gameBoardInfo[k][j] == 2)
					{
						if (k == x2 - 1 && j == y2 - 1) // for문의 마지막에서 콤보 발동
						{
							cnt7 = 0;
							comboEnd_t = clock();

							if ((comboEnd_t - comboStart_t) / 1000 <= 5)
								combo++;
							else
								combo = 0;

							comboStart_t = clock();
						}

						gameBoardInfo[k][j] = 4;
						PurifyBlock();
						score += 20 + combo * 10;
					}
				}
			}
		}
	}
}

// --------------------------------------------------------------------------------------

void PC_Item_Collision();

void MakeBomb()
{
	SetCurrentCursorPos(curPosX, curPosY);
	gameBoardInfo[curPosY][curPosX / 2] = 5;

	for (int a = 0;a < 3;a++)
		for (int b = 0;b < 3;b++)
		{
			if (gameBoardInfo[curPosY - 1 + a][curPosX / 2 - 1 + b] == 6)
				gameBoardInfo[curPosY - 1 + a][curPosX / 2 - 1 + b] = 0;
			else
				break;
		}

}
void ProcessGunKeyInput()
{
	int key;
	for (int i = 0; i < 20; i++)
	{
		if (_kbhit() != 0)
		{
			key = _getch();

			if (keychange % 2 == 0)
			{
				switch (key)
				{
				case w:
					if (num_bomb <= 0) break;
					MakeBomb();
					num_bomb--;
					break;

				case q:
					if (jj == 1)
						break;
					num_jj();
					ShowPC();
					break;

				case UP:
					if (jj == 0)
					{
						MoveUp_PC();
						PC_NPC_Collision();
						PC_Item_Collision();
						PC_Bomb_Collision();
						break;
					}
					Gun_Up();
					num_gun--;
					jj--;
					ShowPC();
					break;

				case LEFT:
					if (jj == 0)
					{
						MoveLeft_PC();
						PC_NPC_Collision();
						PC_Item_Collision();
						PC_Bomb_Collision();
						break;
					}
					Gun_Left();
					num_gun--;
					jj--;
					ShowPC();
					break;
				case DOWN:
					if (jj == 0)
					{
						MoveDown_PC();
						PC_NPC_Collision();
						PC_Item_Collision();
						PC_Bomb_Collision();
						break;
					}
					Gun_Down();
					num_gun--;
					jj--;
					ShowPC();
					break;
				case RIGHT:
					if (jj == 0)
					{
						MoveRight_PC();
						PC_NPC_Collision();
						PC_Item_Collision();
						PC_Bomb_Collision();
						break;
					}
					Gun_Right();
					num_gun--;
					jj--;
					ShowPC();
					break;

				}
			}
			if (keychange % 2 == 1)
			{
				switch (key)
				{
				case w:
					if (num_bomb <= 0) break;
					MakeBomb();
					num_bomb--;
					break;

				case q:
					if (jj == 1)
						break;
					num_jj();
					ShowPC();
					break;

				case UP:
					if (jj == 0)
					{
						MoveDown_PC();
						PC_NPC_Collision();
						PC_Item_Collision();
						PC_Bomb_Collision();
						break;
					}
					Gun_Up();
					num_gun--;
					jj--;
					ShowPC();
					break;

				case LEFT:
					if (jj == 0)
					{
						MoveRight_PC();
						PC_NPC_Collision();
						PC_Item_Collision();
						PC_Bomb_Collision();
						break;
					}
					Gun_Left();
					num_gun--;
					jj--;
					ShowPC();
					break;
				case DOWN:
					if (jj == 0)
					{
						MoveUp_PC();
						PC_NPC_Collision();
						PC_Item_Collision();
						PC_Bomb_Collision();
						break;
					}
					Gun_Down();
					num_gun--;
					jj--;
					ShowPC();
					break;
				case RIGHT:
					if (jj == 0)
					{
						MoveLeft_PC();
						PC_NPC_Collision();
						PC_Item_Collision();
						PC_Bomb_Collision();
						break;
					}
					Gun_Right();
					num_gun--;
					jj--;
					ShowPC();
					break;

				}
			}
		}
	}
}
void ProcessKeyInput()
{
	int key, i;

	for (i = 0; i < 20; i++)
	{
		if (!num_gun == 0)
			ProcessGunKeyInput();

		if (_kbhit() != 0)
		{
			key = _getch();

			if (keychange % 2 == 0) {
				switch (key)
				{

				case LEFT:
					MoveLeft_PC();
					PC_NPC_Collision();
					PC_Item_Collision();
					PC_Bomb_Collision();
					break;
				case RIGHT:
					MoveRight_PC();
					PC_NPC_Collision();
					PC_Item_Collision();
					PC_Bomb_Collision();
					break;
				case UP:
					MoveUp_PC();
					PC_NPC_Collision();
					PC_Item_Collision();
					PC_Bomb_Collision();
					break;
				case DOWN:
					MoveDown_PC();
					PC_NPC_Collision();
					PC_Item_Collision();
					PC_Bomb_Collision();
					break;
				case w:
					if (num_bomb <= 0) break;
					MakeBomb();
					num_bomb--;
					break;
				}
			}
			if (keychange % 2 == 1) {
				switch (key)
				{
				case LEFT:
					MoveRight_PC();
					PC_NPC_Collision();
					PC_Item_Collision();
					PC_Bomb_Collision();
					break;
				case RIGHT:
					MoveLeft_PC();
					PC_NPC_Collision();
					PC_Item_Collision();
					PC_Bomb_Collision();
					break;
				case UP:
					MoveDown_PC();
					PC_NPC_Collision();
					PC_Item_Collision();
					break;
				case DOWN:
					MoveUp_PC();
					PC_NPC_Collision();
					PC_Item_Collision();
					break;
				case w:
					if (num_bomb <= 0) break;
					MakeBomb();
					num_bomb--;
					break;
				}
			}
		}

		if (gameover == 0)
			UI_TimeLimit();

		if (gameover == 1)
			break;

		if (life == 0)
		{
			gameover = 1;
			break;
		}

		Sleep(speed);
		PurifyTest();
		StageClearTest();
	}

	// 아이템 먹으면 3초간 메세지 띄우기
	textE_t = clock();
	if ((textE_t - textS_t) / 1000 >= 3)
		DeleteMessage1();

	// 5초간 좀비 증식 경고문 띄우기
	intextE_t = clock();
	if ((intextE_t - intextS_t) / 1000 >= 5)
		DeleteMessage2();

	// 키반전 유효시간
	keyE_t = clock();
	if ((keyE_t - keyS_t) / 1000 >= 5)
	{
		keychange = 0;
	}
}

// --------------------------------------------------------------------------------------

void What_Item()
{
	srand((unsigned)time(NULL));
	int item = rand() % 9;

	textS_t = clock();
	keyS_t = clock();

	switch (item)
	{
		// 시간 증가 아이템
	case 0:
		stagetime = stagetime + 10;
		SetCurrentCursorPos(20, 26);
		printf("시간이 10초 증가되었다!       ");
		break;

		// 목숨 아이템
	case 1:
		life++;
		SetCurrentCursorPos(20, 26);
		printf("목숨이 늘어났다!              ");
		break;

		// 방향키 전환 아이템
	case 2: case 3: case 4:
		keychange = 1;
		SetCurrentCursorPos(20, 26);
		printf("5초간 방향키가 전환되었다!    ");
		break;

		// 총알 아이템
	case 5: case 6: case 7:
		gun();
		SetCurrentCursorPos(20, 26);
		printf("총알을 획득했다!               ");
		break;

		// 폭탄 아이템
	case 8: case 9:
		bomb();
		SetCurrentCursorPos(20, 26);
		printf("폭탄을 획득했다!              ");
		break;
	}
}

void PC_Item_Collision()
{
	int i, j;

	for (j = 0; j < GBOARD_HEIGHT; j++) {
		for (i = 0; i < GBOARD_WIDTH; i++) {
			if (gameBoardInfo[j][i] == 7) {
				if (curPosX == i * 2 && curPosY == j) {
					gameBoardInfo[j][i] = 0;
					What_Item();
				}
			}
		}
	}
}

void IncreaseNPC(int num_BNPC, int num_CNPC, int num_DNPC)
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);

	now_t = clock();
	int consume_t = (now_t - old_t) / 1000;
	consume_t = stagetime - consume_t;
	// 해당 스테이지의 현재시간이 consume_t

	if (consume_t == increasetime)
		in = 0;

	if (consume_t == increasetime + 5)
	{
		intextS_t = clock();
		SetCurrentCursorPos(27, 24);
		SetConsoleTextAttribute(hC, 12);
		printf("< W A R N I N G >");
		SetConsoleTextAttribute(hC, 15);
		SetCurrentCursorPos(22, 25);
		printf("5초 후에 좀비가 증식합니다");
	}

	if (consume_t < increasetime)
	{
		if (in == 0)
		{
			for (int b = num_BNPC; b < num_BNPC + 1; b++)
			{
				while (1)
				{
					curPosX_BNPC[b] = rand() % GBOARD_WIDTH * 2;
					curPosY_BNPC[b] = rand() % GBOARD_HEIGHT;
					if (stagemap[stage][curPosY_BNPC[b]][curPosX_BNPC[b] / 2] == 0)
						break;
				}
				SetCurrentCursorPos(curPosX_BNPC[b], curPosY_BNPC[b]);
				Show_BNPC(b);
			}
			for (int c = num_CNPC; c < num_CNPC + 2; c++)
			{
				while (1)
				{
					curPosX_CNPC[c] = rand() % GBOARD_WIDTH * 2;
					curPosY_CNPC[c] = rand() % GBOARD_HEIGHT;
					if (stagemap[stage][curPosY_CNPC[c]][curPosX_CNPC[c] / 2] == 0)
						break;
				}
				SetCurrentCursorPos(curPosX_CNPC[c], curPosY_CNPC[c]);
				Show_CNPC(c);
			}
			for (int d = num_DNPC; d < num_DNPC + 4; d++)
			{
				while (1)
				{
					curPosX_DNPC[d] = rand() % GBOARD_WIDTH * 2;
					curPosY_DNPC[d] = rand() % GBOARD_HEIGHT;
					if (stagemap[stage][curPosY_DNPC[d]][curPosX_DNPC[d] / 2] == 0)
						break;
				}
				SetCurrentCursorPos(curPosX_DNPC[d], curPosY_DNPC[d]);
				Show_DNPC(d);
			}
			in = 1;
		}
		for (int b = num_BNPC; b < num_BNPC + 1; b++)
			Move_BNPC(b);
		for (int c = num_CNPC; c < num_CNPC + 2; c++)
			Move_CNPC(c);
		for (int d = num_DNPC; d < num_DNPC + 4; d++)
			Move_DNPC(d);
	}

	if (consume_t < increasetime - 2)
	{
		if (in == 1)
		{
			for (int b = num_BNPC + 1; b < num_BNPC + 2; b++)
			{
				while (1)
				{
					curPosX_BNPC[b] = rand() % GBOARD_WIDTH * 2;
					curPosY_BNPC[b] = rand() % GBOARD_HEIGHT;
					if (stagemap[stage][curPosY_BNPC[b]][curPosX_BNPC[b] / 2] == 0)
						break;
				}
				SetCurrentCursorPos(curPosX_BNPC[b], curPosY_BNPC[b]);
				Show_BNPC(b);
			}
			for (int c = num_CNPC + 2; c < num_CNPC + 4; c++)
			{
				while (1)
				{
					curPosX_CNPC[c] = rand() % GBOARD_WIDTH * 2;
					curPosY_CNPC[c] = rand() % GBOARD_HEIGHT;
					if (stagemap[stage][curPosY_CNPC[c]][curPosX_CNPC[c] / 2] == 0)
						break;
				}
				SetCurrentCursorPos(curPosX_CNPC[c], curPosY_CNPC[c]);
				Show_CNPC(c);
			}
			in = 2;
		}
		for (int b = num_BNPC + 1; b < num_BNPC + 2; b++)
			Move_BNPC(b);
		for (int c = num_CNPC + 2; c < num_CNPC + 4; c++)
			Move_CNPC(c);
	}

	if (consume_t < increasetime - 4)
	{
		if (in == 2)
		{
			for (int b = num_BNPC + 2; b < num_BNPC + 3; b++)
			{
				while (1)
				{
					curPosX_BNPC[b] = rand() % GBOARD_WIDTH * 2;
					curPosY_BNPC[b] = rand() % GBOARD_HEIGHT;
					if (stagemap[stage][curPosY_BNPC[b]][curPosX_BNPC[b] / 2] == 0)
						break;
				}
				SetCurrentCursorPos(curPosX_BNPC[b], curPosY_BNPC[b]);
				Show_BNPC(b);
			}
			for (int c = num_CNPC + 4; c < num_CNPC + 6; c++)
			{
				while (1)
				{
					curPosX_CNPC[c] = rand() % GBOARD_WIDTH * 2;
					curPosY_CNPC[c] = rand() % GBOARD_HEIGHT;
					if (stagemap[stage][curPosY_CNPC[c]][curPosX_CNPC[c] / 2] == 0)
						break;
				}
				SetCurrentCursorPos(curPosX_CNPC[c], curPosY_CNPC[c]);
				Show_CNPC(c);
			}
			in = 3;
		}
		for (int b = num_BNPC + 2; b < num_BNPC + 3; b++)
			Move_BNPC(b);
		for (int c = num_CNPC + 4; c < num_CNPC + 6; c++)
			Move_CNPC(c);
	}

	if (consume_t < increasetime - 15)
	{
		for (int b = num_BNPC; b < num_BNPC + 3; b++)
		{
			Delete_BNPC(b);
			curPosX_BNPC[b] = 2;
			curPosY_BNPC[b] = 20;
			Delete_BNPC(b);
		}
		for (int c = num_CNPC; c < num_CNPC + 6; c++)
		{
			Delete_CNPC(c);
			curPosX_CNPC[c] = 2;
			curPosY_CNPC[c] = 20;
			Delete_CNPC(c);
		}
		for (int d = num_DNPC; d < num_DNPC + 4; d++)
		{
			Delete_DNPC(d);
			curPosX_DNPC[d] = 2;
			curPosY_DNPC[d] = 20;
			Delete_DNPC(d);
		}
		increasetime -= 30;  // 좀비가 증식하는 주기(초)
	}
}

void StageSet(int num_BNPC, int num_CNPC, int num_DNPC)
{
	DrawGameBoard();
	DrawMessage();
	DrawUIBoard();

	curPosX = 22;
	curPosY = 5;
	SetCurrentCursorPos(curPosX, curPosY);
	ShowPC();

	if (stage == 1)
	{
		curPosX_BNPC[0] = 14;
		curPosY_BNPC[0] = 6;
		curPosX_CNPC[0] = 50;
		curPosY_CNPC[0] = 6;
		curPosX_CNPC[1] = 50;
		curPosY_CNPC[1] = 14;
		curPosX_CNPC[2] = 30;
		curPosY_CNPC[2] = 16;
		curPosX_DNPC[0] = 8;
		curPosY_DNPC[0] = 1;
	}

	else
	{
		for (int b = 0; b < num_BNPC; b++)
		{
			while (1) //BNPC 시작위치 지정해주는것
			{
				curPosX_BNPC[b] = rand() % GBOARD_WIDTH * 2;
				curPosY_BNPC[b] = rand() % GBOARD_HEIGHT;
				if (stagemap[stage][curPosY_BNPC[b]][curPosX_BNPC[b] / 2] == 0)
					break;
			}
			SetCurrentCursorPos(curPosX_BNPC[b], curPosY_BNPC[b]);
		}

		for (int c = 0; c < num_CNPC; c++)
		{
			while (1) //CNPC 시작위치 지정해주는것
			{
				curPosX_CNPC[c] = rand() % GBOARD_WIDTH * 2;
				curPosY_CNPC[c] = rand() % GBOARD_HEIGHT;
				if (stagemap[stage][curPosY_CNPC[c]][curPosX_CNPC[c] / 2] == 0)
					break;
			}
			SetCurrentCursorPos(curPosX_CNPC[c], curPosY_CNPC[c]);
		}

		for (int d = 0; d < num_DNPC; d++)
		{
			while (1) //CNPC 시작위치 지정해주는것
			{
				curPosX_DNPC[d] = rand() % GBOARD_WIDTH * 2;
				curPosY_DNPC[d] = rand() % GBOARD_HEIGHT;
				if (stagemap[stage][curPosY_DNPC[d]][curPosX_DNPC[d] / 2] == 0)
					break;
			}
			SetCurrentCursorPos(curPosX_DNPC[d], curPosY_DNPC[d]);
		}
	}
}

// --------------------------------------------------------------------------------------

int main()
{
	srand((unsigned)time(NULL));
	RemoveCursor();
	printstory();

	while (1) // 제일 큰 while 문
	{
		PlaySound(TEXT("ZHS Horror Infection.wav"), NULL, SND_ASYNC);
		DrawStartboard(sign);

		while (1)
		{
			PlaySound(TEXT("마이어스 추격음.wav"), NULL, SND_ASYNC | SND_LOOP);

			//  재시작시 초기화할 것들 //
			for (int i = 0; i < 10; i++) // NPC 좌표 초기화
			{
				curPosX_ANPC[i] = 0;
				curPosY_ANPC[i] = 0;
				curPosX_BNPC[i] = 0;
				curPosY_BNPC[i] = 0;
				curPosX_CNPC[i] = 0;
				curPosY_CNPC[i] = 0;
				curPosX_DNPC[i] = 0;
				curPosY_DNPC[i] = 0;
			}
			int num_BNPC = 0;
			int num_CNPC = 0;
			int num_DNPC = 0;

			old_t = clock();
			gameover = 0;
			life = 5;
			jj = 0;
			////////////////////////////////
			if (stage == 0)
			{
				stagetime = 90;
				num_bomb = 0;
				num_gun = 1;
				num_BNPC = 1;
				num_CNPC = 0;
				num_DNPC = 1;
				curPosX_ANPC[0] = 30;
				curPosY_ANPC[0] = 9;
				curPosX_ANPC[1] = 38;
				curPosY_ANPC[1] = 9;
				curPosX_ANPC[2] = 38;
				curPosY_ANPC[2] = 13;
				StageSet(num_BNPC, num_CNPC, num_DNPC);
			}

			if (stage == 1)
			{
				stagetime = 150;
				num_gun = 2;
				num_bomb = 1;
				num_BNPC = 1;
				num_CNPC = 3;
				num_DNPC = 1;
				curPosX_ANPC[0] = 36;
				curPosY_ANPC[0] = 16;
				curPosX_ANPC[1] = 54;
				curPosY_ANPC[1] = 12;
				curPosX_ANPC[2] = 26;
				curPosY_ANPC[2] = 12;
				curPosX_ANPC[3] = 22;
				curPosY_ANPC[3] = 18;
				StageSet(num_BNPC, num_CNPC, num_DNPC);
			}

			if (stage == 2)
			{
				stagetime = 180;
				increasetime = 150;
				num_gun = 3;
				num_bomb = 2;
				num_BNPC = 2;
				num_CNPC = 1;
				num_DNPC = 1;
				curPosX_ANPC[0] = 26;
				curPosY_ANPC[0] = 7;
				curPosX_ANPC[1] = 42;
				curPosY_ANPC[1] = 7;
				curPosX_ANPC[2] = 42;
				curPosY_ANPC[2] = 15;
				curPosX_ANPC[3] = 26;
				curPosY_ANPC[3] = 15;

				StageSet(num_BNPC, num_CNPC, num_DNPC);
			}

			while (1)   // 게임플레이 while 문
			{
				if (gameover == 0 && stageclear == 0 && stage < 3)  // 게임종료될때까지 돌아감
				{
					if (stage == 0)
					{
						Move_ANPC_LR(0, 30, 38);
						Move_ANPC_UD(1, 9, 13);
						Move_ANPC_RL(2, 30, 38);
					}
					if (stage == 1)
					{
						Move_ANPC_LR(0, 36, 48);
						Move_ANPC_UD(1, 12, 21);
						Move_ANPC_UD(2, 12, 21);
						Move_ANPC_RL(3, 8, 22);
						Stage2_Open();
					}
					if (stage == 2)
					{
						Move_ANPC_LR(0, 26, 42);
						Move_ANPC_UD(1, 7, 15);
						Move_ANPC_RL(2, 26, 42);
						Move_ANPC_DU(3, 7, 15);

						Portal();
						IncreaseNPC(num_BNPC, num_CNPC, num_DNPC);      // 좀비 증식
					}

					for (int b = 0; b < num_BNPC; b++) // bNPC 여러마리 소환
						Move_BNPC(b);
					for (int c = 0; c < num_CNPC; c++) // cNPC 여러마리 소환
						Move_CNPC(c);
					for (int d = 0; d < num_DNPC; d++) // dNPC 여러마리 소환
						Move_DNPC(d);

					PC_NPC_Collision();
					ProcessKeyInput();
					DrawGameUI();
					NPC_Bomb_Collision();
				}

				else {   //게임 종료되었을 때
					break;
				}
			}

			if (gameover == 1)   // 게임오버시
			{
				GameOver();
				score = 0;
				num_gun = 0;
				num_bomb = 0;
				combo = 0;
				keychange = 0;
				stage = 0;
				opentest = 0;
				break;
			}
			if (stage > 2)   // 최종 게임 클리어시
			{
				GameClear();
				stage = 0;
				score = 0;
				num_gun = 0;
				num_bomb = 0;
				combo = 0;
				keychange = 0;
				stage = 0;
				opentest = 0;
				break;
			}
			if (stageclear = 1)      // 스테이지 클리어시
			{
				stageclear = 0;
				num_gun = 0;
				num_bomb = 0;
				combo = 0;
				keychange = 0;
				opentest = 0;
				stage++;
				PrintClear();
			}
		}
	}
	getchar();
}
