#include "include/RSPlayer.h"
#include "include/structs.h"
#include <ctime>
#include <iostream>
#include <ostream>
#include <vector>

RSPlayer::RSPlayer() {
    srand((unsigned) time(NULL));
}

MoveDir RSPlayer::next_move() {
    MoveDir front_dir = snake->get_dir();
    MoveDir left_dir = front_dir;
    MoveDir right_dir = front_dir;

    std::vector<MoveDir> dirs;

    if(front_dir == NEUTRAL) {
        front_dir = N;
        left_dir = W;
        right_dir = E;
        MoveDir back_dir = S;
        dirs.push_back(back_dir);

    }
    else {
        left_dir.turn_left();
        right_dir.turn_right();
    }
    
    dirs.push_back(front_dir);
    dirs.push_back(left_dir);
    dirs.push_back(right_dir);

    auto it = dirs.begin();
    while (it != dirs.end()) {
        TilePos next_loc = snake->get_body().front();
        next_loc.row += (*it).dy;
        next_loc.col += (*it).dx;
        if(running_level->crashed(next_loc) && next_loc != snake->get_body().back()) {
            dirs.erase(it);
        }
        else {
            ++it;
        }
    }

    if(dirs.empty()) {
        return front_dir;
    }
    return random_chance(dirs);
}

void RSPlayer::bind_level(SnakeLevel* level) {
    this->running_level = level;
}

void RSPlayer::bind_snake(Snake* snake) {
    this->snake = snake;
}

MoveDir RSPlayer::random_chance(std::vector<MoveDir> dirs) {
    int index = rand() % dirs.size();

    return dirs[index];
}
