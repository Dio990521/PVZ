#pragma once

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <graphics.h> // easyx graphics library
#include "tools.h"
#include <math.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

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
int sunshine;

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
	bool used;
	int timer;
	float xOffset;
	float yOffset;
};

struct Sunshine sunshinePool[10];
IMAGE sunshineSprites[29];

struct Zombie
{
	int x, y;
	int frameIndex;
	bool used;
	int speed;
	int row;
	int hp;
};
struct Zombie zombiePool[10];
IMAGE zombieSprites[22];

struct Bullet
{
	int x, y;
	int row;
	bool used;
	int speed;
	bool blast;
	int frameIndex;
};
struct Bullet bulletsPool[30];
IMAGE bulletNormalSprite;
IMAGE bulletBlast[4];


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
	sunshine = 50;

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

	// set font
	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 30;
	font.lfWeight = 15;
	strcpy(font.lfFaceName, "Segoe UI Black");
	font.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&font);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);

	// init zombies
	memset(zombiePool, 0, sizeof(zombiePool));
	for (int i = 0; i < 22; ++i)
	{
		sprintf_s(cardsDir, sizeof(cardsDir), "res/zm/0/%d.png", i + 1);
		loadimage(&zombieSprites[i], cardsDir);
	}

	loadimage(&bulletNormalSprite, "res/bullets/PeaNormal/PeaNormal_0.png");
	memset(bulletsPool, 0, sizeof(bulletsPool));

	loadimage(&bulletBlast[3], "res/bullets/PeaNormalExplode/PeaNormalExplode_0.png");
	for (int i = 0; i < 3; ++i)
	{
		float k = (i + 1) * 0.2;
		loadimage(&bulletBlast[i], "res/bullets/PeaNormalExplode/PeaNormalExplode_0.png", 
			bulletBlast[3].getwidth() * k, bulletBlast[3].getheight() * k, true);
	}

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

	// render the sunshine
	int sunshineCount = sizeof(sunshinePool) / sizeof(sunshinePool[0]);
	for (int i = 0; i < sunshineCount; ++i)
	{
		if (sunshinePool[i].used || sunshinePool[i].xOffset)
		{
			IMAGE* sunshineImg = &sunshineSprites[sunshinePool[i].frameIndex];
			putimagePNG(sunshinePool[i].x, sunshinePool[i].y, sunshineImg);
		}
	}

	char scoreText[8];
	sprintf_s(scoreText, sizeof(scoreText), "%d", sunshine);
	outtextxy(276, 67, scoreText);

	// render zombies
	int zombieCount = sizeof(zombiePool) / sizeof(zombiePool[0]);
	for (int i = 0; i < zombieCount; ++i)
	{
		if (zombiePool[i].used)
		{
			IMAGE* zombieImg = &zombieSprites[zombiePool[i].frameIndex];
			putimagePNG(zombiePool[i].x, zombiePool[i].y - zombieImg->getheight(), zombieImg);
		}
	}

	// render bullets
	int bulletCount = sizeof(bulletsPool) / sizeof(bulletsPool[0]);
	for (int i = 0; i < bulletCount; ++i)
	{
		if (!bulletsPool[i].used) continue;
		if (bulletsPool[i].blast)
		{
			IMAGE* img = &bulletBlast[bulletsPool[i].frameIndex];
			putimagePNG(bulletsPool[i].x, bulletsPool[i].y, img);
		}
		else
		{
			putimagePNG(bulletsPool[i].x, bulletsPool[i].y, &bulletNormalSprite);
		}

	}

	EndBatchDraw(); // end buffer

}

void collectSunshine(ExMessage* msg)
{
	int sunshineCount = sizeof(sunshinePool) / sizeof(sunshinePool[0]);
	int w = sunshineSprites[0].getwidth();
	int h = sunshineSprites[0].getheight();
	for (int i = 0; i < sunshineCount; ++i)
	{
		if (sunshinePool[i].used)
		{
			int x = sunshinePool[i].x;
			int y = sunshinePool[i].y;
			if (msg->x > x && msg->x < x + w &&
				msg->y > y && msg->y < y + h)
			{
				sunshinePool[i].used = false;
				mciSendString("play res/audio/sunshine.mp3", 0, 0, 0);

				// set the offset
				float destY = 0;
				float destX = 262;
				float angle = atan((y - destY) / (x - destX));
				sunshinePool[i].xOffset = 4 * cos(angle);
				sunshinePool[i].yOffset = 4 * sin(angle);
			}
		}
	}

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
			else
			{
				collectSunshine(&msg);
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
				sunshinePool[i].timer = 0;
				sunshinePool[i].xOffset = 0;
				sunshinePool[i].yOffset = 0;
				break;
			}
		}
	}

}

