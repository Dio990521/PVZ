#pragma once

#include "AssetManager.h"
#include "AudioManager.h"
#include "Singleton.h"
#include "GameEngine.h"
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <graphics.h> // easyx graphics library
#include "tools.h"
#include "vector2.h"
#include <math.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600
#define ZOMBIE_MAX 10

enum {
	PEASHOOTER,
	SUNFLOWER,
	PLANT_COUNT
};

IMAGE bgImage;
IMAGE plantBar;
IMAGE plantCards[PLANT_COUNT];
IMAGE* plantSprites[PLANT_COUNT][20];
IMAGE zombieStand[11];

int curX, curY; // cursor pos after clicking the plant card
int selectedPlant; // 0: no selection
int sunshine;


struct Plant {
	int type = 0; // 0: no plant
	int frameIndex = 0;
	bool catched;
	int hp;
	int timer;
	int x, y;
	int shootTime;
};

struct Plant map[3][9];

enum {SUNSHINE_DOWN, SUNSHINE_GROUND, SUNSHINE_COLLECT, SUNSHINE_PRODUCT};

enum {RUNNING, WIN, FAIL};
int killCount;
int createdZombieCount;
int gameStatus;

struct Sunshine
{
	int x, y;
	int frameIndex;
	int destY;
	bool used;
	int timer;
	float xOffset;
	float yOffset;

	float t; // 贝塞尔时间点
	vector2 p1, p2, p3, p4;
	vector2 curPos;
	float speed;
	int status;
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
	bool isDead;
	bool isEating;
};
struct Zombie zombiePool[10];
IMAGE zombieSprites[22];
IMAGE zombieDieSprites[10];
IMAGE zombieEatSprites[21];

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

	killCount = 0;
	createdZombieCount = 0;
	gameStatus = RUNNING;

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

	for (int i = 0; i < 10; ++i)
	{
		sprintf_s(cardsDir, sizeof(cardsDir), "res/zm_dead/%d.png", i + 1);
		loadimage(&zombieDieSprites[i], cardsDir);
	}

	for (int i = 0; i < 21; ++i)
	{
		sprintf_s(cardsDir, sizeof(cardsDir), "res/zm_eat/0/%d.png", i + 1);
		loadimage(&zombieEatSprites[i], cardsDir);
	}

	//for (int i = 0; i < 11; ++i)
	//{
	//	sprintf_s(cardsDir, sizeof(cardsDir), "res/zm_eat/0/%d.png", i + 1);
	//}

}

void drawSunshine()
{
	// render the sunshine
	int sunshineCount = sizeof(sunshinePool) / sizeof(sunshinePool[0]);
	for (int i = 0; i < sunshineCount; ++i)
	{
		if (sunshinePool[i].used)// || sunshinePool[i].xOffset)
		{
			IMAGE* sunshineImg = &sunshineSprites[sunshinePool[i].frameIndex];
			//putimagePNG(sunshinePool[i].x, sunshinePool[i].y, sunshineImg);
			putimagePNG(sunshinePool[i].curPos.x, sunshinePool[i].curPos.y, sunshineImg);
		}
	}

	char scoreText[8];
	sprintf_s(scoreText, sizeof(scoreText), "%d", sunshine);
	outtextxy(276, 67, scoreText);
}

void drawCards()
{
	for (int i = 0; i < PLANT_COUNT; ++i)
	{
		int x = 338 + i * 65;
		int y = 6;
		putimage(x, y, &plantCards[i]);
	}
}

void drawPlants()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (map[i][j].type > 0)
			{
				//int x = 256 + j * 81;
				//int y = 179 + i * 102;
				int plantType = map[i][j].type - 1;
				int curFrame = map[i][j].frameIndex;
				//putimagePNG(x, y, plantSprites[plantType][curFrame]);
				putimagePNG(map[i][j].x, map[i][j].y, plantSprites[plantType][curFrame]);
			}
		}
	}

	// render the plant
	if (selectedPlant > 0)
	{
		IMAGE* img = plantSprites[selectedPlant - 1][0];
		putimagePNG(curX - img->getwidth() / 2, curY - img->getheight() / 2, img);
	}
}

