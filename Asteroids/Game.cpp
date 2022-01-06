#include "graphics.h"
#include "scancodes.h"
#include "Game.h"
#include "config.h"
#include "SpaceObject.h"
#include "Ship.h"
#include "Asteroid.h" 
#include "Bullet.h"
#include "LifeOrb.h"
#include <list>
#include <string>
#include <iostream>
#include <fstream>

std::list<Asteroid*> asteroid_list;
std::list<Bullet*> bullet_list;
std::string name;  // used in update end screen
int numfilelines;  // used when reading high scores from file

void Game::init() {
    score = 0;
    lives = 1;
    xpos = 450;
    first_init = true;
    scoresaved = false;
    isHighScore = false;
    name = "";
    orbs_total = 0;
    dif_changed = 0;
    min_ast_speed = (!isSlow()) ? 0.4f : 0.15f; 
    
    asteroid_list.clear();
    bullet_list.clear();
    
    //set background color ================================
    graphics::Brush br;
    br.fill_color[0] = 12 / 255.f;
    br.fill_color[1] = 8 / 255.f;
    br.fill_color[2] = 26 / 255.f;
    graphics::setWindowBackground(br);

    ship = new Ship(*this);

    for (int i = 0; i < MIN_ASTEROIDS; i++) {
        asteroid_list.emplace_back(new Asteroid(*this));
    }

    setFirst_init(false);

    last_time_blink = graphics::getGlobalTime();
    last_time = graphics::getGlobalTime();

    // initialize namelist
    for (int i = 0; i < 5; i++) { namelist[i] = ""; }

    // read names and high scores from file and add values to arrays
    std::ifstream file(HIGH_SCORES_FILE);
    if (!file) {
        std::cerr << "Cannot read file..." << std::endl;
        return;
    }
    std::string line;
    numfilelines = 0;
    while (getline(file, line)) {
        std::string playername;
        int playerscore;
        file >> playername >> playerscore;
        namelist[numfilelines] = playername;
        scorelist[numfilelines] = playerscore;
        numfilelines++;
    }
    file.close();
    if (isMusicOn) playGameMusic();
}
	
void Game::draw() {
    graphics::Brush br;
    if (status == STATUS_START)
        drawStart();
    else if (status == STATUS_PLAY) {
        drawPlay();
        if (isPaused) {
            // draw redish tint on top of paused game ---------------
            br.fill_color[0] = 219 / 255.0f;
            br.fill_color[1] = 84 / 255.0f;
            br.fill_color[2] = 94 / 255.0f;
            br.fill_opacity = 0.2f;
            graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
            
            // draw play button -------------------------------------
            br.fill_opacity = 1.0f;
            br.fill_color[0] = 0.0f;
            br.fill_color[1] = 0.0f;
            br.fill_color[2] = 0.0f;
            br.outline_width = 6.0f;
            graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, 150, 80, br);
            br.fill_color[0] = 1.0f;
            br.fill_color[1] = 1.0f;
            br.fill_color[2] = 1.0f;
            graphics::drawText(CANVAS_WIDTH / 2 - 50, CANVAS_HEIGHT / 2 + 15, 50, "Play", br);

        }
    }
    else { 
        drawEnd();
    }
    br.fill_color[0] = 1.0f;
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 1.0f;
    // draw music button ========================
    if (isMusicOn)
        br.texture = std::string(ASSETS_PATH) + "musicon.png";
    else
        br.texture = std::string(ASSETS_PATH) + "musicoff.png";
    br.outline_opacity = 0.0f;
    graphics::drawRect(CANVAS_WIDTH - 40, CANVAS_HEIGHT - 40, 40, 40, br);
    // draw sound button ========================
    if (isSoundOn)
        br.texture = std::string(ASSETS_PATH) + "soundon.png";
    else
        br.texture = std::string(ASSETS_PATH) + "soundoff.png";
    br.outline_opacity = 0.0f;
    graphics::drawRect(CANVAS_WIDTH - 80, CANVAS_HEIGHT - 40, 40, 40, br);
}

