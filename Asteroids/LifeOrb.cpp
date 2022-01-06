#include "graphics.h"
#include "LifeOrb.h"
#include "Game.h"
#include "config.h"
#include <stdlib.h>


void LifeOrb::init() {
    radius = 20.0f;
    speed = (!game.isSlow()) ? 0.02f : 0.006f;
    center_x = rand() % (int)(CANVAS_WIDTH - 200) + 200; 
    center_y = rand() % (int)(CANVAS_HEIGHT - 200) + 200; 

    double dx = (double)(CANVAS_WIDTH / 2) - (double)center_x; //orb direction x
    double dy = (double)(CANVAS_HEIGHT / 2) - (double)center_y; //orb direction y
    angle = atan2(dx, dy) * 180 / (atan(1) * 4); // pi = atan(1)*4

    inv_radius = rand() % 50 + 100;     // random between 100 and 150
    inv_x = center_x - inv_radius; 
    inv_y = center_y - inv_radius; 

    prev_inv_radius = inv_radius;
    prev_inv_x = inv_x;
    prev_inv_y = inv_y;

    last_time_change = graphics::getGlobalTime();
}


void LifeOrb::draw() {
    graphics::Brush br;

    br.fill_color[0] = (sin((double)graphics::getGlobalTime() / 1000 + 0) * 127 + 128) / 255.0f;
    br.fill_color[1] = (sin((double)graphics::getGlobalTime() / 1000 + 2) * 127 + 128) / 255.0f;
    br.fill_color[2] = (sin((double)graphics::getGlobalTime() / 1000 + 4) * 127 + 128) / 255.0f;
    br.fill_opacity = 1.0f;
    
    br.fill_secondary_color[0] = (sin((double)graphics::getGlobalTime() / 950 + 0) * 127 + 128) / 255.0f;
    br.fill_secondary_color[1] = (sin((double)graphics::getGlobalTime() / 950 + 2) * 127 + 128) / 255.0f;
    br.fill_secondary_color[2] = (sin((double)graphics::getGlobalTime() / 950 + 4) * 127 + 128) / 255.0f;
    br.fill_secondary_opacity = 0.0f;

    br.outline_opacity = 0.0f;
    br.gradient = true;

    //if (is_flickering) { // for flickering when it first appears
    //    bool towait = false;
    //    for (int i = 100; i < 150; i++) {
    //        if ((int)(graphics::getGlobalTime() - last_time_change) % i == 0) towait = true;
    //    }
    //    if (towait) { // don't draw, to seem like flicker
    //        return;
    //    }
    //}

    graphics::drawDisk(center_x, center_y, radius, br);
    
    if (false) { //(game.getIsDebug()) {     ////////////////////////
        // draw current and previous invisible circles during Debug Mode
        br.outline_opacity = 1.0f;
        br.texture = "";
        br.fill_opacity = 0.0f;
        graphics::drawDisk(inv_x, inv_y, inv_radius, br);
        graphics::drawLine(center_x, center_y, inv_x, inv_y, br);
        graphics::drawDisk(prev_inv_x, prev_inv_y, prev_inv_radius, br);
        // draw Collision Disk during Debug Mode
        br.outline_opacity = 1.0f;
        br.texture = "";
        br.fill_color[0] = 1.0f;
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 0.3f;
        br.fill_opacity = 0.3f;
        Disk col_hull = getCollisionHull();
        graphics::drawDisk(col_hull.cx, col_hull.cy, col_hull.radius, br);
    }
}


void LifeOrb::update() {
    center_x = inv_x + cos(angle) * inv_radius;
    center_y = inv_y + sin(angle) * inv_radius;
    angle += speed;

    // to change the circle it's moving on ================
    if (graphics::getGlobalTime() - last_time_change > 5000) {
        last_time_change = graphics::getGlobalTime();

        prev_inv_radius = inv_radius;
        prev_inv_x = inv_x;
        prev_inv_y = inv_y;

        inv_radius = rand() % 30 + 50;
        inv_x = center_x + cos(angle) * inv_radius;
        inv_y = center_y + sin(angle) * inv_radius;
        angle += 185.4f; 
        speed *= -1;
    }
}


Disk LifeOrb::getCollisionHull() const {
    Disk disk;
    disk.cx = center_x;
    disk.cy = center_y;
    disk.radius = radius - 5;
    return disk;
}


LifeOrb::LifeOrb(const Game& game) { 
    init();
}


LifeOrb::LifeOrb() {
}

LifeOrb::~LifeOrb() {
}