void drawBullets()
{
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
}

void drawZombies()
{
	// render zombies
	int zombieCount = sizeof(zombiePool) / sizeof(zombiePool[0]);
	for (int i = 0; i < zombieCount; ++i)
	{
		if (zombiePool[i].used)
		{
			//IMAGE* zombieImg = &zombieSprites[zombiePool[i].frameIndex];
			IMAGE* zombieImg = NULL;
			if (zombiePool[i].isDead) zombieImg = zombieDieSprites;
			else if (zombiePool[i].isEating) zombieImg = zombieEatSprites;
			else zombieImg = zombieSprites;
			//IMAGE* zombieImg = (zombiePool[i].isDead) ? zombieDieSprites : zombieSprites;
			zombieImg += zombiePool[i].frameIndex;
			putimagePNG(zombiePool[i].x, zombiePool[i].y - zombieImg->getheight(), zombieImg);
		}
	}
}

void updateWindow()
{
	BeginBatchDraw(); // buffer

	putimage(-112, 0, &bgImage);
	putimagePNG(250, 0, &plantBar);

	drawCards();
	drawPlants();

	drawSunshine();

	drawZombies();
	drawBullets();

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
			int x = sunshinePool[i].curPos.x;
			int y = sunshinePool[i].curPos.y;

			if (msg->x > x && msg->x < x + w &&
				msg->y > y && msg->y < y + h)
			{
				//sunshinePool[i].used = false;
				sunshinePool[i].status = SUNSHINE_COLLECT;

				//mciSendString("play res/audio/sunshine.mp3", 0, 0, 0);
				//PlaySound("res/audio/sunshine.wav", NULL, SND_FILENAME | SND_ASYNC);
				Singleton<AudioManager>::get()->PlaySFX("sunshine");
				//float destY = 0;
				//float destX = 262;
				//float angle = atan((y - destY) / (x - destX));
				//sunshinePool[i].xOffset = 4 * cos(angle);
				//sunshinePool[i].yOffset = 4 * sin(angle);
				sunshinePool[i].p1 = sunshinePool[i].curPos;
				sunshinePool[i].p4 = vector2(262, 0);
				sunshinePool[i].t = 0;
				float distance = dis(sunshinePool[i].p1 - sunshinePool[i].p4);
				float offset = 8;
				sunshinePool[i].speed = 1.0 / (distance / offset);
				break;
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
		else if (msg.message == WM_LBUTTONUP && state == 1)
		{
			if (msg.x > 256 - 112 && msg.y > 179 && msg.y < 489)
			{
				int row = (msg.y - 179) / 102;
				int col = (msg.x - (256 - 112)) / 81;
				std::cout << row << col << std::endl;

				if (map[row][col].type == 0)
				{
					map[row][col].type = selectedPlant;
					map[row][col].frameIndex = 0;
					map[row][col].shootTime = 0;
					map[row][col].x = 256 - 112 + col * 81;
					map[row][col].y = 179 + row * 102 + 14;
				}
					
			}

			selectedPlant = 0;
			state = 0;
		}
	}
}

