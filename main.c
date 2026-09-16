#include <complex.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>


#define SCREEN_WIDTH 800 //1000
#define SCREEN_HEIGHT 500 //625

#define SHIP_RADIUS 10
#define SHIP_WIDTH 60 
#define SHIP_HEIGHT 60 

#define ASTER_RADIUS 25
#define ASTEROID_HEIGHT 60
#define ASTEROID_WIDTH 60


#define PROJECTILE_WIDTH 10
#define PROJECTILE_HEIGHT 20

#define REWARD_RADIUS 20
#define REWARD_HEIGHT 30
#define REWARD_WIDTH 30

#define ASTEROID_COUNT  5
#define PROJECTILE_COUNT 1

#define DIFFICULTY_INDEX 7.0f

//gymnasium frama foundation
//hugging face lerobot


//implement speed acceleration - done on 10.09.2026
//implement shooting!!! - almost done on 7.09.26
//implent background - done
//implement real design - on 4th of september -> png files, maybe draw own
//implement collision - done; 07.09.2026 - better collision method


float dt;

bool gameRunning;

unsigned int score = 0;
unsigned int highscore = 0;

float now;
float lastTime = 0;
float rewardCooldown = 10.0f;



//circle
typedef struct {

	float x;
	float y;

	int r;

	float speed;

	bool isAlive;

	float angle;

} Player;


//make it a circle - 07.09.2026
typedef struct {

	float x;
	float y;

	float r;

	float speed;

	bool isAlive;

} Obstacle;


//keep it a rectangle; 
//shpuld add an image to it
typedef struct {

	float x;
	float y;

	//to remember the direction and not rotate with the ship
	float dx;
	float dy;
	
	float angle;

	float speed;

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

//reward
Rectangle reward_source;
Rectangle reward_destination;



void setGameStart(Player *ship, Obstacle aster[], Projectile proj[], Obstacle *reward){


	gameRunning = 1;
	score = 0;
	lastTime = GetTime();

	//init asteroids
	for(int i = 0; i < ASTEROID_COUNT; i++){

		aster[i].speed = 300.0f;

		aster[i].r = ASTER_RADIUS;
		aster[i].isAlive = 1;
		aster[i].x = rand() % SCREEN_WIDTH;
		aster[i].y = -(i * 20 + ASTEROID_HEIGHT + rand() % 10); //start off screen

		aster_destination[i].x = aster[i].x - aster[i].r;
		aster_destination[i].y = aster[i].y - aster[i].r;

		aster_destination[i].width = ASTEROID_WIDTH;
		aster_destination[i].height = ASTEROID_HEIGHT;

	}	


	//init ship
	ship->r = SHIP_RADIUS;
	ship->isAlive = 1;
	ship->speed = 380.0f;

// 	for rect ship	
//	ship->x =(((float)SCREEN_WIDTH/2 - (float)SHIP_WIDTH/2) + (float)SHIP_WIDTH/4); // 400 - 60 + 15
//	ship->y = (float)SCREEN_HEIGHT/2 - (float)SHIP_HEIGHT/2;

	ship->x = (float)SCREEN_WIDTH/2;
	ship->y = (float)SCREEN_HEIGHT/2 - SHIP_RADIUS*2;

	ship->angle = 0;

// 	for rect ship
//	ship_destination.x = (float)SCREEN_WIDTH/2 -(float)SHIP_WIDTH/2;
//	ship_destination.y =(float)SCREEN_HEIGHT/2 - (float)SHIP_HEIGHT/2;

	ship_destination.x = ship->x - ship->r;
	ship_destination.y = ship->y - ship->r;

	ship_destination.width = SHIP_WIDTH;
	ship_destination.height = SHIP_HEIGHT;



	//init projectile
	for(int i = 0; i < PROJECTILE_COUNT; i++){
		proj[i].isAlive = 0;
		proj[i].speed = 600.0f;
		proj_destination[i].x = proj[i].x;
		proj_destination[i].y = proj[i].y;
		proj_destination[i].width = PROJECTILE_WIDTH;
		proj_destination[i].height = PROJECTILE_HEIGHT;

	}


	//init reward
	reward->r = REWARD_RADIUS;


	reward->isAlive = 1;

	reward->speed = 300.0f;

	reward->x = rand() % SCREEN_WIDTH;
	reward->y = -(20 + REWARD_HEIGHT + rand() % 10); 

	reward_destination.x = reward->x - reward->r;
	reward_destination.y = reward->y - reward->r;
	reward_destination.width = REWARD_WIDTH;
	reward_destination.height = REWARD_HEIGHT;



}


void moveShip(Player *ship, float dt,  unsigned int *score){

	float dx = 0.0f;
	float dy = 0.0f;

	if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){

		if(ship->x - ship->r > 0){

			dx -= 1.0f;

		}
	}



	if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){

		if(ship->x + ship->r < SCREEN_WIDTH){

			dx += 1.0f;
		}

	}



	if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){

		if(ship->y + ship->r < SCREEN_HEIGHT){

			dy += 1.0f;

		}


	}



	if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)){

		if(ship->y - ship->r > 0){

			dy -= 1.0f ;

		}

	}


	ship->x += dx * (ship->speed + *score * DIFFICULTY_INDEX) * dt; // direction on x * (speed + speed increase)* time
	ship->y += dy * (ship->speed + *score * DIFFICULTY_INDEX) * dt; //direction on y * (speed + speed increasment)* time since last frame

	//new for rect
	//ship_destination.x = ship->x + ship->width / 2.0f;
	//ship_destination.y = ship->y + ship->height / 2.0f;
	
	//dest for a circle ship
	ship_destination.x = ship->x;
	ship_destination.y = ship->y;
	

	if(!(dx == 0 && dy == 0)){

		ship->angle = (atan2(dx, -dy)) * 180.0 / PI;
	
	}



}


