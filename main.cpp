#pragma once

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <graphics.h> // easyx graphics library
#include "tools.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

enum {
	PEASHOOTER,
	SUNFLOWER,
	PLANT_COUNT
};

IMAGE bgImage;
IMAGE plantBar;
IMAGE plantCards[PLANT_COUNT];
IMAGE* plantSprites[PLANT_COUNT][20];

int curX, curY; // cursor pos after clicking the plant card
int selectedPlant; // 0: no selection

struct Plant {
	int type = 0; // 0: no plant
	int frameIndex = 0;
};

struct Plant map[3][9];

struct Sunshine
{
	int x, y;
	int frameIndex;
	int destY;
	int used;
};

struct Sunshine sunshinePool[10];
IMAGE sunshineSprites[29];

bool fileExist(const char* name)
{
	FILE* fp = fopen(name, "r");
	if (fp)
	{
		fclose(fp);
	}
	return fp != NULL;
}

void gameInit()
{
	// load the backround image
	loadimage(&bgImage, "res/map.jpg");
	loadimage(&plantBar, "res/bar.png");

	memset(plantSprites, 0, sizeof(plantSprites));
	//memset(sunshineSprites, 0, sizeof(sunshineSprites));
	memset(map, 0, sizeof(map));

	// load plant cards ui
	char cardsDir[64];
	for (int i = 0; i < PLANT_COUNT; ++i)
	{
		sprintf_s(cardsDir, sizeof(cardsDir), "res/Cards/card_%d.png", i + 1);
		loadimage(&plantCards[i], cardsDir);

		for (int j = 0; j < 20; ++j)
		{
			sprintf_s(cardsDir, sizeof(cardsDir), "res/Plants/%d/%d.png", i, j + 1);
			if (fileExist(cardsDir))
			{
				plantSprites[i][j] = new IMAGE;
				loadimage(plantSprites[i][j], cardsDir);
			}
			else
			{
				break;
			}
		}
	}

	selectedPlant = 0;
	memset(sunshinePool, 0, sizeof(sunshinePool));
	for (int i = 0; i < 29; ++i)
	{
		sprintf_s(cardsDir, sizeof(cardsDir), "res/sunshine/%d.png", i + 1);
		loadimage(&sunshineSprites[i], cardsDir);
	}

	// set a random seed
	srand(time(NULL));

	// create a game window
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, 1);
}

void updateWindow()
{
	BeginBatchDraw(); // buffer

	putimage(0, 0, &bgImage);
	putimagePNG(250, 0, &plantBar);

	for (int i = 0; i < PLANT_COUNT; ++i)
	{
		int x = 338 + i * 65;
		int y = 6;
		putimage(x, y, &plantCards[i]);
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (map[i][j].type > 0)
			{
				int x = 256 + j * 81;
				int y = 179 + i * 102;
				int plantType = map[i][j].type - 1;
				int curFrame = map[i][j].frameIndex;
				putimagePNG(x, y, plantSprites[plantType][curFrame]);
			}
		}
	}

	// render the plant
	if (selectedPlant > 0)
	{
		IMAGE* img = plantSprites[selectedPlant - 1][0];
		putimagePNG(curX - img->getwidth() / 2, curY - img->getheight() / 2, img);
	}

	EndBatchDraw(); // end buffer

}

void userClick()
{
	ExMessage msg;
	static int state = 0;
	if (peekmessage(&msg))
	{
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x > 338 && msg.x < 338 + 65 * PLANT_COUNT && msg.y < 96)
			{
				int index = (msg.x - 338) / 65;
				state = 1;
				selectedPlant = index + 1;
				std::cout << index << std::endl;
			}
		}
		else if (msg.message == WM_MOUSEMOVE && state == 1)
		{
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP)
		{
			if (msg.x > 256 && msg.y > 179 && msg.y < 489)
			{
				int row = (msg.y - 179) / 102;
				int col = (msg.x - 256) / 81;
				std::cout << row << col << std::endl;

				if (map[row][col].type == 0)
				{
					map[row][col].type = selectedPlant;
					map[row][col].frameIndex = 0;
				}
					
			}

			selectedPlant = 0;
			state = 0;
		}
	}
}

void createSunshine()
{
	static int count = 0;
	static int freq = 400;
	++count;
	if (count >= freq)
	{
		freq = 200 + rand() % 200;
		count = 0;
		int sunshineCount = sizeof(sunshinePool) / sizeof(sunshinePool[0]);

		for (int i = 0; i < sunshineCount; ++i)
		{
			if (!sunshinePool[i].used)
			{
				sunshinePool[i].used = true;
				sunshinePool[i].frameIndex = 0;
				sunshinePool[i].x = 260 + rand() % (900 - 260); // 260 ~ 900
				sunshinePool[i].y = 60;
				sunshinePool[i].destY = 200 + (rand() % 4) * 90;
				break;
			}
		}
	}

}

void renderAll()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (map[i][j].type > 0)
			{
				map[i][j].frameIndex++;
				int plantType = map[i][j].type - 1;
				int index = map[i][j].frameIndex;
				if (plantSprites[plantType][index] == NULL)
				{
					map[i][j].frameIndex = 0;
				}
			}
		}
	}

	createSunshine();
} 



void startMenu()
{
	IMAGE startBG, startGame1, startGame2;
	loadimage(&startBG, "res/menu.png");
	loadimage(&startGame1, "res/menu1.png");
	loadimage(&startGame2, "res/menu2.png");

	int flag = 0;

	while (true)
	{
		BeginBatchDraw();
		putimage(0, 0, &startBG);
		putimagePNG(474, 75, flag ? &startGame2 : &startGame1);

		ExMessage msg;
		if (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN && msg.x > 474 && msg.x < 474 + 300 && msg.y > 75 && msg.y < 75 + 140)
			{
				flag = 1;
			}
			else if (msg.message == WM_LBUTTONUP && flag)
			{
				return;
			}
		}

		EndBatchDraw();
	}
	
}

int main()
{
	gameInit();

	startMenu();

	int timer = 0;
	bool flag = true;
	while (true)
	{
		userClick();
		timer += getDelay();
		if (timer > 50)
		{
			flag = true;
			timer = 0;
		}

		if (flag)
		{
			flag = false;
			updateWindow();
			renderAll();
		}

	}

	system("pause");
	return 0;
}