void createSunshine()
{
	int sunshineCount = sizeof(sunshinePool) / sizeof(sunshinePool[0]);
	static int count = 0;
	static int freq = 400;
	++count;
	if (count >= freq)
	{
		freq = 200 + rand() % 200;
		count = 0;
		
		for (int i = 0; i < sunshineCount; ++i)
		{
			if (!sunshinePool[i].used)
			{
				sunshinePool[i].used = true;
				sunshinePool[i].frameIndex = 0;
				//sunshinePool[i].x = 260 + rand() % (900 - 260); // 260 ~ 900
				//sunshinePool[i].y = 60;
				//sunshinePool[i].destY = 200 + (rand() % 4) * 90;
				sunshinePool[i].timer = 0;
				//sunshinePool[i].xOffset = 0;
				//sunshinePool[i].yOffset = 0;

				sunshinePool[i].status = SUNSHINE_DOWN;
				sunshinePool[i].t = 0;
				sunshinePool[i].p1 = vector2(260 - 112 + rand() % (900 - (260 - 112)), 60);
				sunshinePool[i].p4 = vector2(sunshinePool[i].p1.x, 200 + (rand() % 4) * 90);
				int offset = 2;
				float distance = sunshinePool[i].p4.y - sunshinePool[i].p1.y;
				sunshinePool[i].speed = 1.0 / (distance / offset);

				break;
			}
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (map[i][j].type == SUNFLOWER + 1)
			{
				map[i][j].timer++;
				if (map[i][j].timer > 200)
				{
					map[i][j].timer = 0;
					for (int k = 0; k < sunshineCount; ++k)
					{
						if (!sunshinePool[k].used)
						{
							sunshinePool[k].used = true;
							sunshinePool[k].p1 = vector2(map[i][j].x, map[i][j].y);
							int w = 100 + rand() % 50 * (rand() % 2 ? 1 : -1);
							sunshinePool[k].p4 = vector2(map[i][j].x + w, 
								map[i][j].y + plantSprites[SUNFLOWER][0]->getheight() - sunshineSprites[0].getheight());
							sunshinePool[k].p2 = vector2(sunshinePool[k].p1.x + w * 0.3, sunshinePool[k].p1.y - 100);
							sunshinePool[k].p3 = vector2(sunshinePool[k].p1.x + w * 0.7, sunshinePool[k].p1.y - 100);
							sunshinePool[k].status = SUNSHINE_PRODUCT;
							sunshinePool[k].speed = 0.05;
							sunshinePool[k].t = 0;
							break;
						}
					}
				}
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
			if (sunshinePool[i].status == SUNSHINE_DOWN)
			{
				struct Sunshine* sun = &sunshinePool[i];
				sun->t += sun->speed;
				sun->curPos = sun->p1 + sun->t * (sun->p4 - sun->p1);
				if (sun->t >= 1)
				{
					sun->status = SUNSHINE_GROUND;
					sun->timer = 0;
				}
			}
			else if (sunshinePool[i].status == SUNSHINE_GROUND)
			{
				sunshinePool[i].timer++;
				if (sunshinePool[i].timer > 100)
				{
					sunshinePool[i].used = false;
					sunshinePool[i].timer = 0;
				}
			}
			else if (sunshinePool[i].status == SUNSHINE_COLLECT)
			{
				struct Sunshine* sun = &sunshinePool[i];
				sun->t += sun->speed;
				sun->curPos = sun->p1 + sun->t * (sun->p4 - sun->p1);
				if (sun->t > 1)
				{
					sun->used = false;
					sunshine += 25;
				}
			}
			else if (sunshinePool[i].status == SUNSHINE_PRODUCT)
			{
				struct Sunshine* sun = &sunshinePool[i];
				sun->t += sun->speed;
				sun->curPos = calcBezierPoint(sun->t, sun->p1, sun->p2, sun->p3, sun->p4);
				if (sun->t > 1)
				{
					sun->status = SUNSHINE_GROUND;
					sun->timer = 0;
				}
			}

			//sunshinePool[i].frameIndex = (sunshinePool[i].frameIndex + 1) % 29;
			//if (sunshinePool[i].timer == 0)
			//{
			//	sunshinePool[i].y += 4;
			//}
			//if (sunshinePool[i].y >= sunshinePool[i].destY)
			//{
			//	sunshinePool[i].timer++;
			//	if (sunshinePool[i].timer > 100)
			//	{
			//		sunshinePool[i].used = false;
			//	}
			//}
		}
		//else if (sunshinePool[i].xOffset)
		//{
		//	float destY = 0;
		//	float destX = 262;
		//	float angle = atan((sunshinePool[i].y - destY) / (sunshinePool[i].x - destX));
		//	sunshinePool[i].xOffset = 4 * cos(angle);
		//	sunshinePool[i].yOffset = 4 * sin(angle);

		//	sunshinePool[i].x -= sunshinePool[i].xOffset;
		//	sunshinePool[i].y -= sunshinePool[i].yOffset;
		//	if (sunshinePool[i].y < 0 || sunshinePool[i].x < 262)
		//	{
		//		sunshinePool[i].xOffset = 0;
		//		sunshinePool[i].yOffset = 0;
		//		sunshine += 25;
		//	}
		//}
	}
}

void createZombie()
{
	if (createdZombieCount >= ZOMBIE_MAX) return;

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
				memset(&zombiePool[i], 0, sizeof(zombiePool[i]));
				zombiePool[i].used = true;
				zombiePool[i].speed = 1;
				zombiePool[i].hp = 10;
				zombiePool[i].isDead = false;
				zombiePool[i].row = rand() % 3;
				zombiePool[i].x = WINDOW_WIDTH;
				zombiePool[i].y = 172 + (1 + zombiePool[i].row) * 100;
				createdZombieCount++;
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
			if (zombiePool[i].isDead)
			{
				zombiePool[i].frameIndex++;
				if (zombiePool[i].frameIndex >= 10)
				{
					zombiePool[i].used = false;
					killCount++;
					if (killCount == ZOMBIE_MAX)
					{
						gameStatus = WIN;
					}
				}
			}
			else if (zombiePool[i].isEating)
			{
				zombiePool[i].frameIndex = (zombiePool[i].frameIndex + 1) % 21;
			}
			else
			{
				zombiePool[i].frameIndex = (zombiePool[i].frameIndex + 1) % 22;
			}
			
			if (zombiePool[i].x < 170 - 112)
			{
				//MessageBox(NULL, "over", "fuck you loser", 0);
				//exit(0);
				gameStatus = FAIL;
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
				//static int count = 0;
				//++count;
				map[i][j].shootTime++;

				if (map[i][j].shootTime > 20)
				{
					map[i][j].shootTime = 0;
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
							int plantX = 256 - 112 + j * 81;
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

void checkZombieCollision()
{
	int zombieCount = sizeof(zombiePool) / sizeof(zombiePool[0]);
	for (int i = 0; i < zombieCount; ++i)
	{
		if (zombiePool[i].isDead) continue;
		int row = zombiePool[i].row;
		for (int k = 0; k < 9; ++k)
		{
			if (map[row][k].type == 0) continue;

			int plantX = 256 - 112 + k * 81;
			int x1 = plantX + 10;
			int x2 = plantX + 60;
			int x3 = zombiePool[i].x + 80;
			if (x3 > x1 && x3 < x2)
			{
				if (map[row][k].catched)
				{
					map[row][k].hp--;
					if (map[row][k].hp <= 0)
					{
						map[row][k].hp = 0;
						map[row][k].type = 0;
						zombiePool[i].isEating = false;
						zombiePool[i].frameIndex = 0;
						zombiePool[i].speed = 1;
					}
				}
				else
				{
					map[row][k].catched = true;
					map[row][k].hp = 50;
					zombiePool[i].isEating = true;
					zombiePool[i].speed = 0;
					zombiePool[i].frameIndex = 0;
				}
			}
		}
	}
}

void checkBulletCollision()
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
			if (!zombiePool[k].isDead && bulletsPool[i].row == zombiePool[k].row && x > x1 && x < x2)
			{
				zombiePool[k].hp -= 1;//10;
				bulletsPool[i].blast = true;
				bulletsPool[i].speed = 0;

				if (zombiePool[k].hp <= 0)
				{
					zombiePool[k].isDead = true;
					zombiePool[k].speed = 0;
					zombiePool[k].frameIndex = 0;
				}

				break;
			}
		}
	}
}

void collisionCheck()
{
	checkBulletCollision();
	checkZombieCollision();
}

void updatePlant()
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
}

