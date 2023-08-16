#include "include/Snake.h"
#include "include/structs.h"

#include <deque>

std::deque<TilePos>& Snake::get_body() { return body; }
MoveDir Snake::get_dir() { return dir; }
unsigned short& Snake::get_lives() { return lives; }
SnakeLevel* Snake::get_level() { return running_level; }

void Snake::set_dir(MoveDir dir) { this->dir = dir; }
void Snake::set_lives(unsigned short lives) { this->lives = lives; }
void Snake::bind_level(SnakeLevel *level) { this->running_level = level; }

void Snake::step() {
    TilePos next_loc = get_next_location();
    tile_type_e next_type = running_level->get_tile_type(next_loc);

    running_level->set_tile_type(body.front(), SnakeBody);

    body.push_front(next_loc);

    if(next_type != Food) {
        running_level->set_tile_type(body.back(), Empty);
        body.pop_back();
    }
    running_level->set_tile_type(body.front(), SnakeHead);
}

TilePos Snake::get_next_location() {
    TilePos new_loc = body.front();

    new_loc.row += dir.dy;
    new_loc.col += dir.dx;

    return new_loc;
}

void Snake::clear() {
    for(auto tile: body){
        running_level->set_tile_type(tile, Empty);
    }
    body.clear();
}

void Snake::init() {
    dir = NEUTRAL;
    running_level->set_tile_type(running_level->get_spawn_loc(), SnakeHead);
    body.push_back(running_level->get_spawn_loc());
}
