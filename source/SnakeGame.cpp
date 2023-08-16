#include "include/SnakeGame.h"
#include "include/BTSPlayer.h"
#include "include/RSPlayer.h"
#include "include/structs.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <cstdlib>

SnakeGame::SnakeGame() {
    player = nullptr;
}

SnakeGame::~SnakeGame() {
    delete player;
}

GameResult SnakeGame::init(int argc, char *argv[]) {
    if(argc == 1) {
        return usage();
    } //usage

    fps = 10;
    n_food_per_level = 10;
    total_lives = 5;

    for(size_t i = 1; i<argc; ++i) {
        std::string word = argv[i];

        for(char c : word) {
            c = std::tolower(c);
        }

        if(word == "-h" || word == "--h" || word == "--help") {
            return usage();
        }

        if(word == "-f" || word == "--f" || word == "--fps") {
            if(i == argc-1){
                return GameResult(game_result_e::ERROR, "No arguments given for fps");
            }
            int fps;

            try {
                fps = std::stoi(argv[i+1]);
            }
            catch(std::invalid_argument &e) {
                return GameResult(game_result_e::ERROR, "Argument given for fps is not a number");
            }

            if(fps < 1 || fps > 60){
                return GameResult(game_result_e::ERROR, "Value for fps must be between 1 and 60");
            }
            this->fps=fps;
        }

        if(word == "--food") {
            if(i == argc-1){
                return GameResult(game_result_e::ERROR, "No arguments given for food");
            }
            int food;

            try {
                food = std::stoi(argv[i+1]);
            }
            catch(std::invalid_argument &e) {
                return GameResult(game_result_e::ERROR, "Argument given for food is not a number");
            }

            if(food < 1 || food > 50){
                return GameResult(game_result_e::ERROR, "Value for food must be between 1 and 50");
            }
            n_food_per_level = food;
        }

        if(word == "-l" || word == "--l" || word == "--lives") {
            if(i == argc-1){
                return GameResult(game_result_e::ERROR, "No arguments given for lives");
            }
            int lives;

            try {
                lives = std::stoi(argv[i+1]);
            }
            catch(std::invalid_argument &e) {
                return GameResult(game_result_e::ERROR, "Argument given for lives is not a number");
            }

            if(lives < 1 || lives > 10){
                return GameResult(game_result_e::ERROR, "Value for lives must be between 1 and 10");
            }
            total_lives = lives;
        }

        if(word == "-p" || word == "--p" || word == "--player") {
            if(i == argc-1) {
                return GameResult(game_result_e::ERROR, "No arguments given for player");
            }
            std::string type = argv[i+1];

            if(type == "random") {
                player = new RSPlayer();
            }
            else if(type == "backtracking") {
                player = new BTSPlayer();
            }
            else {
                return GameResult(game_result_e::ERROR, "Unknown player type");
            }
        }

        if(i == argc-1) {
            GameResult read_result = read_levels(argv[i]);
            if(read_result.type == game_result_e::ERROR) {
                return read_result;
            }
        }
    }

    if(player == nullptr) {
        player = new BTSPlayer();
    }
    
    snake.set_lives(total_lives);
    running_level = 0;
    state = START;
    consumed_food = 0;
    score = 0;

    return GameResult(game_result_e::OK);
}

void SnakeGame::process_events() {
    if(state == START || state == CRASHED || state == LEVEL_UP) {
        std::cin.get();
    }
    else if(state == RUNNING) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/fps));
    }
}

void SnakeGame::update() {
    if(state == START) {
        init_level();
        state = RUNNING;
    }
    else if(state == RUNNING) {
        snake.set_dir(player->next_move());
        TilePos next_loc = snake.get_next_location();
        SnakeLevel level = levels[running_level];

        if(level.crashed(next_loc) && next_loc != snake.get_body().back()) {
            --snake.get_lives();
            if(snake.get_lives() == 0) {
                state = GAME_OVER;
                return;
            }
            state = CRASHED;
            return;
        }

        if(level.get_tile_type(next_loc) == Food) {
            ++consumed_food;
            snake.step();
            if(consumed_food == n_food_per_level && running_level == levels.size() - 1) {
                score += 10000;
                state = GAME_OVER;
            }
            else if(consumed_food == n_food_per_level)
                state = LEVEL_UP;
            else {
                init_level();
                state = RUNNING;
            }
            return;
        }
        snake.step();

    }
    else if(state == CRASHED) {
        if(snake.get_lives() > 0) {
            init_level();
            state = RUNNING;
            return;
        }
        state = GAME_OVER;
        return;
    }
    else if(state == LEVEL_UP) {
        if(running_level < levels.size() - 1) {
            ++running_level;
            init_level();
            state = RUNNING;
            return;
        }
    }
}

