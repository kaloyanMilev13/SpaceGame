#include <stdlib.h>
#include <raylib.h>
#include <stddef.h>


#define SCREEN_WIDTH 800 //1000
#define SCREEN_HEIGHT 500 //625


#define SHIP_WIDTH 50 //100
#define SHIP_HEIGHT 50 //150

//#define ASTER_RADIUS 10
#define ASTEROID_HEIGHT 60
#define ASTEROID_WIDTH 60


#define PROJECTILE_WIDTH 5
#define PROJECTILE_HEIGHT 5


#define ASTEROID_COUNT  5
#define PROJECTILE_COUNT 3


//implement speed acceleration - 
//implement shooting!!! - almost done on 7.09.26
//implent background - done
//implement real design - on 4th of september -> png files, maybe draw own
//implement collision - done



bool gameRunning;


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
	//int r;
	int height;
	int width;
	bool isAlive;

} Obstacle;


typedef struct {

	int x;
	int y;
	int width;
	int height;
	bool isAlive;

} Projectile;


//ship
Rectangle ship_source;

Rectangle ship_destination;


//asteroids
Rectangle aster_source;

Rectangle aster_destination[ASTEROID_COUNT];


//peojctiles
Rectangle proj_source;

Rectangle proj_destination[PROJECTILE_COUNT];



void setGameStart(Player *ship, Obstacle aster[], Projectile proj[]){


	gameRunning = 1;

	//init asteroids
	for(int i = 0; i < ASTEROID_COUNT; i++){
		aster[i].width = ASTEROID_WIDTH - 10;
		aster[i].height = ASTEROID_HEIGHT- 10;
		aster[i].isAlive = 1;
		aster[i].x = rand() % SCREEN_WIDTH;
		aster[i].y = -(i * 20 + aster[i].height + rand() % 10); //start off screen
		
		aster_destination[i].x = aster[i].x;
		aster_destination[i].y = aster[i].y;
		aster_destination[i].width = aster[i].width;
		aster_destination[i].height = aster[i].height;

	}	


	//init ship
	ship->x = SCREEN_WIDTH/2 - SHIP_WIDTH/2;
	ship->y = SCREEN_HEIGHT/2 - SHIP_HEIGHT/2;
	ship->width = SHIP_WIDTH;
	ship->height = SHIP_HEIGHT;

	ship_destination.x = ship->x;
	ship_destination.y = ship->y;
	ship_destination.width = ship->width;
	ship_destination.height = ship->height;



	//init projectile
	for(int i = 0; i < PROJECTILE_COUNT; i++){
		proj[i].isAlive = 0;
		proj[i].width = PROJECTILE_WIDTH;
		proj[i].height = PROJECTILE_HEIGHT;


		proj_destination[i].x = proj[i].x;
		proj_destination[i].y = proj[i].y;
		proj_destination[i].width = proj[i].width;
		proj_destination[i].height = proj[i].height;

	}	

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


	ship_destination.x = ship->x;
	ship_destination.y = ship->y;


}


void spawnAsteroids(Obstacle aster[]){

	for(int i = 0; i < ASTEROID_COUNT; i++){

		if(aster[i].isAlive == 0){

			aster[i].isAlive = 1;

			aster[i].y = -(i * 20 + aster[i].height	+ rand() % 10); //start off screen

			aster[i].x = rand() % SCREEN_WIDTH;


			aster_destination[i].x = aster[i].x;
			aster_destination[i].y = aster[i].y;


		}

	}

}


void moveAsteroids(Obstacle aster[]){

	for(int i = 0; i < ASTEROID_COUNT; i++){

		if(aster[i].y + aster[i].height  < SCREEN_HEIGHT){

			aster[i].y += 5;

			aster_destination[i].x = aster[i].x;
			aster_destination[i].y = aster[i].y;

		}else {

			aster[i].isAlive = 0;

		}

	}


}


void shootWeapon(Projectile proj[], Player *ship){

	for(int i = 0; i < PROJECTILE_COUNT; i++){

		if(IsKeyPressed(KEY_SPACE)){

			if(proj[i].isAlive == 0){

				proj[i].isAlive = 1;
				proj[i].x = ship->x + SHIP_WIDTH/4;
				proj[i].y = ship->y;


				proj_destination[i].x = proj[i].x;
				proj_destination[i].y = proj[i].y;


			}
		}
	}
}