void spawnObstacles(Obstacle aster[], Obstacle *reward){

	now = GetTime();

	if(now - lastTime >= rewardCooldown && reward->isAlive == 0){

		reward->isAlive = 1;

		reward->x = rand() % SCREEN_WIDTH;
		reward->y = -(20 + REWARD_HEIGHT + rand() % 10); 

		lastTime = now;
	}



	for(int i = 0; i < ASTEROID_COUNT; i++){

		if(aster[i].isAlive == 0){

			aster[i].isAlive = 1;

			aster[i].y = -(i * 20 + ASTEROID_HEIGHT	+ rand() % 10); //start off screen

			aster[i].x = rand() % SCREEN_WIDTH;


			aster_destination[i].x = aster[i].x;
			aster_destination[i].y = aster[i].y;


		}

	}

}


void moveAsteroids(Obstacle aster[], float dt, unsigned int *score){

	for(int i = 0; i < ASTEROID_COUNT; i++){

		if(aster[i].y + ASTEROID_HEIGHT  < SCREEN_HEIGHT){

			aster[i].y += (aster[i].speed + (*score * DIFFICULTY_INDEX)) * dt;

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

				proj[i].x = ship->x;
				proj[i].y = ship->y;

				proj[i].angle = ship->angle;

				float rad = proj[i].angle * DEG2RAD;
				
				proj[i].dx = sinf(rad);
				proj[i].dy = -cosf(rad);

				proj_destination[i].x = proj[i].x;
				proj_destination[i].y = proj[i].y;


			}
		}
	}
}


void moveProjectile(Projectile proj[], float dt){

	for(int i = 0; i < PROJECTILE_COUNT; i++){

		if(!(proj[i].x < 0 || proj[i].x > SCREEN_WIDTH || proj[i].y < 0 || proj[i].y > SCREEN_HEIGHT)){

			proj[i].x += proj[i].dx * proj[i].speed * dt;
			proj[i].y += proj[i].dy * proj[i].speed * dt;

			proj_destination[i].x = proj[i].x;
			proj_destination[i].y = proj[i].y;


		}else {

			proj[i].isAlive = 0;

		}


	}

}