void SnakeGame::init_level() {
    if(state == START) {
        running_level = 0;
        consumed_food = 0;
        score = 0;
        levels[running_level].place_food();
        snake.bind_level(&levels[running_level]);
        snake.init();
        player->bind_level(&levels[running_level]);
        player->bind_snake(&snake);
    }
    else if(state == RUNNING) {
        levels[running_level].place_food();
        score += 100;
    }
    else if(state == CRASHED) {
        snake.clear();
        snake.init();
    }
    else if(state == LEVEL_UP) {
        consumed_food = 0;
        score += 1100;
        snake.clear();
        snake.bind_level(&levels[running_level]);
        player->bind_level(&levels[running_level]);
        snake.init();
        levels[running_level].place_food();
    }
}

bool SnakeGame::game_over() { return state == game_state_e::GAME_OVER; }

GameResult SnakeGame::usage() {
    std::ostringstream oss;
    oss << "Usage: snaze [<options>] <input_level_file>\n"
           "    Game simulation options:\n"
           "        --help              Print this help text.\n"
           "        --fps <num>         Number of frames presented per second. Default = 10.\n"
           "        --lives <num>       Number of lives the snake shall have. Default = 5.\n"
           "        --food <num>        Amount of food per level. Default = 10.\n"
           "        --playertype <type> Type of snake intelligence: random, backtracking. Default = backtracking\n";

    return GameResult(game_result_e::ERROR, oss.str());
}

GameResult SnakeGame::read_levels(std::string level_file_name) {
    std::ifstream file(level_file_name);
    if(!file.is_open())
        return GameResult(game_result_e::ERROR, "File could not be read");
    std::string line;
    while(getline(file, line)) {
        SnakeLevel level;
        std::istringstream ss(line);
        int rows, columns;
        ss>>rows>>columns;
        if(rows>100 || columns>100 || rows<1 || columns<1) {
            return GameResult(game_result_e::ERROR, "File could not be read");
        }
        for(auto i=0; i<rows; i++) {
            getline(file, line);
            if(line[line.size() - 1] == '\r') {
                line.pop_back();
            }
            for(auto i:line) {
                if(i != '#' && i != '*' && i != '.' && i != ' ')
                    return GameResult(game_result_e::ERROR, "File could not be read");
            }
            level.add_line(line);
        }
        if(not level.has_spawn_location()) {
            return GameResult(game_result_e::ERROR, "File could not be read");
        }
        levels.push_back(level);
    }
    if(levels.empty()) 
        return GameResult(game_result_e::ERROR, "File could not be read");
    return GameResult(game_result_e::OK);
}

void SnakeGame::render() {
    std::system("clear");
    std::ostringstream board;
    if(state == LEVEL_UP) {
        board<<">>> Press <ENTER> to start next level!"<<std::endl;
    }
    else if(state == GAME_OVER) {
        if(consumed_food == n_food_per_level) {
            board<<">>> YAY! CONGRATULATIONS anaconda WON!"<<std::endl;
            board<<">>> Thanks for playing!"<<std::endl;
        }
        else {
            board<<">>> OH NO! GAME OVER, better luck next time!"<<std::endl;
        }
    }
    else if(state == CRASHED) {
        board<<">>> Press <ENTER> to try again!"<<std::endl;
    }
    else if(state == START) {
        board<<"  ---> Welcome to the classic Snake Game <---  \n";
        board<<"copyright Giovanna Batista & Pedro Vinicius 2023\n";
        board<<"------------------------------------------------------\n";
        board<<"Levels loaded: "<<levels.size()<<" | "<<"Snake lives: "<<snake.get_lives()<<" | "<<"Apples to eat: "<<n_food_per_level<<'\n';
        board<<"Clear all levels to win the game. Good luck!!\n";
        board<<"------------------------------------------------------\n";
        board<<">>> Press <ENTER> to start playing!"<<std::endl;
    }
    std::string tiles[] = {" ", "█", " ", "", "", "●"};
    board<<"Lives: ";
    for(auto i=0; i<snake.get_lives(); ++i)
        board<<"";
    for(auto j=0; j<total_lives-snake.get_lives(); j++)
        board<<"";
    board<<" | Score: "<<score;
    board<<" | Food eaten: "<<consumed_food<<" of "<<n_food_per_level<<'\n';
    board<<"Level "<<running_level+1<<" of "<<levels.size()<<" | ---> Try not to CRASH! <---\n\n";
    for(auto i=0; i<levels[running_level].n_rows(); ++i) {
        for(auto j=0; j<levels[running_level].n_cols(); ++j) {
            TilePos pos;      
            pos.col = j;
            pos.row = i;
            auto type = levels[running_level].get_tile_type(pos);
            if((state == CRASHED || (state == GAME_OVER && consumed_food != n_food_per_level)) && (type == SnakeHead || type == SnakeBody)) {
                if(type == SnakeHead)
                    board<<"󰯈";
                else if(type == SnakeBody)
                    board<<"";
            }
            else if(type == SnakeHead && snake.get_dir() != NEUTRAL) {
                if(snake.get_dir() == N)
                    board<<"";
                else if(snake.get_dir() == E)
                    board<<"";
                else if(snake.get_dir() == W)
                    board<<"";
                else if(snake.get_dir() == S)
                    board<<"";
            }
            else {                
                board<<tiles[type];
            }
        }
        board<<std::endl;
    }
    board<<std::endl;
    std::cout<<board.str();
}
