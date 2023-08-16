#ifndef BTSPLAYER_H
#define BTSPLAYER_H

#include "SPlayer.h"
#include "Snake.h"
#include "SnakeLevel.h"
#include "structs.h"

#include <vector>

/// @brief Player that uses a backtracking algorithm to find a solution
class BTSPlayer : public SPlayer {
private:
    SnakeLevel* running_level; //!< Pointer to the level the snake is playing on
    Snake* snake; //!< Pointer to the snake
    std::vector<MoveDir> solution; //!< Vector of moves that lead to the solution
    std::vector<MoveDir>::iterator curr_move; //!< Iterator to the current move
    bool solution_found; //!< Flag that indicates if a solution was found
    bool searched_solution; //!< Flag that indicates if the solution was searched for

public:
    /// @brief Constructor
    BTSPlayer();
    /// @brief Binds the player to a level
    void bind_level(SnakeLevel* level);
    /// @brief Binds the player to a snake
    void bind_snake(Snake* snake);
    /// @brief Returns the next move in the solution vector, or a random move
    /// if no solution was found
    MoveDir next_move();

private:
    /// @brief Returns the direction opposite to the given direction
    MoveDir opposite_direction(MoveDir dir);
    /// @brief Finds a solution using a breadth-first search algorithm and
    /// stores it in the solution vector
    void find_solution();
    /// @brief Returns a random move
    MoveDir next_move_random();
    /// @brief Returns a random move from the given vector of moves
    MoveDir random_chance(std::vector<MoveDir> dirs);
};

#endif //BTSPLAYER_H
