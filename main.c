#include <complex.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <time.h> //for srand

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
#define REWARD_COOLDOWN 10.0f

#define SHIELD_COOLDOWN 5.0f
#define SHIELD_RADIUS 35.0f


#define IMPACT_DURATION 0.5f

//check:
//gymnasium frama foundation
//hugging face lerobot


float dt;

bool gameMode = 0;

unsigned int score = 0;
unsigned int highscore = 0;

float rewardNow;
float rewardLastTime = 0;
float rewardCooldown = REWARD_COOLDOWN;

float shieldNow;
float shieldLastTime = 0;
float shieldCooldown = SHIELD_COOLDOWN;

float impactTime = 0.0f;


typedef enum {

	GAME_RUNNING, 
	GAME_MENU, 
	GAME_OVER, 
	GAME_INTRO 

} GameState;


//circle
typedef struct {

	float x;
	float y;

	int r;

	float speed;

	bool isAlive;

	float angle;

	bool hasShield;

} Player;


//circle
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

	score = 0;
	rewardLastTime = GetTime();

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
	ship->hasShield = 0;
	ship->speed = 380.0f;

	// 	for rect ship	
	//	ship->x =(((float)SCREEN_WIDTH/2 - (float)SHIP_WIDTH/2) + (float)SHIP_WIDTH/4); // 400 - 60 + 15
	//	ship->y = (float)SCREEN_HEIGHT/2 - (float)SHIP_HEIGHT/2;

	ship->x = (float)SCREEN_WIDTH/2;
	ship->y = 400;

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
		proj[i].speed = 800.0f;

		proj[i].x = ship->x;
		proj[i].y = ship->y;

		proj[i].angle = 0;
		proj[i].dx = 0;
		proj[i].dy = 0;

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

	shieldNow = GetTime();


	if(shieldNow - shieldLastTime >= SHIELD_COOLDOWN && ship->hasShield){

		ship->hasShield = 0;

		shieldLastTime = shieldNow;

	}


	float dx = 0.0f;
	float dy = 0.0f;


	if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){

		if(ship->x - ship->r - 20 > 0){

			dx -= 1.0f;

		}
	}



	if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){

		if(ship->x + ship->r + 20 < SCREEN_WIDTH){

			dx += 1.0f;
		}

	}



	if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){

		if(ship->y + ship->r + 20 < SCREEN_HEIGHT){

			dy += 1.0f;

		}


	}



	if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)){

		if(ship->y - ship->r - 20 > 0){

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


	if(gameMode == 1){

		if(!(dx == 0 && dy == 0)){

			ship->angle = (atan2(dx, -dy)) * 180.0 / PI;

		}

	}else if(gameMode == 0){

		ship->angle = 0;

	}




}


void spawnObstacles(Obstacle aster[], Obstacle *reward){

	rewardNow = GetTime();

	if(rewardNow - rewardLastTime >= rewardCooldown && reward->isAlive == 0){

		reward->isAlive = 1;

		reward->x = rand() % SCREEN_WIDTH;
		reward->y = -(20 + REWARD_HEIGHT + rand() % 10); 

		rewardLastTime = rewardNow;
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

		if(aster[i].y - ASTEROID_HEIGHT  < SCREEN_HEIGHT){

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

		if(!(proj[i].x < 0 || proj[i].x > SCREEN_WIDTH || proj[i].y < 0 || proj[i].y > SCREEN_HEIGHT) && proj[i].isAlive){

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

	if(reward->y - REWARD_HEIGHT < SCREEN_HEIGHT){

		reward->y += reward->speed * dt;

		reward_destination.x = reward->x;
		reward_destination.y = reward->y;

	}else{
		reward->isAlive = 0;
	}




}

void checkCollisions(Player *ship, Obstacle aster[], Projectile proj[], Obstacle *reward, GameState *gameState){

	float dx, dy, combinedRadius;


	for(int i = 0; i < ASTEROID_COUNT; i++){

		//Circle and rect detection - asteroid and projectile
		for(int j = 0; j < PROJECTILE_COUNT; j++){

			if(proj[j].isAlive && aster[i].isAlive && aster[i].x + aster[i].r >= proj[j].x && aster[i].x - aster[i].r <= proj[j].x + PROJECTILE_WIDTH &&
					aster[i].y + aster[i].r >= proj[j].y && aster[i].y - aster[i].r <= proj[j].y + PROJECTILE_HEIGHT){


				aster[i].isAlive = 0;
				proj[j].isAlive = 0;
				score++;


			}

		}




		//Circle detection - asteroid and ship
		dx = aster[i].x - ship->x; // a
		dy = aster[i].y - ship->y; // b
		combinedRadius = aster[i].r + ship->r; // c


		if(ship->hasShield){

			combinedRadius = aster[i].r + SHIELD_RADIUS; //shield radius

			if(aster[i].isAlive && (dx * dx + dy * dy <= combinedRadius * combinedRadius)){

				aster[i].isAlive = 0;
				ship->hasShield = 0;

			}


		} else if(aster[i].isAlive && (dx * dx + dy * dy <= combinedRadius * combinedRadius)){ // a^2 + b^2 = c^2, pythagorean theorem


			ship->isAlive = 0;

			for(int k = 0; k < ASTEROID_COUNT; k++){

				if(k != i)
					aster[k].isAlive = 0; //remove every asteroid except the one that hit the ship

			}

			reward->isAlive = 0;

			impactTime = IMPACT_DURATION; //set impact time to 0.5f

			return;

			//*gameState = GAME_OVER;    // will be set in main	

		}

	}


	//cicle detection for ship and reward, again with the pythegoren method
	dx = reward->x  - ship->x;
	dy = reward->y - ship->y;

	combinedRadius = reward->r + ship->r;


	if(reward->isAlive && ship->isAlive && (dx * dx + dy *dy <= combinedRadius * combinedRadius)){ //a^2 + b^2 = c^2

		reward->isAlive = 0;
		ship->hasShield = 1;
		shieldLastTime = GetTime();

	}


}


void gameMenu(Texture2D *background, float *backgroundY, float *backgroundSpeed){


	if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)){

		gameMode = !gameMode;

	}

	BeginDrawing();

	*backgroundY += *backgroundSpeed * GetFrameTime(); // S = V*T

	*backgroundY = fmodf(*backgroundY, (float)background->height);

	DrawTexture(*background, 0, (int)*backgroundY, WHITE);
	DrawTexture(*background, 0, (int)*backgroundY - background->height, WHITE);


	DrawText("GAME MENU", 80, 50, 100, WHITE);

	DrawText("DIFFICULTY: ", 120, 200, 50,  WHITE);

	if(gameMode == 0){

		DrawText(TextFormat("EASY"), 520, 200, 50, WHITE);

	}else{

		DrawText(TextFormat("HARD"), 520, 200, 50, WHITE);

	}


	//up arrow
	DrawLineEx((Vector2){680, 210}, (Vector2){690, 195}, 3, WHITE);
	DrawLineEx((Vector2){700, 210}, (Vector2){690, 195}, 3, WHITE);

	// down arrow
	DrawLineEx((Vector2){680, 230}, (Vector2){690, 245}, 3, WHITE);
	DrawLineEx((Vector2){690, 245}, (Vector2){700, 230}, 3, WHITE);


	DrawText("WASD - MOVE", 220, 300, 20, WHITE);
	DrawText("M - MENU", 480, 300, 20, WHITE);

	DrawText("SPACE - SHOOT", 220, 330, 20, WHITE);
	DrawText("R/Enter - Restart", 480, 330, 20, WHITE);

	DrawText("Press M To Return", 240, 400, 30, WHITE);



	EndDrawing();


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

	DrawText("High Score: ", 200, 280, 50, WHITE);
	DrawText(TextFormat("%u", highscore), 600, 280, 50, WHITE);

	DrawText("Press R To Restart", 240, 380, 30, WHITE);
	DrawText("Press M For MENU", 280, 430, 25, WHITE);

	EndDrawing();

}


int main(void){

	srand(time(NULL));

	float rotation = 0.0f;

	float introTime = 0.0f;
	float alpha = introTime;
	Color textColor;

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


	//game state
	GameState gameState = GAME_INTRO;
	GameState previousState = gameState;





	while(!WindowShouldClose()){
		
		//dt is the delta time, made to make movement independent of FPS, get time since last frame:
		dt = GetFrameTime();


		if((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_M)) && gameState == GAME_MENU){

			gameState = previousState;

		}else if(IsKeyPressed(KEY_M) && (gameState == GAME_RUNNING || gameState == GAME_OVER)){

			previousState = gameState;
			gameState = GAME_MENU;

		}




		switch (gameState) {

			case GAME_INTRO:



				introTime += dt;

				alpha = introTime;

				if(alpha > 1.0f)
					alpha = 1.0f;

				textColor = Fade(WHITE, alpha);

				BeginDrawing();

				backgroundY += backgroundSpeed * GetFrameTime(); // S = V*T

				backgroundY = fmodf(backgroundY, (float)background.height);

				DrawTexture(background, 0, (int)backgroundY, WHITE);
				DrawTexture(background, 0, (int)backgroundY - background.height, WHITE);	



				DrawText("WASD - MOVE", 100, 70, 50, textColor);

				if (introTime >= 0.5f){
					DrawText("SPACE - SHOOT", 100, 160, 50, textColor);
				}

				if (introTime >= 1.0f){
					DrawText("R/Enter - Restart", 100, 250, 50, textColor);
				}

				if (introTime >= 1.5f){
					DrawText("M - Menu", 100, 340, 50, textColor);
				}

				DrawText("Enter To START", 540, 440, 25, YELLOW);

				EndDrawing();

				if(IsKeyPressed(KEY_ENTER)){
					gameState = GAME_RUNNING;
				}


				break;




			case GAME_RUNNING:

				rotation += 300.0f * dt;


				//spawn asteroids
				//move ship
				//check for shooting
				//move proj
				//move asteroids
				//check collision


				if(ship.isAlive){

					spawnObstacles(aster, &reward);

					moveShip(&ship, dt, &score);

					shootWeapon(proj, &ship);

					moveProjectile(proj, dt);

					moveAsteroids(aster, dt, &score);

					moveReward(&reward, dt);

					checkCollisions(&ship, aster, proj, &reward, &gameState);
				}else{

					impactTime -= dt; //decrease impact time

					if(impactTime <= 0.0f){ //clmp the impact time, so its not below 0


						impactTime = 0.0f;
						gameState = GAME_OVER; //change game state here

					}

				}


				BeginDrawing(); //2ri init na samoto risuwane

				ClearBackground(BLACK);//set background color

				backgroundY += (backgroundSpeed + score * DIFFICULTY_INDEX) * GetFrameTime(); // S = V*T

				backgroundY = fmodf(backgroundY, (float)background.height); //towa prawi copy 1 da se wurne ot nachalo, a copy 2 da zastane pak nad nego; ne razbiram bash kak stawa, no copy 1 winagi trugwa ot nachaloto do kraq, a ne si smenqt mestata s copy 2


				DrawTexture(background, 0, (int)backgroundY, WHITE);
				DrawTexture(background, 0, (int)backgroundY - background.height, WHITE);


				if (ship.isAlive || (int)(impactTime * 10) % 2 == 0) { //if ship is alive, c even skips the second part of the condition. when it is not alive, it starts checking if the num is odd or even for flickering effect


					//Draw Ship
					//hitbox
					//DrawCircle(ship.x, ship.y, ship.r, WHITE);
					DrawTexturePro(ship_png, ship_source, ship_destination, (Vector2){(float)SHIP_WIDTH/2, (float)SHIP_HEIGHT/2}, ship.angle,  WHITE);


					//Draw shield
					if(ship.hasShield){

						DrawCircleV((Vector2) {ship.x, ship.y}, SHIELD_RADIUS, Fade(SKYBLUE, 0.15f));

						DrawCircleLinesV((Vector2) {ship.x, ship.y}, SHIELD_RADIUS, Fade(SKYBLUE, 0.7f));

					}


					//Draw Asteroids
					for(int i = 0; i < ASTEROID_COUNT; i++){

						if(aster[i].isAlive){

							//hitbox
							//DrawCircle(aster[i].x, aster[i].y, aster[i].r, WHITE);	
							DrawTexturePro(asteroid_png, aster_source, aster_destination[i], (Vector2){(float)ASTEROID_WIDTH/2, (float)ASTEROID_HEIGHT/2}, 0.0f, WHITE);


						}
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


				break;


			case GAME_MENU: 

				gameMenu(&background, &backgroundY, &backgroundSpeed);

				break;



			case GAME_OVER:

				gameOverMenu(&background, &backgroundY, &backgroundSpeed);

				if(IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER)){

					setGameStart(&ship, aster, proj, &reward);
					gameState = GAME_RUNNING;
				}

				break;

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