void Game::update() {
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);
    if (mouse.button_left_released) { // if left mouse click
        //if mouse on music button
        if (isMouseOnMusic()) {
            if (isMusicOn) graphics::stopMusic(200);
            else playGameMusic();
            isMusicOn = !isMusicOn;
        }
        //if mouse on sound button
        if (isMouseOnSound()) {
            isSoundOn = !isSoundOn;
        }
        //if game paused and mouse on unpause button
        if (isPaused && isMouseOnUnpause()) {
            isPaused = false;
        }

    }
    if (graphics::getKeyState(graphics::SCANCODE_P) && !isPaused) { isPaused = true; } // Pause when P is pressed
    if (status == STATUS_START) {
        updateStart();
    }
    else if (status == STATUS_PLAY) {
        if (!isPaused) updatePlay();
    }
    else {
        updateEnd();
    }
}

void Game::loadingScreen() {
    // draw black background
    /*
    graphics::Brush br;
    br.fill_color[0] = 0.0f;
    br.fill_color[1] = 0.0f;
    br.fill_color[2] = 0.0f;
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
    */
}

void Game::loadPictures() {
    graphics::Brush br;
    //br.texture = std::string(ASSETS_PATH) + "asteroid1.1.png";
    //graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    br.texture = std::string(ASSETS_PATH) + "musicoff.png";
    graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    br.texture = std::string(ASSETS_PATH) + "musicon.png";
    //graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    //br.texture = std::string(ASSETS_PATH) + "play.png";
    graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    br.texture = std::string(ASSETS_PATH) + "soundoff.png";
    graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    br.texture = std::string(ASSETS_PATH) + "soundon.png";
    graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    //br.texture = std::string(ASSETS_PATH) + "space5.1.png";
    //graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    //br.texture = std::string(ASSETS_PATH) + "space5.png";
    //graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    for (int i = 1; i < 5; i++) {
        br.texture = std::string(ASSETS_PATH) + "spaceship" + std::to_string(i) + ".png";
        graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    }
    for (int i = 1; i < 7; i++) {
        br.texture = std::string(ASSETS_PATH) + "stage" + std::to_string(i) + "orange.png";
        graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
        br.texture = std::string(ASSETS_PATH) + "stage" + std::to_string(i) + "red.png";
        graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
    }
    br.texture = std::string(ASSETS_PATH) + "stars joined.png";
    graphics::drawRect(CANVAS_WIDTH, CANVAS_HEIGHT, 1, 1, br);
}