void renderAll()
{

	updatePlant();

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
				EndBatchDraw();
				break;
			}
		}

		EndBatchDraw();
	}
	
}

void viewScene()
{
	int xMin = WINDOW_WIDTH - bgImage.getwidth(); // 900-1400=-500
	vector2 points[9] = {
		{550, 80}, {530, 160}, {630, 170}, {530, 200}, {515, 270}, {565, 370}, {605, 340}, {705, 280}, {690, 340}
	};
	int index[9];
	for (int i = 0; i < 9; ++i)
	{
		index[i] = rand() % 11;
	}

	int count = 0;
	for (int x = 0; x >= xMin; x -= 2)
	{
		BeginBatchDraw();

		putimage(x, 0, &bgImage);

		// zombies preview
		// TODO
		++count;
		for (int k = 0; k < 9; ++k)
		{
			putimagePNG(points[k].x - xMin + x, points[k].y, &zombieStand[index[k]]);
			if (count >= 10)
			{
				index[k] = (index[k] + 1) % 11;
			}
		}

		if (count >= 10) count = 0;

		EndBatchDraw();
		Sleep(5);
	}

	for (int i = 0; i < 100; ++i)
	{
		BeginBatchDraw();

		putimage(xMin, 0, &bgImage);
		for (int k = 0; k < 9; ++k)
		{
			putimagePNG(points[k].x, points[k].y, &zombieStand[index[k]]);
			index[k] = (index[k] + 1) % 11;
		}
		EndBatchDraw();
		Sleep(40);
	}

	//往回移
	for (int x = xMin; x <= -112; x += 2) {	//每帧移动2像素
		BeginBatchDraw();

		putimage(x, 0, &bgImage);

		count++;
		for (int k = 0; k < 9; k++) {
			putimagePNG(points[k].x - xMin + x, points[k].y, &zombieStand[index[k]]);
			if (count >= 10) {
				index[k] = (index[k] + 1) % 11;
			}
			if (count >= 10) count = 0;
		}

		EndBatchDraw();
		Sleep(5);
	}
}

