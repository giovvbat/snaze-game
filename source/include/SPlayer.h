#ifndef SPLAYER_H
#define SPLAYER_H

#include "Snake.h"
#include "SnakeLevel.h"
#include "structs.h"

/// @brief Abstract class for a snake player
class SPlayer {
public:
    /// @brief Destructor
    virtual ~SPlayer() = default;
    /// @brief Returns the next move
    virtual MoveDir next_move() = 0;
    /// @brief Binds the player to a snake
    virtual void bind_snake(Snake* snake) = 0;
    /// @brief Binds the player to a level
    virtual void bind_level(SnakeLevel* level) = 0;
};

#endif //SPLAYER_H
