#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "SPlayer.h"
#include "Snake.h"
#include "SnakeLevel.h"
#include "structs.h"

#include <fstream>
#include <vector>

/// @brief Class that represents the snake game
class SnakeGame {
private:
    std::vector<SnakeLevel> levels; //!< Vector of levels
    Snake snake; //!< Snake
    SPlayer *player; //!< Player
    size_t running_level; //!< Index of the running level
    game_state_e state; //!< Current game state
    int n_food_per_level; //!< Number of food per level
    int consumed_food; //!< Number of food consumed
    float score; //!< Score
    size_t total_lives; //!< Total number of lives
    size_t fps; //!< Frames per second

public:
    /// @brief Constructor
    SnakeGame();
    /// @brief Destructor
    ~SnakeGame();
    /// @brief Initializes the game
    GameResult init(int argc, char *argv[]);
    /// @brief Runs the game
    void update();
    /// @brief Processes events
    void process_events();
    /// @brief Renders the game
    void render();
    /// @brief Returns true if the game is over
    bool game_over();

private:
    /// @brief Reads the levels from the given file
    GameResult read_levels(std::string level_file_name);
    /// @brief Returns the usage of the game
    GameResult usage();
    /// @brief Initializes the level
    void init_level();

};

#endif 
//SNAKE_GAME_H