void moveReward(Obstacle *reward, float dt){

	if(reward->y + REWARD_HEIGHT < SCREEN_HEIGHT){

		reward->y += reward->speed * dt;

		reward_destination.x = reward->x;
		reward_destination.y = reward->y;

	}else{
		reward->isAlive = 0;
	}




}

void checkCollisions(Player *ship, Obstacle aster[], Projectile proj[], Obstacle *reward){


	for(int i = 0; i < ASTEROID_COUNT; i++){


		//Circle detection - asteroid and ship
		if(aster[i].isAlive && aster[i].x + aster[i].r >= ship->x - ship->r && aster[i].x - aster[i].r <= ship->x + ship->r  && 
				aster[i].y + aster[i].r >= ship->y - ship->r && aster[i].y - aster[i].r <= ship->y + ship->r){
			ship->isAlive = 0;
			aster[i].isAlive = 0;
			gameRunning = 0;	
		}



		//Circle detection - asteroid and projectile
		for(int j = 0; j < PROJECTILE_COUNT; j++){

			if(proj[j].isAlive && aster[i].isAlive && aster[i].x + aster[i].r >= proj[j].x && aster[i].x - aster[i].r <= proj[j].x + PROJECTILE_WIDTH &&
					aster[i].y + aster[i].r >= proj[j].y && aster[i].y - aster[i].r <= proj[j].y + PROJECTILE_HEIGHT){


				aster[i].isAlive = 0;
				proj[j].isAlive = 0;
				score++; //not sure


			}

		}

	}


	if(reward->isAlive && ship->isAlive && reward->x + reward->r >= ship->x - ship->r && reward->x - reward->r <= ship->x + ship->r &&
			reward->y + reward->r >= ship->y - ship->r && reward->y - reward->r <= ship->y + ship->r){

		reward->isAlive = 0;
		score += 20;

	}


}


void gameOverMenu(Texture2D *background, float *backgroundY, float *backgroundSpeed){

	if(score >= highscore){
		highscore = score;
	}

	BeginDrawing();

	//ClearBackground(BLACK);

	*backgroundY += *backgroundSpeed * GetFrameTime(); // S = V*T
			
	*backgroundY = fmodf(*backgroundY, (float)background->height);

	DrawTexture(*background, 0, (int)*backgroundY, WHITE);
	DrawTexture(*background, 0, (int)*backgroundY - background->height, WHITE);



	DrawText("GAME OVER!!", 80, 50, 100, WHITE);

	DrawText("Score: ", 200, 200, 50, WHITE);
	DrawText(TextFormat("%u", score), 600, 200, 50, WHITE);

	DrawText("High Score: ", 200, 250, 50, WHITE);
	DrawText(TextFormat("%u", highscore), 600, 250, 50, WHITE);


	DrawText("Press R To Restart", 240, 400, 30, WHITE);

	EndDrawing();

}