void barDown()
{
	int height = plantBar.getheight();
	for (int y = -height; y <= 0; y++) {
		BeginBatchDraw();

		putimage(-112, 0, &bgImage);

		putimagePNG(250, y, &plantBar);

		for (int i = 0; i < PLANT_COUNT; i++) {
			int x = 338 + i * 65;
			putimagePNG(x, 6 + y, &plantCards[i]);
		}

		EndBatchDraw();
		Sleep(5);
	}
}

bool checkGameOver()
{
	bool res = false;
	if (gameStatus == WIN)
	{
		Sleep(1000);
		loadimage(0, "res/win.png");
		res = true;
	}
	else if (gameStatus == FAIL)
	{
		Sleep(1000);
		loadimage(0, "res/fail.png");
		res = true;
	}
	return res;
}



int main()
{

	AudioAsset* audioAsset = Singleton<AssetManager>::get()->Load<AudioAsset>("sunshine", "res/audio/sunshine.wav");
	ImageAsset* imageAsset = Singleton<AssetManager>::get()->Load<ImageAsset>("map", "res/map.jpg");
	Singleton<AudioManager>::get()->PlayBGM("res/audio/BG.MP3", 50);
	//sf::Sprite* mapSprite = Singleton<AssetManager>::get()->Get<ImageAsset>("map")->GetSprite();
	

	gameInit();

	startMenu();

	//viewScene();

	barDown();

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
			if (checkGameOver())
			{
				break;
			}
		}

	}

	//GameEngine gameEngine;
	//GameEngine::GetEntities();
	//gameEngine.run();


	system("pause");
	return 0;
}