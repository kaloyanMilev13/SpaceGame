#include <stdlib.h>
#include <raylib.h>


#define SCREEN_WIDTH 800 //1000
#define SCREEN_HEIGHT 500 //625


#define SHIP_WIDTH 60 //100
#define SHIP_HEIGHT 60 //150

#define ASTER_RADIUS 25
#define ASTEROID_HEIGHT 60
#define ASTEROID_WIDTH 60


#define PROJECTILE_WIDTH 5
#define PROJECTILE_HEIGHT 5


#define ASTEROID_COUNT  5
#define PROJECTILE_COUNT 1


//implement speed acceleration - 
//implement shooting!!! - almost done on 7.09.26
//implent background - done
//implement real design - on 4th of september -> png files, maybe draw own
//implement collision - done; 07.09.2026 - better collision method


float dt;
float speed = 360.0f;

bool gameRunning;

unsigned int score = 0;
unsigned int highscore = 0;



//make it a triangle pointing up
//actually decided to keep it a rectangle so that the wings of the rocket are not in the hitbox, only the body - 08.09.2026
typedef struct {

	float x;
	float y;

	int width;
	int height
		;
	bool isAlive;

} Player;


//make it a circle - 07.09.2026
typedef struct {

	int x;
	int y;
	int r;
	int height;
	int width;
	bool isAlive;

} Obstacle;


//keep it a rectangle; 
//shpuld add an image to it
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
	score = 0;

	//init asteroids
	for(int i = 0; i < ASTEROID_COUNT; i++){

		aster[i].r = ASTER_RADIUS;

		aster[i].width = ASTEROID_WIDTH;
		aster[i].height = ASTEROID_HEIGHT;
		aster[i].isAlive = 1;
		aster[i].x = rand() % SCREEN_WIDTH;
		aster[i].y = -(i * 20 + aster[i].height + rand() % 10); //start off screen
		
		aster_destination[i].x = aster[i].x - aster[i].r;
		aster_destination[i].y = aster[i].y - aster[i].r;

		aster_destination[i].width = aster[i].width;
		aster_destination[i].height = aster[i].height;

	}	


	//init ship
	ship->x =(((float)SCREEN_WIDTH/2 - (float)SHIP_WIDTH/2) + (float)SHIP_WIDTH/4); // 400 - 60 + 15
	ship->y = (float)SCREEN_HEIGHT/2 - (float)SHIP_HEIGHT/2;
	ship->width = SHIP_WIDTH/3; // 60 / 3 = 20; 
	ship->height = SHIP_HEIGHT;

	ship_destination.x = (float)SCREEN_WIDTH/2 -(float)SHIP_WIDTH/2;
	ship_destination.y =(float)SCREEN_HEIGHT/2 - (float)SHIP_HEIGHT/2;
	ship_destination.width = SHIP_WIDTH;
	ship_destination.height = SHIP_HEIGHT;



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


void moveShip(Player *ship, float dt){

	float dx = 0.0f;
	float dy = 0.0f;

	if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){

		if(ship->x - 5 > 0){

			dx -= 1.0f;

		}
	}



	if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){

		if(ship->x + ship->width < SCREEN_WIDTH){

			dx += 1.0f;
		}
				
	}



	if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){

		if(ship->y + ship->height < SCREEN_HEIGHT){

			dy += 1.0f;

		}


	}



	if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)){

		if(ship->y > 0){

			dy -= 1.0f ;

		}

	}



	ship->x += dx * speed * dt; // direction on x * speed * time
	ship->y += dy * speed * dt; //direction on y * speed * time since last frame

	ship_destination.x = ship->x - (float)SHIP_WIDTH/4;
	ship_destination.y = ship->y;


}


void spawnAsteroids(Obstacle aster[]){

	for(int i = 0; i < ASTEROID_COUNT; i++){

		if(aster[i].isAlive == 0){

			aster[i].isAlive = 1;

			aster[i].y = -(i * 20 + aster[i].height	+ rand() % 10); //start off screen

			aster[i].x = rand() % SCREEN_WIDTH;


			aster_destination[i].x = aster[i].x - aster[i].r;
			aster_destination[i].y = aster[i].y - aster[i].r;


		}

	}

}