// draw ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
void Game::drawStart() {

    // add background image =====================
    graphics::Brush br;
    br.texture = std::string(ASSETS_PATH) + "space5.1.png";
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

    // draw astronaut ===========================
    //br.outline_opacity = 0.0f; 
    //br.texture = std::string(ASSETS_PATH) + "astronaut.png";
    //graphics::drawRect(CANVAS_WIDTH / 10, CANVAS_HEIGHT *8/14, 560 *4/7, 1055 *4/7, br);

    // write title ==============================
    graphics::setFont(std::string(ASSETS_PATH) + "manaspc.ttf");
    std::string title = "Space  Shooters";
    br.fill_color[0] = 186 / 255.0f;
    br.fill_color[1] = 110 / 255.0f;
    br.fill_color[2] = 116 / 255.0f;
    graphics::drawText(363, 122, 70, title, br);
    br.fill_color[0] = 145 / 255.0f;
    br.fill_color[1] = 42 / 255.0f;
    br.fill_color[2] = 51 / 255.0f;
    graphics::drawText(360, 120, 70, title, br);

    br.outline_color[0] = 145 / 255.0f;
    br.outline_color[1] = 42 / 255.0f;
    br.outline_color[2] = 51 / 255.0f;
    graphics::drawLine(360, 128, 390, 128, br);
    graphics::drawLine(415, 128, 860, 128, br);

    // write explanatory text ===================
    br.fill_color[0] = 186 / 255.0f;
    br.fill_color[1] = 110 / 255.0f;
    br.fill_color[2] = 116 / 255.0f;
    int ypos = 260;
    std::string info;
    int textsize = 28;

    info = "DESTROY  the  ASTEROIDS  and  get  a  HIGH  SCORE!";
    graphics::drawText(310, ypos, textsize, info, br);     ypos += 45;
    info = "Use  the  CURSOR  to  MOVE,  and  LEFT  CLICK / SPACE  to  FIRE.";
    graphics::drawText(245, ypos, textsize, info, br);     ypos += 45;
    info = "Big  asteroids  give  5  POINTS.  Small  asteroids  give  10  POINTS.";
    graphics::drawText(230, ypos, textsize, info, br);     ypos += 45;
    info = "You  have  LIMITED  LIVES.";
    graphics::drawText(460, ypos, textsize, info, br);     ypos += 45;
    info = "Magic ORB gives EXTRA LIFE.";
    graphics::drawText(420, ypos, textsize, info, br);     ypos += 45;
    ypos += 155;
    
    br.fill_color[0] = 181 / 255.0f;
    br.fill_color[1] = 139 / 255.0f;
    br.fill_color[2] = 143 / 255.0f;
    info = "PRESS  ENTER  TO  START";
    graphics::drawText(390, ypos, textsize + 10, info, br);

    // cursor flickers ==========================
    if (graphics::getGlobalTime() - last_time_blink < 500) {        // turn color to background
        br.fill_color[0] = 18 / 255.0f;
        br.fill_color[1] = 23 / 255.0f;
        br.fill_color[2] = 38 / 255.0f;
    }
    else if (graphics::getGlobalTime() - last_time_blink < 900) {   // turn color to text color
        br.fill_color[0] = 181 / 255.0f;
        br.fill_color[1] = 139 / 255.0f;
        br.fill_color[2] = 143 / 255.0f;
    }
    else {
        last_time_blink = graphics::getGlobalTime();
    }
    graphics::drawText(785, ypos, textsize + 10, "|", br);
}

void Game::drawPlay() {
    //add background image ================================
    graphics::Brush br;
    br.texture = std::string(ASSETS_PATH) + "space5.png";
    graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

    // draw asteroids =====================================
    for (Asteroid* ast : asteroid_list)
        if (ast) ast->draw();

    // draw life orb ======================================
    if (orb) orb->draw();

    // draw ship ==========================================
    if (ship) ship->draw();

    // draw bullets =======================================
    for (Bullet* bul : bullet_list)
        if (bul) bul->draw();

    // show score =========================================
    graphics::setFont(std::string(ASSETS_PATH) + "manaspc.ttf");
    std::string info = "S c o r e : " + std::to_string(getScore());
    graphics::drawText(CANVAS_WIDTH - 200, 30, 20, info, br);

    // show lives =========================================
    graphics::setOrientation(35.0f);
    br.outline_opacity = 0.0f;
    br.texture = std::string(ASSETS_PATH) + "spaceship1.png";
    graphics::drawRect(CANVAS_WIDTH - 190, 50, SHIP_DIM_X * 3/4, SHIP_DIM_Y * 3/4, br);
    graphics::resetPose();
    info = "x"; 
    graphics::drawText(CANVAS_WIDTH - 170, 55, 17, info, br);
    info = std::to_string(getLives());
    graphics::drawText(CANVAS_WIDTH - 155, 57, 20, info, br);

    // show "Press P to Pause" ============================
    if (!isPaused) {
        info = "Press P to Pause";
        graphics::drawText(15, CANVAS_HEIGHT - 15, 20, info, br);
    }
}

