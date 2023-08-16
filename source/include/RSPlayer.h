#ifndef RSPLAYER_H
#define RSPLAYER_H

#include "SPlayer.h"
#include "Snake.h"
#include "SnakeLevel.h"
#include "structs.h"

/// @brief Player that moves randomly
class RSPlayer : public SPlayer {
private:
    SnakeLevel* running_level; //!< Pointer to the level the snake is playing on
    Snake* snake; //!< Pointer to the snake

public:
    /// @brief Constructor
    RSPlayer();
    /// @brief Returns a random move
    MoveDir next_move();
    /// @brief Binds the player to a level
    void bind_level(SnakeLevel* level);
    /// @brief Binds the player to a snake
    void bind_snake(Snake* snake);

private:
    /// @brief Returns a random move from the given vector of moves
    MoveDir random_chance(std::vector<MoveDir> dirs);

};

#endif //RSPLAYER_H
