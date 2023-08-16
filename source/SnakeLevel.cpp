#include "include/SnakeLevel.h"
#include "include/structs.h"
#include <ctime>
#include <iostream>
#include <ostream>
#include <random>
#include <stdexcept>

void SnakeLevel::add_line(std::string line) { board.push_back(line); }

void SnakeLevel::place_food() {
    std::srand((unsigned) time(NULL));

    auto spaces = empty_spaces();
    if(spaces.size() == 0)
        throw std::runtime_error("");

    size_t index = rand() % spaces.size();
    set_tile_type(spaces[index], Food);

    food_loc = spaces[index];
}

std::vector<TilePos> SnakeLevel::empty_spaces() {
    std::vector<TilePos> empty;
    for(auto i=0; i<n_rows(); ++i) {
        for(auto j=0; j<n_cols(); ++j) {
            if(board[i][j] == ' ') {
                TilePos tile;
                tile.row = i;
                tile.col = j;
                if(tile != snake_spawn_loc)
                    empty.push_back(tile);
            }
        }
    }
    return empty;
}

size_t SnakeLevel::n_rows() { return board.size(); }

size_t SnakeLevel::n_cols() { return board[0].size(); }

tile_type_e SnakeLevel::get_tile_type(TilePos pos) {
    std::string tiles = " #.&*@";
    for(auto i=0; i<tiles.size(); ++i) {
        if(board[pos.row][pos.col] == tiles[i])
            return static_cast<tile_type_e>(i);
    }
    return Empty;
}

TilePos SnakeLevel::get_spawn_loc() {
    return snake_spawn_loc;
}

void SnakeLevel::set_tile_type(TilePos pos, tile_type_e type) {
    std::string tiles = " #.&*@";
    board[pos.row][pos.col] = tiles[type];
}

bool SnakeLevel::has_spawn_location() {
    for(auto i=0; i<n_rows(); ++i) {
        for(auto j=0; j<n_cols(); ++j) {
            if(board[i][j] == '*') {
                snake_spawn_loc.row = i;
                snake_spawn_loc.col = j;
                return true;
            }
        }
    }
    return false;
}

bool SnakeLevel::crashed(TilePos tile) {
    //checar a ultima parte da cobra
    std::string tiles = "#.@";
    for(auto i=0; i<tiles.size(); ++i) {
        if(board[tile.row][tile.col] == tiles[i])
            return true;
    }
    return false;
}