void Game::drawEnd() {
    // add background image ================================
    graphics::Brush br;
    br.texture = std::string(ASSETS_PATH) + "stars joined.png";
    float offset = CANVAS_WIDTH * sinf(graphics::getGlobalTime() / 5000.0f) / 10;
    graphics::drawRect(CANVAS_WIDTH / 2 + offset, CANVAS_HEIGHT / 2, CANVAS_WIDTH * 2, CANVAS_HEIGHT, br);
    // show player's score ================================
    graphics::setFont(std::string(ASSETS_PATH) + "manaspc.ttf");
    std::string info = "S c o r e : " + std::to_string(getScore());
    graphics::drawText(550, 50, 30, info, br);

    int textpos = 120;
    info = "H i g h  S c o r e s :";
    graphics::drawText(470, textpos, 40, info, br);         textpos += 70;

    // print names and high scores ========================
    for (int i = 0; i < NUM_TOP_SCORES; i++) {
        if (namelist[i] != "") {
            std::string name = namelist[i];
            if (name == "-") { name = " "; }
            graphics::drawText(400, textpos, 30, name, br);
            graphics::drawText(750, textpos, 30, std::to_string(scorelist[i]), br);     textpos += 40;
        }
        else {
            graphics::drawText(400, textpos, 30,"_  _  _  _  _  _  _", br);
            graphics::drawText(750, textpos, 30, "_  _  _", br);     textpos += 40;
        }
    }
    textpos += 50;

    // show "Enter your name:", flickering cursor, and "Save" button
    if (score > scorelist[numfilelines - 1] || numfilelines < NUM_TOP_SCORES) { // if score high enough to be in top high scores
        isHighScore = true;
        if (!scoresaved) {
            info = "E n t e r  y o u r  n a m e :  ";
            graphics::drawText(100, textpos, 35, info, br);
            // cursor flickers ================================
            if (graphics::getGlobalTime() - last_time_blink < 500) {        // turn color to black
                br.fill_color[0] = 0.0f;
                br.fill_color[1] = 0.0f;
                br.fill_color[2] = 0.0f;
            }
            else if (graphics::getGlobalTime() - last_time_blink < 900) {   // turn color to white
                br.fill_color[0] = 1.0f;
                br.fill_color[1] = 1.0f;
                br.fill_color[2] = 1.0f;
            }
            else {
                last_time_blink = graphics::getGlobalTime();
            }
            graphics::drawText(xpos, textpos, 35, "|", br);
            br.fill_color[0] = 1.0f; // turn color back to white
            br.fill_color[1] = 1.0f; // so other stuff dont flicker
            br.fill_color[2] = 1.0f; // like the cursor
            // draw "Save" button =========================
            graphics::drawText(1000, 640, 40, "Save", br);
            br.outline_opacity = 1.0f;
            graphics::drawRect(1040, 628, 113, 50, br);
        }
    }
    // draw "Play Again" button and "Exit" button =========
    if ( !isHighScore || (isHighScore && scoresaved) ) {
        graphics::drawText(520, 640, 40, "Play Again", br);
        graphics::drawRect(610, 630, 200, 60, br);
        graphics::drawText(1000, 640, 40, "Exit", br); 
        graphics::drawRect(1035, 628, 100, 50, br); 
    }
}

// update ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
void Game::updateStart() {
    if (graphics::getKeyState(graphics::SCANCODE_KP_ENTER)) {
        status = STATUS_PLAY;
        if (isMusicOn) playGameMusic(); 
    }
}

