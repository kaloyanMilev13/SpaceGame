#include <complex.h>
#include <stdlib.h>
#include <raylib.h>
#include <stddef.h>


#define SCREEN_WIDTH 800 //1000
#define SCREEN_HEIGHT 500 //625


#define SHIP_WIDTH 20 //100
#define SHIP_HEIGHT 30 //150

#define ASTER_RADIUS 10
//#define ASTER_HEIGHT 30



//implement speed acceleration
//implement shooting
//implent background
//implement real design
//implement collision



bool gameRunning;
int asteroid_cnt = 5;

typedef struct {

	int x;
	int y;
	int width;
	int height;
	bool isAlive;

} Player;


typedef struct {

	int x;
	int y;
	int r;
	//int height;
	bool isAlive;

} Obstacle;


void setGameStart(Player *ship, Obstacle aster[], int n){


	gameRunning = 1;

	//init asteroids
	for(int i = 0; i < n; i++){
		aster[i].r = ASTER_RADIUS;
		aster[i].isAlive = 1;
		aster[i].x = rand() % SCREEN_WIDTH;
		aster[i].y = -(i * 20 + aster[i].r + rand() % 10); //start off screen
	}	


	//init ship
	ship->x = SCREEN_WIDTH/2 - SHIP_WIDTH/2;
	ship->y = SCREEN_HEIGHT/2 - SHIP_HEIGHT/2;
	ship->width = SHIP_WIDTH;
	ship->height = SHIP_HEIGHT;

}


void moveShip(Player *ship){

	if(IsKeyDown(KEY_A)){

		if(ship->x - 5 > 0){

			ship->x -= 5;

		}
	}



	if(IsKeyDown(KEY_D)){

		if(ship->x + ship->width + 5 < SCREEN_WIDTH){

			ship->x += 5;

		}		
	}



	if(IsKeyDown(KEY_S)){

		if(ship->y + ship->height + 5 < SCREEN_HEIGHT){

			ship->y += 5;

		}


	}



	if(IsKeyDown(KEY_W)){

		if(ship->y - 5 > 0){

			ship->y -= 5;

		}

	}

}


void spawnAsteroids(Obstacle aster[], int n){

	for(int i = 0; i < n; i++){

		if(aster[i].isAlive == 0){

			aster[i].isAlive = 1;

			aster[i].y = -(i * 20 + aster[i].r + rand() % 10); //start off screen

			aster[i].x = rand() % SCREEN_WIDTH;


		}

	}

}


void moveAsteroids(Obstacle aster[], int n){

	for(int i = 0; i < n; i++){

		if(aster[i].y + aster[i].r  < SCREEN_HEIGHT){

			aster[i].y += 5;

		}else {

			aster[i].isAlive = 0;

		}

	}


}


void checkCollisions(Player *ship, Obstacle aster[], int n){


	for(int i = 0; i < n; i++){

		if(aster[i].isAlive && aster[i].x + aster[i].r >= ship->x && aster[i].x - aster[i].r <= ship->x + SHIP_WIDTH && aster[i].y + aster[i].r >= ship->y && aster[i].y - aster[i].r <= ship->y + SHIP_HEIGHT){

			ship->isAlive = 0;
			aster[i].isAlive = 0;
			gameRunning = 0;	
		}

	}

}


int main(void){

	InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "SpaceGame"); //init
	SetTargetFPS(60); //set fps



	//ToggleFullscreen();



	Texture2D background = LoadTexture("img/background_ex.png");



	Player ship;

	Obstacle aster[asteroid_cnt];


	setGameStart(&ship, aster, asteroid_cnt);


	while(!WindowShouldClose()){

		
		
		
		if(gameRunning == 0){

			BeginDrawing(); //2ri init na samoto risuwane

			ClearBackground(BLACK);
		
			DrawText("GAME OVER!!", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, WHITE);


			if(IsKeyPressed(KEY_SPACE)){
				gameRunning = 1;
				setGameStart(&ship, aster, asteroid_cnt);
			}

			
			EndDrawing(); //zatwarqne na chetkata


		}else if(gameRunning){
			
			BeginDrawing(); //2ri init na samoto risuwane
			
			ClearBackground(BLACK);//set background color

			//DrawTexture(background, 0, 0, WHITE);

			spawnAsteroids(aster, asteroid_cnt);

			moveShip(&ship);

			moveAsteroids(aster, asteroid_cnt);

			checkCollisions(&ship, aster, asteroid_cnt);

			for(int i = 0; i < asteroid_cnt; i++){

				if(aster[i].isAlive){

					DrawCircle(aster[i].x, aster[i].y, aster[i].r, WHITE);

				}
			}


			DrawRectangle(ship.x, ship.y, ship.width, ship.height, WHITE);

			EndDrawing(); //zatwarqne na chetkata

		}




	}

	UnloadTexture(background);
	CloseWindow();



	return 0;

}

