#ifndef SNAKE_H
#define SNAKE_H

#include <deque>

#include "structs.h"
#include "SnakeLevel.h"

/// @brief Class that represents a snake
class Snake {
private:
    std::deque<TilePos> body; //!< Deque of positions that represent the snake's body
    MoveDir dir; //!< Direction the snake is moving in
    unsigned short lives; //!< Number of lives the snake has
    SnakeLevel *running_level; //!< Pointer to the level the snake is playing on

public:
    /// @brief Returns the body of the snake
    std::deque<TilePos>& get_body();
    /// @brief Returns the direction the snake is moving in
    MoveDir get_dir();
    /// @brief Returns the number of lives the snake has
    unsigned short& get_lives();
    /// @brief Returns the level the snake is playing on
    SnakeLevel *get_level();

    /// @brief Sets the direction the snake is moving in
    void set_dir(MoveDir dir);
    /// @brief Sets the number of lives the snake has
    void set_lives(unsigned short lives);

    /// @brief Moves the snake one step forward
    void step();
    /// @brief Returns the next location the snake will move to
    TilePos get_next_location();
    /// @brief Binds the snake to a level
    void bind_level(SnakeLevel *level);
    /// @brief Removes the snake from the level and clears the body
    void clear();
    /// @brief Initializes the snake
    void init();
};

#endif  // SNAKE_H