void Game::updatePlay() {
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);

    // update ship ========================================
    float shiptime = 0.0f;
    if (ship) {
        ship->update();
        if (ship->getStage() > 6) {
            shiptime = ship->getLastTime();
            delete ship;
            ship = nullptr;
        }
        // checks collision ship-orb
        if (orb && collidesWith(ship, orb)) {
            delete orb;
            orb = nullptr;
            lives++; // earns a life
        }
    }
    else if (graphics::getGlobalTime() - shiptime > 3000) { // don't make new ship immediately
        ship = new Ship(*this);
    }

    // update asteroids ===================================
    auto ast = asteroid_list.begin();
    while (ast != asteroid_list.end()) {
        if ((*ast)) {
            (*ast)->update();
            // check collision asteroid-ship
            if (ship && ship->getIsActive() && collidesWith((*ast), ship)) {
                ship->increaseStage();      // to activate explosion animation
                ship->setIsActive(false);   // without this, it keeps losing lives while touching asteroid (while explosion animation happens)
                if (isSoundOn) graphics::playSound(std::string(ASSETS_PATH) + "SFX_Explosion_19.wav", 0.4f * vol, false);
                lives--; // loses a life when crash with asteroid
                if (lives < 0) {
                    status = STATUS_END;
                    last_time = graphics::getGlobalTime();
                    if (isMusicOn) playGameMusic();
                    last_time_blink = graphics::getGlobalTime();
                    return;
                }
            }
            if ((*ast)->getStage() > 6) {
                delete (*ast);
                (*ast) = nullptr;
                ast = asteroid_list.erase(ast);
            }
            else {
                ast++;
            }
        }
    }
    // big asteroids break to 2 smaller ones ==============
    //for (auto ast = asteroid_list.begin(); ast != asteroid_list.end(); ast++) {
    //    if ((*ast)->isBig() && (*ast)->getStage() == 4) { /////////////////////////////////////////
    //        asteroid_list.emplace_back(new Asteroid(*this, *ast, 0));
    //        asteroid_list.emplace_back(new Asteroid(*this, *ast, 1));
    //    }
    //}
    
    // update bullets =====================================
    auto bul = bullet_list.begin();
    while (bul != bullet_list.end()) {
        bool deletedbul = false;
        if ((*bul)) {
            (*bul)->update();
            // if out of bounds
            if ((*bul)->getX() > CANVAS_WIDTH || (*bul)->getX() < 0.0f || (*bul)->getY() > CANVAS_HEIGHT || (*bul)->getY() < 0.0f) {
                delete (*bul);
                (*bul) = nullptr;
                bul = bullet_list.erase(bul);
                deletedbul = true;
            }
            else {
                // checks collision bullet-astreroid
                for (Asteroid* ast : asteroid_list) {
                    if (ast && !ast->getStage() && collidesWith((*bul), ast)) {
                        if (isSoundOn) graphics::playSound(std::string(ASSETS_PATH) + "SFX_Explosion_02.wav", 0.2f * vol, false);
                        // increase score
                        if (!ast->isBig())
                            score += 10;
                        else
                            score += 5;
                        // increase asteroid stage
                        ast->increaseStage();
                        // delete bullet
                        delete (*bul);
                        (*bul) = nullptr;
                        bul = bullet_list.erase(bul);
                        deletedbul = true;
                        break;
                    }
                } //for
            } //else
        } //if
        if (!deletedbul) bul++;
    } //for
    
    // if too few asteroids, make more! ===================
    while (asteroid_list.size() < MIN_ASTEROIDS) { 
        asteroid_list.emplace_back(new Asteroid(*this));
    }
    // increase asteroid speed if above certain score =====
    if ((int)score / DIF_CHANGE_EVERY > dif_changed) {
        min_ast_speed += 0.1;
        dif_changed++;
    }

    // update lifeorb =====================================
    if (orb) {
        orb->update();
        // if out of bounds
        if (orb->getX() > CANVAS_WIDTH || orb->getX() < 0.0f || orb->getY() > CANVAS_HEIGHT || orb->getY() < 0.0f) {
            delete orb;
            orb = nullptr;
        }
    }
    else if ((int)score / APPEAR_EVERY > orbs_total) {
         orb = new LifeOrb(*this);
         orbs_total++; 
    }

    // fires bullet on left mouse click ===================
    if (mouse.button_left_released) { // if left mouse click
        // if mouse not on music or sound button
        if (!isMouseOnMusic() && !isMouseOnSound()) {
            fireBullet();
        }
    }
    // fires bullet on space press ========================
    if (graphics::getKeyState(graphics::SCANCODE_SPACE)) { fireBullet(); }
    
    if (score > 9999999999) { 
        status = STATUS_END;
        if (isMusicOn) playGameMusic(); 
    }
}

