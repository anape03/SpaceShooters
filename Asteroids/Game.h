#pragma once

#include "graphics.h"
#include "Ship.h"
#include "LifeOrb.h"
#include "config.h"

class Game {
	typedef enum { STATUS_START, STATUS_PLAY, STATUS_END } gameStatus;
	gameStatus status = STATUS_START;
	typedef enum { FAST, SLOW } gameSpeed;
	gameSpeed game_speed = FAST; // FAST: used for my laptop, SLOW: used for my PC

	Ship* ship = nullptr;
	LifeOrb* orb = nullptr;

	bool first_init; // whether it's setting up the begining of the level
	
	float last_time_blink; // used for cursor blink
	/// <summary>at PLAY: last time ship fired (used for fire cooldown) 
	/// <para>at END: last time a letter was typed (used for cooldown, to only write a letter once)</para></summary>
	float last_time; 

	int score; // score regarding asteroids destroyed (small -> +10, big -> +5)
	int lives; // lives left for player (ship)
	int vol = 1; // sound and music volume 
	bool isMusicOn = true;
	bool isSoundOn = true;
	bool isPaused = false;

	std::string namelist[NUM_TOP_SCORES]; // contains top high score names
	int scorelist[NUM_TOP_SCORES]; // contains top high scores

	float xpos; // used for positioning cursor in end screen 
	bool scoresaved; // used in endscreen for desplaying high scores
	bool isHighScore;
	int orbs_total; // total number of orbs appeared in game
	int dif_changed; // number of times asteroids were made to go faster (difficulty changed)
	float min_ast_speed; // minimum asteroid speed
	
	bool debug_mode = false;

public:

	Game();
	~Game();
	void init();
	void draw();
	void update();

	void loadingScreen(); 
	void loadPictures();

	void drawStart();
	void drawPlay();
	void drawEnd();

	void updateStart();
	void updatePlay();
	void updateEnd();

	void fireBullet();

	bool isSlow() const { return game_speed == SLOW; }
	bool getIsDebug() const { return debug_mode; };
	Ship* getShip() const { return ship; }
	void setFirst_init(bool val) { first_init = val; }
	bool getFirst_init() const { return first_init; }
	int getScore() const { return score; }
	int getLives() const { return lives; }
	int getDifChanged() const { return dif_changed; }
	float getMinAstSpeed() const { return min_ast_speed; }

	void playGameMusic() {
		if (status == STATUS_START)
			graphics::playMusic(std::string(ASSETS_PATH) + "8_Bit_Surf.mp3", 0.4f * vol);
		else if (status == STATUS_PLAY)
			graphics::playMusic(std::string(ASSETS_PATH) + "Retro_Forest_-_David_Fesliyan.mp3", 0.5f * vol);
		else
			graphics::playMusic(std::string(ASSETS_PATH) + "Space Oddity (8-Bits).mp3", 0.6f * vol);
	}

	// mouse on button ====================================
	bool isMouseOnMusic(); //on music button
	bool isMouseOnSound(); //on sound button
	bool isMouseOnUnpause(); //on unpause button
	bool isMouseOnSave(); //on save button
	bool isMouseOnPlayAgain(); //on play again button
	bool isMouseOnExit(); //on exit button
	// ====================================================

	void destroyStuff(); //deletes ship, asteroids, bullets
	
	template<class T, class S>
	bool collidesWith(T* first, S* second);
};

template<class T, class S>
inline bool Game::collidesWith(T* first, S* second){
	if (!first || !second)
		return false;

	Disk d1 = first->getCollisionHull();
	Disk d2 = second->getCollisionHull();

	double dx = static_cast<double>(d1.cx) - static_cast<double>(d2.cx);
	double dy = static_cast<double>(d1.cy) - static_cast<double>(d2.cy);

	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		return true;
	return false;
}