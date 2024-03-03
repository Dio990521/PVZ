#pragma once

#include <stdio.h>
#include <iostream>
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

		}
	}
}

int main()
{
	gameInit();

	while (true)
	{
		userClick();

		updateWindow();
	}

	system("pause");
	return 0;
}