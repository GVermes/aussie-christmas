#include <SDL.h>
#include <SDL_image.h>//load img and processing functions. Supports lots of image types e.g png,jpg
#include <SDL_ttf.h> //loads up ttf font files and helps render text to texture
#include <SDL_mixer.h> //for music and sound
#include <iostream>
#include <string>
#include "Animation.h"
#include "GameObject.h"
#include "Hero.h"
#include "Wall.h"
#include <list>
#include "KeyboardHandler.h"
#include "GameControllerHandler.h"
#include <stdlib.h> //rand and srand
#include <time.h> //get milliseconds since start of year
#include "SoundManager.h"
#include "Globals.h"
#include "Chaser.h"
#include "Item.h"

using namespace std;

//create new item
Item* createItem(SDL_Texture* texture, int x, int y, string type, SDL_Renderer* renderer) {
	Item* item = new Item();
	item->texture = texture;
	item->type = type;
	item->collisionBox.x = x;
	item->collisionBox.y = y;
	item->renderer = renderer;

	return item;
}

//create new wall
Wall* createWall(int x, int y, int w, int h, SDL_Renderer* renderer) {
	Wall* wall = new Wall();
	wall->pos.x = x;
	wall->pos.y = y;
	wall->w = w;
	wall->h = h;
	wall->renderer = renderer;

	return wall;
}


void renderText(TTF_Font* font, SDL_Color textColour, string text, SDL_Renderer* renderer, SDL_Texture* &texture) {
	//clean up memory, before rebuild
	if (texture != NULL)
		SDL_DestroyTexture(texture);
	
	//create surface(image in ram) using font, colour and text
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColour);
	//convert surface to texture
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	//cleanup surface
	SDL_FreeSurface(textSurface);
}