void moveAsteroids(Obstacle aster[]){

	for(int i = 0; i < ASTEROID_COUNT; i++){

		if(aster[i].y + aster[i].height  < SCREEN_HEIGHT){

			aster[i].y += 5;

			aster_destination[i].x = aster[i].x - aster[i].r;
			aster_destination[i].y = aster[i].y - aster[i].r;

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
				proj[i].x = ship->x + (float)SHIP_WIDTH/4;
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

		

		//Square detection
		//if(aster[i].isAlive && aster[i].x + aster[i].width >= ship->x && aster[i].x <= ship->x + SHIP_WIDTH &&
		//		aster[i].y + aster[i].height >= ship->y && aster[i].y <= ship->y + SHIP_HEIGHT){


		//Circle detection
		if(aster[i].isAlive && aster[i].x + aster[i].r >= ship->x && aster[i].x - aster[i].r <= ship->x + (float)SHIP_WIDTH/4 && 
				aster[i].y + aster[i].r >= ship->y && aster[i].y - aster[i].r <= ship->y + SHIP_HEIGHT){
			ship->isAlive = 0;
			aster[i].isAlive = 0;
			gameRunning = 0;	
		}




		for(int j = 0; j < PROJECTILE_COUNT; j++){

			if(proj[j].isAlive && aster[i].isAlive && aster[i].x + aster[i].r >= proj[j].x && aster[i].x - aster[i].r <= proj[j].x + PROJECTILE_WIDTH &&
					aster[i].y + aster[i].r >= proj[j].y && aster[i].y - aster[i].r <= proj[j].y + PROJECTILE_HEIGHT){


				aster[i].isAlive = 0;
				proj[j].isAlive = 0;
				score++; //not sure


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

			if(score >= highscore){
				highscore = score;
			}

			BeginDrawing(); //2ri init na samoto risuwane

			ClearBackground(BLACK);

			DrawText("GAME OVER!!", SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2, 20, WHITE);

			DrawText("Score: ", SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 + 30, 20, WHITE);
			DrawText(TextFormat("%u", score), SCREEN_WIDTH/2 + 100, SCREEN_HEIGHT/2 + 30, 20, WHITE);

			DrawText("High Score: ", SCREEN_WIDTH/2 - 15, SCREEN_HEIGHT/2 + 60, 20, WHITE);
			DrawText(TextFormat("%u", highscore), SCREEN_WIDTH/2 + 140, SCREEN_HEIGHT/2 + 60, 20, WHITE);

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
			
			//dt is the delta time, made to make movement independent of FPS
			dt = GetFrameTime();
			moveShip(&ship, dt);

			shootWeapon(proj, &ship);

			moveProjectile(proj);

			moveAsteroids(aster);

			checkCollisions(&ship, aster, proj);
	
			DrawTexturePro(ship_png, ship_source, ship_destination, (Vector2){0, 0}, 0.0f,  WHITE);
			
			//hitbox
			//DrawRectangle(ship.x, ship.y, ship.width, ship.height, WHITE);

			for(int i = 0; i < ASTEROID_COUNT; i++){

				if(aster[i].isAlive){

					DrawTexturePro(asteroid_png, aster_source, aster_destination[i], (Vector2){0,0}, 0.0f, WHITE);
					
					//hitbox
					//DrawCircle(aster[i].x, aster[i].y, aster[i].r, WHITE);	

				}
			}


			for(int i = 0; i < PROJECTILE_COUNT; i++){

				if(proj[i].isAlive){

					DrawRectangle(proj[i].x, proj[i].y, proj[i].width, proj[i].height, WHITE);

				}
			}
				

			DrawText("Score: ", 650, 5, 20, WHITE);
			DrawText(TextFormat("%u", score), 750, 5, 20, WHITE);
			
			
			//DrawRectangle(ship.x, ship.y, ship.width, ship.height, WHITE);

			EndDrawing(); //zatwarqne na chetkata

		}




	}

	UnloadTexture(background);
	UnloadTexture(ship_png);
	UnloadTexture(asteroid_png);
	UnloadTexture(proj_png);
	CloseWindow();



	return 0;

}