int main(void){

	float rotation;

	float backgroundY = 0.0f;
	float backgroundSpeed = 40.0f;

	//INIT WINDOW
	InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "SpaceGame"); //init
	SetTargetFPS(60); //set fps


	//INIT OBJECTS
	Player ship;

	Obstacle aster[ASTEROID_COUNT];

	Projectile proj[PROJECTILE_COUNT];

	Obstacle reward;

	setGameStart(&ship, aster, proj, &reward);



	//Load Textures
	Texture2D background = LoadTexture("img/background6.png");

	Texture2D ship_png = LoadTexture("img/ship.png");
	ship_source = (Rectangle){0, 0, ship_png.width, ship_png.height};

	Texture2D asteroid_png = LoadTexture("img/asteroid.png");
	aster_source = (Rectangle){0, 0, asteroid_png.width, asteroid_png.height};

	Texture2D proj_png = LoadTexture("img/laser.png");
	proj_source = (Rectangle){0, 0, proj_png.width, proj_png.height};

	Texture2D rew_png = LoadTexture("img/reward.png");
	reward_source = (Rectangle) {0, 0, rew_png.width, rew_png.height};



	while(!WindowShouldClose()){


		if(gameRunning == 0){

			
			gameOverMenu(&background, &backgroundY, &backgroundSpeed);

			if(IsKeyPressed(KEY_R)){
				gameRunning = 1;
				setGameStart(&ship, aster, proj, &reward);
			}


			


		}else if(gameRunning){

			rotation += 5;

			BeginDrawing(); //2ri init na samoto risuwane

			ClearBackground(BLACK);//set background color
			
			backgroundY += (backgroundSpeed + score * DIFFICULTY_INDEX) * GetFrameTime(); // S = V*T
			
			backgroundY = fmodf(backgroundY, (float)background.height); //towa prawi copy 1 da se wurne ot nachalo, a copy 2 da zastane pak nad nego; ne razbiram bash kak stawa, no copy 1 winagi trugwa ot nachaloto do kraq, a ne si smenqt mestata s copy 2


			DrawTexture(background, 0, (int)backgroundY, WHITE);
			DrawTexture(background, 0, (int)backgroundY - background.height, WHITE);

			//spawn asteroids
			//move ship
			//check for shooting
			//move proj
			//move asteroids
			//check collision

			spawnObstacles(aster, &reward);

			//dt is the delta time, made to make movement independent of FPS, get time since last frame:
			dt = GetFrameTime();

			moveShip(&ship, dt, &score);

			shootWeapon(proj, &ship);

			moveProjectile(proj, dt);

			moveAsteroids(aster, dt, &score);

			moveReward(&reward, dt);

			checkCollisions(&ship, aster, proj, &reward);


			//Draw Ship
			//hitbox
			//DrawCircle(ship.x, ship.y, ship.r, WHITE);
			DrawTexturePro(ship_png, ship_source, ship_destination, (Vector2){(float)SHIP_WIDTH/2, (float)SHIP_HEIGHT/2}, ship.angle,  WHITE);


			//Draw Asteroids
			for(int i = 0; i < ASTEROID_COUNT; i++){

				if(aster[i].isAlive){

					//hitbox
					//DrawCircle(aster[i].x, aster[i].y, aster[i].r, WHITE);	
					DrawTexturePro(asteroid_png, aster_source, aster_destination[i], (Vector2){(float)ASTEROID_WIDTH/2, (float)ASTEROID_HEIGHT/2}, 0.0f, WHITE);


				}
			}


			if(reward.isAlive){

				//Draw Reward
				//DrawCircle(reward.x, reward.y, reward.r, WHITE);	
				DrawTexturePro(rew_png, reward_source, reward_destination, (Vector2){(float)REWARD_WIDTH/2, (float)REWARD_HEIGHT/2}, rotation, WHITE);	

			}


			//Draw Projectile
			for(int i = 0; i < PROJECTILE_COUNT; i++){

				if(proj[i].isAlive){

					//hitbox
					//DrawRectangle(proj[i].x, proj[i].y, proj[i].width, proj[i].height, WHITE);
					DrawTexturePro(proj_png, proj_source, proj_destination[i], (Vector2){0, 0}, proj[i].angle,  WHITE);

				}
			}



			//Draw Score
			DrawText("Score: ", 650, 5, 20, WHITE);
			DrawText(TextFormat("%u", score), 750, 5, 20, WHITE);

			EndDrawing(); //zatwarqne na chetkata

		}




	}

	UnloadTexture(background);
	UnloadTexture(ship_png);
	UnloadTexture(asteroid_png);
	UnloadTexture(proj_png);
	UnloadTexture(rew_png);
	CloseWindow();



	return 0;

}