//for SDL to work
int main(int argc, char **argv)
{
	//SET random number series seed value (changes which set of random numbers are returned from rand)
	srand(time(NULL));//time(NULL) gives milliseconds since the start of this year, so should be different each game run

	//initialise SDL and its subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "ERROR: " << SDL_GetError() << endl;
		system("pause");
		return -1; //exit program early
	}
	else
	{
		cout << "SDL initialised!!!" << endl;
	}
	//Initialise SDL image
	if (!(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)))
	{
		//if didn't work, report error
		cout << "SDL IMAGE ERROR: " << IMG_GetError() << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	if (TTF_Init() != 0)
	{
		//failed!
		cout << "TTF FAILED: " << TTF_GetError() << endl;
		system("pause");
		SDL_Quit();
		return -1;
	}

	//mixer init
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		cout << "mixer error: " << Mix_GetError() << endl;
		system("pause");
		SDL_Quit();
		return -1;
	}

	//params: 
	//	window title, x and y pos, width, height, flags for creation
	SDL_Window* window = SDL_CreateWindow("AussieXmas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Globals::screenWidth, Globals::screenHeight, SDL_WINDOW_SHOWN);//SDL_WINDOW_FULLSCREEN

	if (window != NULL) {
		cout << "Game ON!" << endl;
	}
	else
	{
		cout << "ERROR: " << SDL_GetError() << endl;
		system("pause");
		SDL_Quit(); //cleanup SDL memory
		return -1; //exit the program
	}

	//create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		cout << "ERROR: " << SDL_GetError() << endl;
		system("pause");
		SDL_Quit(); //cleanup SDL memory
		return -1; //exit the program
	}

	
	//so we can access window/renderer anywhere within our project without passing them via paramater 
	Globals::window = window;
	Globals::renderer = renderer;

	//LOAD UP ASSETS TO DRAW
	SDL_Surface* beerSurface = IMG_Load("assets/beer.jpg");
	SDL_Surface* presentSurface = IMG_Load("assets/present.png");

	SDL_Texture* beerTexture = SDL_CreateTextureFromSurface(renderer, beerSurface);
	SDL_Texture* presentTexture = SDL_CreateTextureFromSurface(renderer, presentSurface);

	SDL_FreeSurface(beerSurface);
	SDL_FreeSurface(presentSurface);

	//LOAD run surface up so we can colour key
	SDL_Surface* runSurface = IMG_Load("assets/santa2.png");
	//set colour key( this colour goes fully transparent)
	//params: surface, set or unset(1/0), uint for colour (using mapRGB to find index of this colour in image)
	SDL_SetColorKey(runSurface, 1, SDL_MapRGB(runSurface->format, 255, 255, 255));
	//turn into texture
	SDL_Texture* runSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, runSurface);
	//clean up surface
	SDL_FreeSurface(runSurface);

	SDL_Surface* runSurface2 = IMG_Load("assets/k2.png");
	SDL_SetColorKey(runSurface2, 1, SDL_MapRGB(runSurface2->format, 255, 255, 255));
	SDL_Texture* runSpriteSheetWithNoBG2 = SDL_CreateTextureFromSurface(renderer, runSurface2);
	SDL_FreeSurface(runSurface2);

	Animation anim2(runSpriteSheetWithNoBG, renderer, 4, 57, 94, 0.15);
	Animation anim3(runSpriteSheetWithNoBG2, renderer, 4, 91, 77, 0.15);

	//load up 'welcome' screen
	SDL_Surface* kangarooSurface = IMG_Load("assets/boxingkangaroo.jpg");
	SDL_Surface* koalaSurface = IMG_Load("assets/koala2.jpg");
	SDL_Surface* santaSurface = IMG_Load("assets/aussiesanta.jpg");
	SDL_Surface* decorSurface = IMG_Load("assets/decor.jpg");
	SDL_Surface* presentPicSurface = IMG_Load("assets/present.png");
	SDL_Surface* beerPicSurface = IMG_Load("assets/beer.jpg");

	SDL_Texture* kangarooTexture = SDL_CreateTextureFromSurface(renderer, kangarooSurface);
	SDL_Texture* koalaTexture = SDL_CreateTextureFromSurface(renderer, koalaSurface);
	SDL_Texture* santaTexture = SDL_CreateTextureFromSurface(renderer, santaSurface);
	SDL_Texture* decorTexture = SDL_CreateTextureFromSurface(renderer, decorSurface);
	SDL_Texture* presentPicTexture = SDL_CreateTextureFromSurface(renderer, presentPicSurface);
	SDL_Texture* beerPicTexture = SDL_CreateTextureFromSurface(renderer, beerPicSurface);

	SDL_FreeSurface(kangarooSurface);
	SDL_FreeSurface(koalaSurface);
	SDL_FreeSurface(santaSurface);
	SDL_FreeSurface(decorSurface);
	SDL_FreeSurface(presentPicSurface);
	SDL_FreeSurface(beerPicSurface);

	SDL_Rect kangarooSourceRectangle;
	SDL_Rect koalaSourceRectangle;
	SDL_Rect santaSourceRectangle;
	SDL_Rect decorSourceRectangle;
	SDL_Rect presentPicSourceRectangle;
	SDL_Rect beerPicSourceRectangle;

	SDL_QueryTexture(kangarooTexture, NULL, NULL, &kangarooSourceRectangle.w, &kangarooSourceRectangle.h);
	kangarooSourceRectangle.x = 0;
	kangarooSourceRectangle.y = 0;
	kangarooSourceRectangle.w;
	kangarooSourceRectangle.h;

	SDL_Rect kangarooDestinationRectangle;
	kangarooDestinationRectangle.x = 850;
	kangarooDestinationRectangle.y = 200;
	kangarooDestinationRectangle.w = kangarooSourceRectangle.w;
	kangarooDestinationRectangle.h = kangarooSourceRectangle.h;

	SDL_QueryTexture(koalaTexture, NULL, NULL, &koalaSourceRectangle.w, &koalaSourceRectangle.h);
	koalaSourceRectangle.x = 0;
	koalaSourceRectangle.y = 0;
	koalaSourceRectangle.w;
	koalaSourceRectangle.h;

	SDL_Rect koalaDestinationRectangle;
	koalaDestinationRectangle.x = 570;
	koalaDestinationRectangle.y = 420;
	koalaDestinationRectangle.w = koalaSourceRectangle.w;
	koalaDestinationRectangle.h = koalaSourceRectangle.h;

	SDL_QueryTexture(santaTexture, NULL, NULL, &santaSourceRectangle.w, &santaSourceRectangle.h);
	santaSourceRectangle.x = 0;
	santaSourceRectangle.y = 0;
	santaSourceRectangle.w;
	santaSourceRectangle.h;

	SDL_Rect santaDestinationRectangle;
	santaDestinationRectangle.x = 230;
	santaDestinationRectangle.y = 200;
	santaDestinationRectangle.w = santaSourceRectangle.w;
	santaDestinationRectangle.h = santaSourceRectangle.h;

	SDL_QueryTexture(decorTexture, NULL, NULL, &decorSourceRectangle.w, &decorSourceRectangle.h);
	decorSourceRectangle.x = 0;
	decorSourceRectangle.y = 0;
	decorSourceRectangle.w;
	decorSourceRectangle.h;

	SDL_Rect decorDestinationRectangle;
	decorDestinationRectangle.x = 550;
	decorDestinationRectangle.y = 0;
	decorDestinationRectangle.w = decorSourceRectangle.w;
	decorDestinationRectangle.h = decorSourceRectangle.h;

	SDL_QueryTexture(presentPicTexture, NULL, NULL, &presentPicSourceRectangle.w, &presentPicSourceRectangle.h);
	presentPicSourceRectangle.x = 0;
	presentPicSourceRectangle.y = 0;
	presentPicSourceRectangle.w;
	presentPicSourceRectangle.h;

	SDL_Rect presentDestinationRectangle;
	presentDestinationRectangle.x = 1040;
	presentDestinationRectangle.y = 600;
	presentDestinationRectangle.w = presentPicSourceRectangle.w / 4;
	presentDestinationRectangle.h = presentPicSourceRectangle.h / 4;

	SDL_QueryTexture(beerPicTexture, NULL, NULL, &beerPicSourceRectangle.w, &beerPicSourceRectangle.h);
	beerPicSourceRectangle.x = 0;
	beerPicSourceRectangle.y = 0;
	beerPicSourceRectangle.w;
	beerPicSourceRectangle.h;

	SDL_Rect beerPicDestinationRectangle;
	beerPicDestinationRectangle.x = 150;
	beerPicDestinationRectangle.y = 500;
	beerPicDestinationRectangle.w = beerPicSourceRectangle.w / 4;
	beerPicDestinationRectangle.h = beerPicSourceRectangle.h / 4;


	//Load up FONT file
	TTF_Font* font4 = TTF_OpenFont("assets/vermin_vibes_1989.ttf", 80);
	SDL_Color textColour4 = { 0, 100, 0, 0 };
	SDL_Color textColour2 = { 153, 0, 0, 0 };
	SDL_Color textColour3 = { 0, 0, 0, 0 };

	SDL_Surface* textSurface4 = TTF_RenderText_Blended(font4, "Aussie", textColour4);
	SDL_Surface* textSurface2 = TTF_RenderText_Blended(font4, "Xmas", textColour2);
	SDL_Surface* textSurface3 = TTF_RenderText_Blended(font4, "Welcome", textColour3);

	SDL_Texture* picTextTexture = SDL_CreateTextureFromSurface(renderer, textSurface4);
	SDL_Texture* picTextTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
	SDL_Texture* picTextTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);

	SDL_FreeSurface(textSurface4);
	SDL_FreeSurface(textSurface2);
	SDL_FreeSurface(textSurface3);

	//Text destination for 'welcome screen'
	SDL_Rect picTextDest;
	picTextDest.x = 340;
	picTextDest.y = 100;

	SDL_QueryTexture(picTextTexture, NULL, NULL, &picTextDest.w, &picTextDest.h);

	SDL_Rect picTextDest2;
	picTextDest2.x = 750;
	picTextDest2.y = 100;

	SDL_QueryTexture(picTextTexture2, NULL, NULL, &picTextDest2.w, &picTextDest2.h);

	SDL_Rect picTextDest3;
	picTextDest3.x = 500;
	picTextDest3.y = 270;

	SDL_QueryTexture(picTextTexture3, NULL, NULL, &picTextDest3.w, &picTextDest3.h);

	//SETUP INITIAL GAME OBJECTS
	list<GameObject*> gameObjects;
	//give this list reference to the game objects static variable
	GameObject::gameObjects = &gameObjects;

	//items -> add to the list
	gameObjects.push_back(createItem(presentTexture, 180, 550, "present", renderer));
	gameObjects.push_back(createItem(presentTexture, 300, 70, "present", renderer));
	gameObjects.push_back(createItem(presentTexture, 800, 100, "present", renderer));
	gameObjects.push_back(createItem(presentTexture, 600, 270, "present", renderer));
	gameObjects.push_back(createItem(presentTexture, 1130, 450, "present", renderer));
	gameObjects.push_back(createItem(presentTexture, 400, 450, "present", renderer));
	gameObjects.push_back(createItem(presentTexture, 640, 630, "present", renderer));
	gameObjects.push_back(createItem(beerTexture, 1100, 80, "beer", renderer));
	gameObjects.push_back(createItem(beerTexture, 100, 300, "beer", renderer));
	gameObjects.push_back(createItem(beerTexture, 800, 470, "beer", renderer));

	//build a hero
	Hero* hero = new Hero();
	hero->animation = &anim2;
	hero->pos.x = 50;
	hero->pos.y = 50;
	hero->renderer = renderer;
	//add to the list
	gameObjects.push_back(hero);

	//walls -> add to the list
	gameObjects.push_back(createWall(0, 0, 1280, 20, renderer));
	gameObjects.push_back(createWall(0, 0, 20, 720, renderer));
	gameObjects.push_back(createWall(1260, 0, 20, 720, renderer));
	gameObjects.push_back(createWall(0, 700, 1280, 20, renderer));
	gameObjects.push_back(createWall(500, 360, 260, 10, renderer));
	gameObjects.push_back(createWall(300, 200, 10, 100, renderer));
	gameObjects.push_back(createWall(300, 200, 200, 10, renderer));
	gameObjects.push_back(createWall(700, 200, 200, 10, renderer));
	gameObjects.push_back(createWall(900, 200, 10, 100, renderer));
	gameObjects.push_back(createWall(300, 450, 10, 100, renderer));
	gameObjects.push_back(createWall(300, 550, 200, 10, renderer));
	gameObjects.push_back(createWall(700, 550, 200, 10, renderer));
	gameObjects.push_back(createWall(900, 450, 10, 110, renderer));

	//build Chaser -> Jack the kangaroo
	Chaser* chaser = new Chaser();
	chaser->renderer = renderer;
	chaser->pos.x = 1100;
	chaser->pos.y = 600;
	chaser->animation = &anim3;
	chaser->target = hero;
	//add to the list
	gameObjects.push_back(chaser);

	//INPUT HANDLERS
	KeyboardHandler keyboardHandler;
	keyboardHandler.hero = hero;

	GameControllerHandler gameControllerHandler;
	gameControllerHandler.hero = hero;

	//Load up FONT file
	TTF_Font* font1 = TTF_OpenFont("assets/vermin_vibes_1989.ttf", 23);//params: file path, font size
	SDL_Color textColour = { 123, 0, 34, 0 };

	//create surface(image in ram) using font, colour and text
	SDL_Surface* textSurface = TTF_RenderText_Blended(font1, " ", textColour);
	//convert surface to texture
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	//cleanup surface
	SDL_FreeSurface(textSurface);

	//Text destination
	SDL_Rect textDest;
	textDest.x = 30;
	textDest.y = 30;
	//query texture for width and height
	SDL_QueryTexture(textTexture, NULL, NULL, &textDest.w, &textDest.h);

	TTF_Font* font2 = TTF_OpenFont("assets/vermin_vibes_1989.ttf", 100);

	SDL_Surface* gameOverTextSurface = TTF_RenderText_Blended(font2, "GAME OVER", textColour);
	//convert surface to texture
	SDL_Texture* gameOverTextTexture = SDL_CreateTextureFromSurface(renderer, gameOverTextSurface);
	//cleanup surface
	SDL_FreeSurface(gameOverTextSurface);

	SDL_Rect gameOverTextDest;
	gameOverTextDest.x = 360;
	gameOverTextDest.y = 100;
	//query texture for width and height
	SDL_QueryTexture(gameOverTextTexture, NULL, NULL, &gameOverTextDest.w, &gameOverTextDest.h);


	//load up our music
	Mix_Music* music = Mix_LoadMUS("assets/music1.ogg");
	if (music == NULL)
	{
		cout << "Music load failed: " << Mix_GetError() << endl;
		system("pause");
		SDL_Quit();
		return -1;
	}
	//play music
	//params: music to play -> how many times to loop the song (0 means play once with no repeats, -1 means play infinite times)
	Mix_PlayMusic(music, -1);

	//Load up sound effects
	SoundManager::soundManager.loadSound("coin", "assets/coin.wav");
	SoundManager::soundManager.loadSound("gameover", "assets/gameover.wav");

	//set last update to current number of milliseconds since start of game
	Uint32 lastUpdate = SDL_GetTicks();

	//set up level for the different game stages
	int level = 0;

	bool loop = true;
	//GAME LOOP
	while (loop) {
		//TIME MANAGEMENT
		//difference of current time minus the last update time
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		//convert timeDiff to deltaTime (timeDiff as a fraction of a second)
		float DT = timeDiff / 1000.0;//e.g timeDiff = 200ms, then 200/1000 = 0.2 for DT
		//we want to measure timeDiff between gameLoop iterations, so we need to update lastUpdate
		lastUpdate = SDL_GetTicks();

		//set render draw colour with RGBA value (values between 0-255)
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//clear the screen with current draw colour
		SDL_RenderClear(renderer);

		//show title screen for 5 sec before the game starts than go to next level
		if (level == 0) {
			SDL_RenderCopy(renderer, kangarooTexture, &kangarooSourceRectangle, &kangarooDestinationRectangle);
			SDL_RenderCopy(renderer, koalaTexture, &koalaSourceRectangle, &koalaDestinationRectangle);
			SDL_RenderCopy(renderer, santaTexture, &santaSourceRectangle, &santaDestinationRectangle);
			SDL_RenderCopy(renderer, decorTexture, &decorSourceRectangle, &decorDestinationRectangle);
			SDL_RenderCopy(renderer, presentPicTexture, &presentPicSourceRectangle, &presentDestinationRectangle);
			SDL_RenderCopy(renderer, beerPicTexture, &beerPicSourceRectangle, &beerPicDestinationRectangle);
			SDL_RenderCopy(renderer, picTextTexture, NULL, &picTextDest);
			SDL_RenderCopy(renderer, picTextTexture2, NULL, &picTextDest2);
			SDL_RenderCopy(renderer, picTextTexture3, NULL, &picTextDest3); 
			SDL_RenderPresent(renderer);
			SDL_Delay(5000);
			level++;
		}

		//point system left top corner
		string text = "Points = " + to_string(hero->getScore());
		renderText(font1, textColour, text, renderer, textTexture);

		//drawing text on top of everything else
		SDL_QueryTexture(textTexture, NULL, NULL, &textDest.w, &textDest.h);
		SDL_RenderCopy(renderer, textTexture, NULL, &textDest);

		//INPUT HANDLING
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			//check if user clicked close on window
			if (e.type == SDL_QUIT) {
				loop = false;
			}
			//check if user pressed a button
			if (e.type == SDL_KEYDOWN) {
				//see if ESC key pressed
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					loop = false;
				}
				
			}
			keyboardHandler.update(&e);
			gameControllerHandler.update(&e);
		}

		if (gameControllerHandler.controller == NULL)
			keyboardHandler.updateHeld();
		else
			gameControllerHandler.updateHeld();
		
		int noOfItems = 0;

		//UPDATE AND DRAW ALL GAME OBJECTS
		for each (GameObject* gObj in gameObjects)
		{
			gObj->update(DT);
			gObj->updateCollisions(DT);
			if (gObj->type == "beer" || gObj->type == "present") {
				noOfItems++;
			}
		}
		for each (GameObject* gObj in gameObjects)
		{
			gObj->draw();
		}
		if (hero->active == false) {
			// if Santa collided with enemy show game over wait 4 sec quit the app
			string gameOverText = "GAME OVER";
			renderText(font2, textColour, gameOverText, renderer, gameOverTextTexture);
			SDL_QueryTexture(gameOverTextTexture, NULL, NULL, &gameOverTextDest.w, &gameOverTextDest.h);
			SDL_RenderCopy(renderer, gameOverTextTexture, NULL, &gameOverTextDest);
			SDL_RenderPresent(renderer);
			SDL_Delay(4000);
			loop = false;
			break;
		}
		if (noOfItems == 0 && level == 1) {
			level++;
			//show level 2 on screen before the second stage starts 
			TTF_Font* font2 = TTF_OpenFont("assets/vermin_vibes_1989.ttf", 100);
			SDL_Surface* level2TextSurface = TTF_RenderText_Blended(font2, "LEVEL 2", textColour);
			SDL_Texture* level2TextTexture = SDL_CreateTextureFromSurface(renderer, level2TextSurface);
			SDL_FreeSurface(level2TextSurface);
			SDL_Rect level2TextDest;
			level2TextDest.x = 430;
			level2TextDest.y = 100;
			SDL_QueryTexture(gameOverTextTexture, NULL, NULL, &level2TextDest.w, &level2TextDest.h);
			string level2Text = "LEVEL 2";
			renderText(font2, textColour, level2Text, renderer, level2TextTexture);
			SDL_QueryTexture(level2TextTexture, NULL, NULL, &level2TextDest.w, &level2TextDest.h);
			SDL_RenderCopy(renderer, level2TextTexture, NULL, &level2TextDest);
			SDL_RenderPresent(renderer);
			//wait till level 2 upload to screen
			SDL_Delay(2500);

			//reser hero -> Santa pos
			hero->pos.x = 50;
			hero->pos.y = 50;

			//reset chaser -> Jack pos
			chaser->pos.x = 1100;
			chaser->pos.y = 600;

			//reset items as once was picked for new positions
			gameObjects.push_back(createItem(presentTexture, 80, 370, "present", renderer));
			gameObjects.push_back(createItem(presentTexture, 290, 620, "present", renderer));
			gameObjects.push_back(createItem(beerTexture, 800, 110, "beer", renderer));
			gameObjects.push_back(createItem(presentTexture, 900, 630, "present", renderer));
			gameObjects.push_back(createItem(presentTexture, 1130, 490, "present", renderer));
			gameObjects.push_back(createItem(presentTexture, 400, 260, "present", renderer));
			gameObjects.push_back(createItem(beerTexture, 580, 490, "beer", renderer));
			gameObjects.push_back(createItem(presentTexture, 1100, 90, "present", renderer));
			gameObjects.push_back(createItem(presentTexture, 330, 50, "present", renderer));
			gameObjects.push_back(createItem(presentTexture, 800, 270, "present", renderer));

			//creat second chaser -> Sam the koala for level 2
			SDL_Surface* runSurface3 = IMG_Load("assets/koala.png");
			SDL_SetColorKey(runSurface3, 1, SDL_MapRGB(runSurface3->format, 255, 255, 255));
			SDL_Texture* runSpriteSheetWithNoBG3 = SDL_CreateTextureFromSurface(renderer, runSurface3);
			SDL_FreeSurface(runSurface3);
			//new animation for 'Sam'
			Animation anim4(runSpriteSheetWithNoBG3, renderer, 4, 49.98, 68, 0.25);
			//set Sam's pos
			Chaser* chaser2 = new Chaser();
			chaser2->renderer = renderer;
			chaser2->pos.x = 90;
			chaser2->pos.y = 590;
			chaser2->animation = &anim4;
			chaser2->target = hero;
			gameObjects.push_back(chaser2);

		}
		else if (noOfItems == 0 && level == 2) {
			//all items collected? You win!
			TTF_Font* font2 = TTF_OpenFont("assets/vermin_vibes_1989.ttf", 100);
			SDL_Surface* youWonTextSurface = TTF_RenderText_Blended(font2, "YOU WON!", textColour);
			SDL_Texture* youWonTextTexture = SDL_CreateTextureFromSurface(renderer, youWonTextSurface);
			SDL_FreeSurface(youWonTextSurface);
			SDL_Rect youWonTextDest;
			youWonTextDest.x = 430;
			youWonTextDest.y = 100;
			SDL_QueryTexture(gameOverTextTexture, NULL, NULL, &youWonTextDest.w, &youWonTextDest.h);
			string level2Text = "YOU WON !";
			renderText(font2, textColour, level2Text, renderer, youWonTextTexture);
			SDL_QueryTexture(youWonTextTexture, NULL, NULL, &youWonTextDest.w, &youWonTextDest.h);
			SDL_RenderCopy(renderer, youWonTextTexture, NULL, &youWonTextDest);
			SDL_RenderPresent(renderer);

			SDL_Delay(1000);
			//show final score on screen and cout also!
			SDL_Surface* finalScoreSurface = TTF_RenderText_Blended(font1, "FINAL SCORE: ", textColour);
			SDL_Texture* finalScoreTexture = SDL_CreateTextureFromSurface(renderer, finalScoreSurface);
			SDL_FreeSurface(finalScoreSurface);
			SDL_Rect finalScoreDest;
			finalScoreDest.x = 260;
			finalScoreDest.y = 290;
			SDL_QueryTexture(finalScoreTexture, NULL, NULL, &finalScoreDest.w, &finalScoreDest.h);
			string finalScoretext = "FINAL SCORE: 20";
			renderText(font2, textColour, finalScoretext, renderer, finalScoreTexture);
			SDL_QueryTexture(finalScoreTexture, NULL, NULL, &finalScoreDest.w, &finalScoreDest.h);
			SDL_RenderCopy(renderer, finalScoreTexture, NULL, &finalScoreDest);
			SDL_RenderPresent(renderer);
			//wait before closing the game
			SDL_Delay(5000);
			loop = false;
		}
		//delete inactive game objects
		GameObject::deleteInactiveGameObjects();

		//swap the buffers, show our current frame we've been drawing
		SDL_RenderPresent(renderer);
	}

	//stop playing music
	Mix_PauseMusic();
	//delete music from memory
	Mix_FreeMusic(music);

	//CLEANUP
	TTF_CloseFont(font1);
	TTF_CloseFont(font2);
	
	SDL_DestroyTexture(runSpriteSheetWithNoBG);
	SDL_DestroyTexture(runSpriteSheetWithNoBG2);

	gameControllerHandler.close();
	SDL_DestroyTexture(textTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	//cout final score and a thank you msg
	cout << "YOUR FINAL SCORE: " << hero->getScore() << endl;
	cout << "Thank you for playing AussieXmass !" << endl;

	system("pause");
	return 0;
}

