#include "graphics.h"
#include "Asteroid.h"
#include "Game.h"
#include "config.h"
#include <stdlib.h>


void Asteroid::init() {
    
    radius = (rand() % 2) ? rand() % 15 + 25 : rand() % 20 + 40;	// random between 25-30(small) and 40-60(big)
    speed = (rand() % 2 + 1) * game.getMinAstSpeed();

    if (game.getFirst_init()) {
        angle = rand() % 360;			// random between 0 and 360
        do {
            center_x = rand() % (CANVAS_WIDTH - 10) + 10;
            center_y = rand() % (CANVAS_HEIGHT - 10) + 10;
        } while (abs(center_x - CANVAS_WIDTH / 2) < 100 && abs(center_y - CANVAS_HEIGHT / 2)); // while too close to ship 
    }
    else {
        int left_right = pow(-1, rand()%1), up_down = pow(-1, rand()%1);
        //  left = -1, right = 1            up = -1, down = 1

        center_x = CANVAS_WIDTH + left_right * (radius - 2.0f);
        center_y = CANVAS_HEIGHT + up_down * (radius - 2.0f);
        angle = rand() % 
            (int) (-( atan2( ((double)((left_right) ? CANVAS_WIDTH : 0) - center_x), CANVAS_HEIGHT - (double)center_y ) * 180 / (atan(1) * 4)
                    - atan2( ((double)((left_right) ? CANVAS_WIDTH : 0) - center_x), -center_y ) * 180 / (atan(1) * 4) ) - radius * 3)
            - 180 - radius * 3; 
            // random between -180-theta and -180, 
            // where theta the angle forming on (center_x,center_y) with the two closest canvas corners
            // there's also radius * 3 added on either "side" to further confine the angle
    }
    rotation = angle;
}

/*
void Asteroid::init(Asteroid* ast, int num) { ///////////////////////////
    radius = rand() % 15 + 25;	// random between 25-30(small)
    speed = (rand() % 2 + 1) * game.getMinAstSpeed();
    center_x = ast->getX();
    center_y = ast->getY();
    angle = ast->getAngle() + 100.0f * pow(-1, num);
    rotation = angle;
}
*/

void Asteroid::draw() {

	graphics::setOrientation(rotation);

	graphics::Brush br;
	br.texture = std::string(ASSETS_PATH) + "asteroid1.1.png";
	br.outline_opacity = 0.0f;

    if (stage < 4)
	    graphics::drawRect(center_x, center_y, radius * 1.7f, radius * 1.7f, br);

    // draw explosion if needed
    if (stage) {
        br.texture = std::string(ASSETS_PATH) + "stage" + std::to_string(stage) + "orange.png";
        graphics::drawRect(center_x, center_y, radius * (stage / 3.0f), radius * (stage / 3.0f), br);
    }
     graphics::resetPose();

    if (game.getIsDebug()) {
        // draw Collision Disk during Debug Mode
        br.outline_opacity = 1.0f;
        br.texture = "";
        br.fill_color[0] = 1.0f;
        br.fill_color[1] = 0.3f;
        br.fill_color[2] = 0.3f;
        br.fill_opacity = 0.3f;
        Disk col_hull = getCollisionHull();
        graphics::drawDisk(col_hull.cx, col_hull.cy, col_hull.radius, br);

        // show info above asteroid
        br.fill_color[0] = 1.0f;
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;
        br.fill_opacity = 0.6f;
        graphics::drawText(center_x - 10, center_y - radius - 15, 15, (radius < 40) ? "small" : "big", br);  // size: big / small
        graphics::drawText(center_x - 20, center_y - radius, 15, "stage: "+std::to_string(stage), br);       // stage: 0-6
    }
}


void Asteroid::update() {

    rotation += (speed > 1.0) ? speed / 4 : 0.3; // different rotation rates
    
    // future asteroid center =============================
    float future_center_x = center_x + speed * cos(angle);
    float future_center_y = center_y + speed * sin(angle);

    float margin = (radius < 50) ? radius / 6 : - radius / 2;
    // new asteroid center x ==============================
    if ((future_center_x + margin) >= CANVAS_WIDTH)
        center_x = (margin + 0.1f) + speed * cos(angle);
    else if ((future_center_x - margin) <= 0.0f)
        center_x = CANVAS_WIDTH - (margin + 0.1f) - speed * cos(angle);
    else
        center_x += speed * cos(angle);
    
    // new asteroid center y ==============================
    if ((future_center_y + margin) >= CANVAS_HEIGHT)
        center_y = (margin + 0.1f) + speed * sin(angle);
    else if ((future_center_y - margin) <= 0.0f)
        center_y = CANVAS_HEIGHT - (margin + 0.1f) - speed * sin(angle);
    else
        center_y += speed * sin(angle);

    // explosion timing for stages ========================
    int waittime = (radius < 40) ? 100 : 125; // wait time between frames: 100 for small, 125 for big
    if (stage && graphics::getGlobalTime() - last_time_change > waittime) {
        stage++;
        last_time_change = graphics::getGlobalTime();
    }
}


Disk Asteroid::getCollisionHull() const {
    Disk disk;
    disk.cx = center_x;
    disk.cy = center_y;
    disk.radius = radius - radius/6;
    return disk;
}


Asteroid::Asteroid(const Game& game) : SpaceObject(game) {
    init();
}

//Asteroid::Asteroid(const Game& game, Asteroid* ast, int num) { //////////////////////
//    init(ast, num);
//}


Asteroid::Asteroid() {
}


Asteroid::~Asteroid() {
}