void Game::updateEnd() {
    if (!isHighScore || (isHighScore && scoresaved)) {
        graphics::MouseState mouse;
        graphics::getMouseState(mouse);
        if (mouse.button_left_released) { // if left mouse click
            // if mouse on "Play Again" button
            if (isMouseOnPlayAgain()) {
                destroyStuff();
                // re-initialise everything ===============
                init();
                status = STATUS_PLAY;
                if (isMusicOn) playGameMusic();
            }
            // if mouse on "Exit" button
            if (isMouseOnExit()) {
                destroyStuff();
                exit(0);
            }
        }
    }

    graphics::Brush br;
    br.fill_color[0] = 1.0f;
    br.fill_color[1] = 1.0f;
    br.fill_color[2] = 1.0f;

    // typing name ========================================
    // only appear when name < 15 characters and score can be in highscores
    std::string info = "";
    if (isHighScore){
        if (!scoresaved) {
            if (name.length() < 30 && graphics::getGlobalTime() - last_time > TYPING_COOLDOWN) {
                last_time = graphics::getGlobalTime(); 
                if (graphics::getKeyState(graphics::SCANCODE_A)) { info = "A ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_B)) { info = "B ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_C)) { info = "C ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_D)) { info = "D ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_E)) { info = "E ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_F)) { info = "F ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_G)) { info = "G ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_H)) { info = "H ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_I)) { info = "I ";   xpos += 8.0 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_J)) { info = "J ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_K)) { info = "K ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_L)) { info = "L ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_M)) { info = "M ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_N)) { info = "N ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_O)) { info = "O ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_P)) { info = "P ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_Q)) { info = "Q ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_R)) { info = "R ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_S)) { info = "S ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_T)) { info = "T ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_U)) { info = "U ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_V)) { info = "V ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_W)) { info = "W ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_X)) { info = "X ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_Y)) { info = "Y ";   xpos += 19.4 + 5; }
                else if (graphics::getKeyState(graphics::SCANCODE_Z)) { info = "Z ";   xpos += 19.4 + 5; }

                name.append(info);

                if (graphics::getKeyState(graphics::SCANCODE_BACKSPACE) && !name.empty()) {
                    xpos -= (name.substr(name.length() - 2) == "I ") ? (8.0 + 5) : (19.4 + 5);
                    name = name.substr(0, name.size() - 2); 
                }
            }
            graphics::drawText(450, 640, 35, name, br);
        }
        
        // if left click on Save "button" =================
        graphics::MouseState mouse;
        graphics::getMouseState(mouse);
        if (mouse.button_left_released) { // if left mouse click
            // if mouse on Save button
            if (isMouseOnSave()) {
                if (name != "") {
                    // remove extra spaces between letters ---
                    int count = 0; // counts letters in name
                    for (int i = 0; name[i]; i++)
                        if (name[i] != ' ')
                            name[count++] = name[i];
                    name[count] = '\0';
                    name.erase(name.begin() + count + 1, name.end());
                    // ----------------------------------------
                }
                else {
                    name = "-";
                }
                if (numfilelines < NUM_TOP_SCORES) { numfilelines++; }
                namelist[numfilelines - 1] = name;
                scorelist[numfilelines - 1] = score;
                // sort high scores (bubble sort)
                for (int i = 0; i < numfilelines - 1; i++) {
                    for (int j = 0; j < numfilelines - 1 - i; j++) {
                        if (scorelist[j] < scorelist[j + 1]) {
                            int temp1 = scorelist[j];
                            scorelist[j] = scorelist[j + 1];
                            scorelist[j + 1] = temp1;
                            std::string temp2 = namelist[j];
                            namelist[j] = namelist[j + 1];
                            namelist[j + 1] = temp2;
                        }
                    }
                }
                // write high scores in file
                std::ofstream file(HIGH_SCORES_FILE);
                if (!file) {
                    std::cerr << "Cannot read file..." << std::endl;
                    return;
                }
                for (int i = 0; i < numfilelines; i++) {
                    file << "\n" << namelist[i] << " " << scorelist[i];
                    if (file.fail()) {
                        std::cerr << "Could not write data" << std::endl;
                    }
                }
                scoresaved = true;
                file.close();
            }
        }
    }
    
}