void moveProjectile(Projectile proj[]){

	for(int i = 0; i < PROJECTILE_COUNT; i++){

		if(proj[i].y + PROJECTILE_HEIGHT  >  0){

			proj[i].y -= 10;

			proj_destination[i].x = proj[i].x;
			proj_destination[i].y = proj[i].y;


		}else {

			proj[i].isAlive = 0;

		}

	}

}

void checkCollisions(Player *ship, Obstacle aster[], Projectile proj[]){


	for(int i = 0; i < ASTEROID_COUNT; i++){

		if(aster[i].isAlive && aster[i].x + aster[i].width >= ship->x && aster[i].x <= ship->x + SHIP_WIDTH &&
				aster[i].y + aster[i].height >= ship->y && aster[i].y <= ship->y + SHIP_HEIGHT){

			ship->isAlive = 0;
			aster[i].isAlive = 0;
			gameRunning = 0;


			WaitTime(1);	
		}




		for(int j = 0; j < PROJECTILE_COUNT; j++){

			if(proj[j].isAlive && aster[i].isAlive && aster[i].x + aster[i].width >= proj[j].x && aster[i].x <= proj[j].x + PROJECTILE_WIDTH &&
					aster[i].y + aster[i].height >= proj[j].y && aster[i].y <= proj[j].y + PROJECTILE_HEIGHT){


				aster[i].isAlive = 0;
				proj[j].isAlive = 0;


			}


		}



	}

}


int main(void){

	//INIT WINDOW
	InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "SpaceGame"); //init
	SetTargetFPS(60); //set fps



	//ToggleFullscreen();

	//INIT OBJECTS
	Player ship;

	Obstacle aster[ASTEROID_COUNT];

	Projectile proj[PROJECTILE_COUNT];


	setGameStart(&ship, aster, proj);


	Texture2D background = LoadTexture("img/background.png");

	Texture2D ship_png = LoadTexture("img/ship.png");
	ship_source = (Rectangle){0, 0, ship_png.width, ship_png.height};

	Texture2D asteroid_png = LoadTexture("img/asteroid.png");
	aster_source = (Rectangle){0, 0, asteroid_png.width, asteroid_png.height};

	Texture2D proj_png = LoadTexture("img/projectile.png");
	proj_source = (Rectangle){0, 0, proj_png.width, proj_png.height};






	while(!WindowShouldClose()){




		if(gameRunning == 0){

			BeginDrawing(); //2ri init na samoto risuwane

			ClearBackground(BLACK);

			DrawText("GAME OVER!!", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, WHITE);


			if(IsKeyPressed(KEY_R)){
				gameRunning = 1;
				setGameStart(&ship, aster, proj);
			}


			EndDrawing(); //zatwarqne na chetkata


		}else if(gameRunning){


			BeginDrawing(); //2ri init na samoto risuwane

			ClearBackground(BLACK);//set background color

			DrawTexture(background, 0, 0, WHITE);

			//spawn asteroids
			//move ship
			//check for shooting
			//move proj
			//move asteroids
			//check collision

			spawnAsteroids(aster);

			moveShip(&ship);

			shootWeapon(proj, &ship);

			moveProjectile(proj);

			moveAsteroids(aster);
			
			checkCollisions(&ship, aster, proj);
			
			DrawTexturePro(ship_png, ship_source, ship_destination, (Vector2){0, 0}, 0.0f,  WHITE);
			//DrawRectangle(ship.x, ship.y, SHIP_WIDTH, SHIP_HEIGHT, WHITE);

			for(int i = 0; i < ASTEROID_COUNT; i++){

				if(aster[i].isAlive){
					//DrawTexture(asteroid_png, aster[i].x, aster[i].y, WHITE);

					DrawTexturePro(asteroid_png, aster_source, aster_destination[i], (Vector2){0,0}, 0.0f, WHITE);
					
					//DrawRectangle(aster[i].y, aster[i].y, 	ASTEROID_WIDTH, ASTEROID_HEIGHT, WHITE);


				}
			}


			for(int i = 0; i < PROJECTILE_COUNT; i++){

				if(proj[i].isAlive){

					DrawRectangle(proj[i].x, proj[i].y, proj[i].width, proj[i].height, WHITE);

				}
			}

			
			



			//DrawRectangle(ship.x, ship.y, ship.width, ship.height, WHITE);

			EndDrawing(); //zatwarqne na chetkata

		}




	}

	UnloadTexture(background);
	CloseWindow();



	return 0;

}