void updateSunshine()
{
	int sunshineCount = sizeof(sunshinePool) / sizeof(sunshinePool[0]);
	for (int i = 0; i < sunshineCount; ++i)
	{
		if (sunshinePool[i].used)
		{
			sunshinePool[i].frameIndex = (sunshinePool[i].frameIndex + 1) % 29;
			if (sunshinePool[i].timer == 0)
			{
				sunshinePool[i].y += 4;
			}
			if (sunshinePool[i].y >= sunshinePool[i].destY)
			{
				sunshinePool[i].timer++;
				if (sunshinePool[i].timer > 100)
				{
					sunshinePool[i].used = false;
				}
			}
		}
		else if (sunshinePool[i].xOffset)
		{
			float destY = 0;
			float destX = 262;
			float angle = atan((sunshinePool[i].y - destY) / (sunshinePool[i].x - destX));
			sunshinePool[i].xOffset = 4 * cos(angle);
			sunshinePool[i].yOffset = 4 * sin(angle);

			sunshinePool[i].x -= sunshinePool[i].xOffset;
			sunshinePool[i].y -= sunshinePool[i].yOffset;
			if (sunshinePool[i].y < 0 || sunshinePool[i].x < 262)
			{
				sunshinePool[i].xOffset = 0;
				sunshinePool[i].yOffset = 0;
				sunshine += 25;
			}
		}
	}
}

void createZombie()
{
	static int zombieFreq = 200;
	static int count = 0;
	++count;
	int zombieCount = sizeof(zombiePool) / sizeof(zombiePool[0]);

	if (count > zombieFreq)
	{
		count = 0;
		zombieFreq = rand() % 200 + 300;
		for (int i = 0; i < zombieCount; ++i)
		{
			if (!zombiePool[i].used)
			{
				zombiePool[i].used = true;
				zombiePool[i].speed = 1;
				zombiePool[i].hp = 100;
				zombiePool[i].row = rand() % 3;
				zombiePool[i].x = WINDOW_WIDTH;
				zombiePool[i].y = 172 + (1 + zombiePool[i].row) * 100;
				break;
			}
		}
	}

}

void updateZombie()
{
	int zombieCount = sizeof(zombiePool) / sizeof(zombiePool[0]);
	for (int i = 0; i < zombieCount; ++i)
	{
		if (zombiePool[i].used)
		{
			zombiePool[i].x -= zombiePool[i].speed;
			zombiePool[i].frameIndex = (zombiePool[i].frameIndex + 1) % 22;
			if (zombiePool[i].x < 170)
			{
				MessageBox(NULL, "over", "fuck you loser", 0);
				exit(0);
			}
		}
	}

}

void shoot()
{
	int lines[3] = { 0 };
	int zombieCount = sizeof(zombiePool) / sizeof(zombiePool[0]);
	int bulletCount = sizeof(bulletsPool) / sizeof(bulletsPool[0]);
	int dangerX = WINDOW_WIDTH - zombieSprites[0].getwidth();
	for (int i = 0; i < zombieCount; ++i)
	{
		if (zombiePool[i].used && zombiePool[i].x < dangerX)
		{
			lines[zombiePool[i].row] = 1;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (map[i][j].type == PEASHOOTER + 1 && lines[i])
			{
				static int count = 0;
				++count;
				if (count > 20)
				{
					count = 0;
					int k = 0;
					for (k = 0; k < bulletCount; ++k)
					{
						if (!bulletsPool[k].used)
						{
							bulletsPool[k].used = true;
							bulletsPool[k].row = i;
							bulletsPool[k].speed = 5;
							bulletsPool[k].blast = false;
							bulletsPool[k].frameIndex = 0;
							int plantX = 256 + j * 81;
							int plantY = 179 + i * 102 + 14;
							bulletsPool[k].x = plantX + plantSprites[map[i][j].type - 1][0]->getwidth() - 10;
							bulletsPool[k].y = plantY + 5;
							break;
						}
					}
				}
			}
		}
	}
}

void updateBullets()
{
	int bulletCount = sizeof(bulletsPool) / sizeof(bulletsPool[0]);

	for (int i = 0; i < bulletCount; ++i)
	{
		if (bulletsPool[i].used)
		{
			bulletsPool[i].x += bulletsPool[i].speed;
			if (bulletsPool[i].x > WINDOW_WIDTH)
			{
				bulletsPool[i].used = false;
			}

			if (bulletsPool[i].blast)
			{
				bulletsPool[i].frameIndex++;
				if (bulletsPool[i].frameIndex > 3)
				{
					bulletsPool[i].used = false;
				}
			}
		}
	}
}

void collisionCheck()
{
	int bulletCount = sizeof(bulletsPool) / sizeof(bulletsPool[0]);
	int zombieCount = sizeof(zombiePool) / sizeof(zombiePool[0]);
	for (int i = 0; i < bulletCount; ++i)
	{
		if (!bulletsPool[i].used || bulletsPool[i].blast) continue;
		for (int k = 0; k < zombieCount; ++k)
		{
			if (!zombiePool[k].used) continue;
			int x1 = zombiePool[k].x + 80;
			int x2 = zombiePool[k].x + 110;
			int x = bulletsPool[i].x;
			if (bulletsPool[i].row == zombiePool[k].row && x > x1 && x < x2)
			{
				zombiePool[k].hp -= 20;
				bulletsPool[i].blast = true;
				bulletsPool[i].speed = 0;
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
	updateSunshine();

	createZombie();
	updateZombie();

	shoot();
	updateBullets();

	collisionCheck();
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