void Game::fireBullet() {
    if (ship && ship->getIsActive() && graphics::getGlobalTime() - last_time > FIRE_COOLDOWN) {
        if (isSoundOn) graphics::playSound(std::string(ASSETS_PATH) + "8bit_gunloop_explosion.wav", 0.2f * vol, false);
        bullet_list.emplace_back(new Bullet(*this));
        last_time = graphics::getGlobalTime();
    }
}

// mouse on button ====================================
bool Game::isMouseOnMusic() { //on music button
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);
    return mouse.cur_pos_x > CANVAS_WIDTH - 40 - 20 && mouse.cur_pos_x < CANVAS_WIDTH - 40 + 20 && mouse.cur_pos_y > CANVAS_HEIGHT - 40 - 20 && mouse.cur_pos_y < CANVAS_HEIGHT - 40 + 20;
}
bool Game::isMouseOnSound() { //on sound button
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);
    return mouse.cur_pos_x > CANVAS_WIDTH - 80 - 12 && mouse.cur_pos_x < CANVAS_WIDTH - 80 + 12 && mouse.cur_pos_y > CANVAS_HEIGHT - 40 - 12 && mouse.cur_pos_y < CANVAS_HEIGHT - 40 + 12;
}
bool Game::isMouseOnUnpause() { //on unpause button
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);
    //return mouse.cur_pos_x > CANVAS_WIDTH / 2 - 100 && mouse.cur_pos_x < CANVAS_WIDTH / 2 + 100 && mouse.cur_pos_y > CANVAS_HEIGHT / 2 - 100 && mouse.cur_pos_y < CANVAS_HEIGHT / 2 + 100;
    return mouse.cur_pos_x > CANVAS_WIDTH / 2 - 75 && mouse.cur_pos_x < CANVAS_WIDTH / 2 + 75 && mouse.cur_pos_y > CANVAS_HEIGHT / 2 - 40 && mouse.cur_pos_y < CANVAS_HEIGHT / 2 + 40;
}
bool Game::isMouseOnSave() { //on save button
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);
    return mouse.cur_pos_x > 1040 - 113 / 2 && mouse.cur_pos_x < 1040 + 113 / 2 && mouse.cur_pos_y > 628 - 50 / 2 && mouse.cur_pos_y < 628 + 50 / 2;
}
bool Game::isMouseOnPlayAgain() { //on play again button
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);
    return mouse.cur_pos_x > 610 - 200 / 2 && mouse.cur_pos_x < 610 + 200 / 2 && mouse.cur_pos_y > 630 - 60 / 2 && mouse.cur_pos_y < 630 + 60 / 2;
}
bool Game::isMouseOnExit() {
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);
    return mouse.cur_pos_x > 1035 - 100 / 2 && mouse.cur_pos_x < 1035 + 100 / 2 && mouse.cur_pos_y > 628 - 50 / 2 && mouse.cur_pos_y < 628 + 50 / 2;
}
// ====================================================

void Game::destroyStuff() {
    // destroy ship ===========================
    if (ship) {
        delete ship;
        ship = nullptr;
    }
    // destroy asteroids ======================
    for (Asteroid* ast : asteroid_list) {
        if (ast) {
            delete ast;
            ast = nullptr;
        }
    }
    // destroy bullets ========================
    for (Bullet* bul : bullet_list) {
        if (bul) {
            delete bul;
            bul = nullptr;
        }
    }
}

Game::Game() {
}

Game::~Game() {
    destroyStuff(); 
}