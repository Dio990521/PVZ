#pragma once

#include <stdio.h>
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

void gameInit()
{
	// load the backround image
	loadimage(&bgImage, "res/map.jpg");
	loadimage(&plantBar, "res/bar.png");

	// load plant cards ui
	char cardsDir[64];
	for (int i = 0; i < PLANT_COUNT; ++i)
	{
		sprintf_s(cardsDir, sizeof(cardsDir), "res/Cards/card_%d.png", i + 1);
		loadimage(&plantCards[i], cardsDir);
	}

	// create a game window
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void updateWindow()
{
	putimage(0, 0, &bgImage);
	putimagePNG(250, 0, &plantBar);

	for (int i = 0; i < PLANT_COUNT; ++i)
	{
		int x = 338 + i * 65;
		int y = 6;
		putimage(x, y, &plantCards[i]);
	}

}

int main()
{
	gameInit();

	updateWindow();

	system("pause");
	return 0;
}