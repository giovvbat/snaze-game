#ifndef SNAKE_LEVEL_H
#define SNAKE_LEVEL_H

#include "structs.h"
#include <string>
#include <vector>

/// @brief Class that represents a snake level
class SnakeLevel {
public:
    std::vector<std::string> board; //!< Vector of strings that represent the board
    TilePos snake_spawn_loc; //!< Location the snake spawns at
    TilePos food_loc; //!< Location of the food

public:
    /// @brief Returns a vector of empty spaces on the board, not including the
    /// snake spawn location
    std::vector<TilePos> empty_spaces();
    /// @brief Returns the number of columns on the board
    size_t n_cols();
    /// @brief Returns the number of rows on the board
    size_t n_rows();
    /// @brief Returns the tile type at the given position
    tile_type_e get_tile_type(TilePos pos);
    /// @brief Returns the spawn location of the snake
    TilePos get_spawn_loc();

    /// @brief Adds a line to the board
    void add_line(std::string line);
    /// @brief Sets the tile type at the given position
    void set_tile_type(TilePos pos, tile_type_e type);

    /// @brief Returns true if the board has a spawn location, and sets the
    /// snake_spawn_loc variable
    bool has_spawn_location();
    /// @brief Places food on a random empty space on the board
    void place_food();
    /// @brief Returns true if the given tile is a wall or part of the snake
    bool crashed(TilePos tile);
};

#endif  // SNAKE_LEVEL_H
