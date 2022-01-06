#include "graphics.h"
#include "Ship.h"
#include "Game.h"
#include "config.h"
#include <iostream>

void Ship::init() {
    graphics::MouseState mouse;
    graphics::getMouseState(mouse);

    center_x = CANVAS_WIDTH / 2;
    center_y = CANVAS_HEIGHT / 2;

    double dx = (double)(mouse.cur_pos_x) - (double)(center_x); //ship direction x
    double dy = (double)(mouse.cur_pos_y) - (double)(center_y); //ship direction y
    
    angle = atan2(dx, dy) * 180 / (atan(1)*4) + 180; // pi = atan(1)*4
    radius = SHIP_DIM_X / 2;
    speed = (!game.isSlow()) ? 1.3f : 0.8f;

    last_time_change = last_frame_change = graphics::getGlobalTime();
}


void Ship::draw() {
    graphics::setOrientation(angle);

    graphics::Brush br;
    br.texture = std::string(ASSETS_PATH) + "spaceship" + std::to_string(frame) +".png";
    br.outline_opacity = 0.0f;

    if (!is_active){ // for flickering during cooldown
        bool towait = false;
        for (int i = 100; i < 150; i++) {
            if ((int)(graphics::getGlobalTime() - last_time_change) % i == 0) towait = true;
        }
        if (towait) { // don't draw, to seem like flicker
            graphics::resetPose();
            return;
        }
    }
    // draw ship
    if (stage < 4)
        graphics::drawRect(center_x, center_y, SHIP_DIM_X, SHIP_DIM_Y, br); 

    // draw explosion if needed
    if (stage) {
        br.texture = std::string(ASSETS_PATH) + "stage" + std::to_string(stage) + "red.png"; 
        graphics::drawRect(center_x, center_y, radius * (stage / 3.0f), radius * (stage / 3.0f), br);
    }

    graphics::resetPose();
    
    if (game.getIsDebug()) {
        // draw Collision Disk during Debug Mode
        br.outline_opacity = 1.0f;
        br.texture = "";
        br.fill_color[0] = 0.3f;
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 0.3f;
        br.fill_opacity = 0.3f;
        Disk col_hull = getCollisionHull();
        graphics::drawDisk(col_hull.cx, col_hull.cy, col_hull.radius, br);

        // draw line at front of ship
        graphics::drawLine(center_x, center_y, center_x + cos((-(double)angle - 90.0f) * (atan(1) * 4) / 180.0f) * 30.0f, center_y + sin((-(double)angle - 90.0f) * (atan(1) * 4) / 180.0f) * 30.0f, br);

        // show info above ship
        br.fill_color[0] = 1.0f;
        br.fill_color[1] = 1.0f;
        br.fill_color[2] = 1.0f;
        br.fill_opacity = 0.6f;
        graphics::drawText(center_x - 20, center_y - radius - 10, 15, "stage: " + std::to_string(stage), br);   // stage: 0-6
        graphics::drawText(center_x - 20, center_y - radius, 15, (is_active) ? "active" : "not active", br);    // active or not
    }
    graphics::resetPose();
}


void Ship::update() {

    if (graphics::getGlobalTime() - last_time_change > SHIP_COOLDOWN && stage == 0) { // wait time for cooldown
        is_active = true;
    }

    graphics::MouseState mouse;
    graphics::getMouseState(mouse);

    double dx = (double)(mouse.cur_pos_x) - (double)(center_x); //ship direction x
    double dy = (double)(mouse.cur_pos_y) - (double)(center_y); //ship direction y
    
    // future ship center based mouse current position
    float future_center_x = center_x + dx / sqrt(dx * dx + dy * dy) * speed;
    float future_center_y = center_y + dy / sqrt(dx * dx + dy * dy) * speed;
    
    // to not flicker when mouse current position is the same as ship
    if (mouse.prev_pos_x != mouse.cur_pos_x && mouse.prev_pos_y != mouse.cur_pos_y)
        angle = atan2(dx, dy) * 180 / (atan(1)*4) + 180; // pi = atan(1)*4

    // new ship center x     
    if ((future_center_x + radius) >= CANVAS_WIDTH)
        center_x = radius + dx / sqrt(dx * dx + dy * dy) * speed;
    else if ((future_center_x - radius) <= 0.0f)
        center_x = CANVAS_WIDTH - radius - dx / sqrt(dx * dx + dy * dy) * speed;
    else
        center_x += dx / sqrt(dx * dx + dy * dy) * speed;

    // new ship center y 
    if ((future_center_y + radius) >= CANVAS_HEIGHT)
        center_y =radius + dy / sqrt(dx * dx + dy * dy) * speed;
    else if ((future_center_y - radius) <= 0.0f)
        center_y = CANVAS_HEIGHT - radius - dy / sqrt(dx * dx + dy * dy) * speed;
    else
        center_y += dy / sqrt(dx * dx + dy * dy) * speed;

    // explosion timing for stages ========================
    if (stage && graphics::getGlobalTime() - last_time_change > 100) {
        stage++;
        last_time_change = graphics::getGlobalTime();
    }
    // changing frame (to animate flame) ==================
    if (!stage && graphics::getGlobalTime() - last_frame_change > 120) {
        frame++;
        if (frame > 4) frame = 1;
        last_frame_change = graphics::getGlobalTime();
    }
}


Disk Ship::getCollisionHull() const {
    Disk disk;
    disk.cx = center_x + radius / 5 * cos(-((double)angle + 90) * (atan(1) * 4) / 180.0f);
    disk.cy = center_y + radius / 5 * sin(-((double)angle + 90) * (atan(1) * 4) / 180.0f);
    disk.radius = radius * 4/5;
    return disk;
}


Ship::Ship(const Game& game) : SpaceObject(game) {
    init();
}


Ship::Ship() {
}


Ship::~Ship() {
}