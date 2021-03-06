#include "graphics.h"
#include "Game.h"
#include "config.h"


void update(float ms) {
    Game * game = reinterpret_cast<Game*>(graphics::getUserData());
    game->update();
}


void draw() {
    Game * game = reinterpret_cast<Game*>(graphics::getUserData());
    game->draw();
}


int main() {
    Game game;

    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Shooters");
    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    graphics::setUserData(&game);
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    game.init();
    game.loadingScreen(); 
    game.loadPictures();
    
    graphics::startMessageLoop();
    
    return 0;
}