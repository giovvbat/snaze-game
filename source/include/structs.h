#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

/// @brief Struct that represents a tile position
struct TilePos {
    size_t row; //!< Row of the tile
    size_t col; //!< Column of the tile

    /// @brief Equality operator
    bool operator==(TilePos pos) {
        return row == pos.row && col == pos.col;
    }

    /// @brief Inequality operator
    bool operator!=(TilePos pos) {
        return row != pos.row || col != pos.col;
    }
};

/// @brief Enum that represents the tile type
enum tile_type_e {
    Empty = 0,
    Wall,
    InvWall,
    Food,
    SnakeHead,
    SnakeBody
};

/// @brief Struct that represents a move direction
struct MoveDir {
    short dx; //!< Change in x, positive is right
    short dy; //!< Change in y, positive is down

    /// @brief Constructor
    MoveDir(short dx = 0, short dy = 0) {
        this->dx = dx;
        this->dy = dy;
    }

    /// @brief Turns the direction left
    void turn_left(){
        short aux = dy;
        dy = -dx;
        dx = aux;
    }
    /// @brief Turns the direction right
    void turn_right(){
        short aux = dy;
        dy = dx;
        dx = -aux;
    }

    /// @brief Equality operator
    bool operator==(const MoveDir& other) {
        return dx == other.dx && dy == other.dy;
    }

    /// @brief Inequality operator
    bool operator!=(const MoveDir& other) {
        return dx != other.dx || dy != other.dy;
    }
};

/// @brief North direction
const MoveDir N(0, -1);
/// @brief South direction
const MoveDir S(0, 1);
/// @brief East direction
const MoveDir E(1, 0);
/// @brief West direction
const MoveDir W(-1, 0);
/// @brief Neutral direction
const MoveDir NEUTRAL(0, 0);

/// @brief Enum that represents the game result
enum game_result_e {
    OK = 0,
    ERROR
};

/// @brief Struct that represents the game result, including a message
struct GameResult {
    game_result_e type; //!< Type of the result
    std::string message; //!< Message of the result

    /// @brief Constructor
    GameResult(game_result_e type, std::string message = "") :
        type(type), message(message) {}
};

/// @brief Enum that represents the game state
enum game_state_e {
    START = 0,
    RUNNING,
    CRASHED,
    LEVEL_UP,
    GAME_OVER
};

#endif  // STRUCTS_H
