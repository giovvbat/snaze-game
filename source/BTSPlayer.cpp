#include "include/BTSPlayer.h"
#include "include/structs.h"
#include <ctime>
#include <queue>
#include <set>
#include <stack>

BTSPlayer::BTSPlayer() {
    searched_solution = false;
    solution_found = false;
    srand((unsigned) time(NULL));
}

void BTSPlayer::bind_level(SnakeLevel* level) {
    this->running_level = level;
}

void BTSPlayer::bind_snake(Snake* snake) {
    this->snake = snake;
}

MoveDir BTSPlayer::opposite_direction(MoveDir dir) {
    dir.turn_left();
    dir.turn_left();
    return dir;
}

void BTSPlayer::find_solution() {
    // parte A

    struct State {
        TilePos position;
        std::vector<MoveDir> directions;
        std::deque<TilePos> snake_state;
    };

    solution.clear();

    std::queue<State> search; //PNE
    std::set<std::string> visited;
    std::vector<MoveDir> possible_dir;

    possible_dir.push_back(N);
    possible_dir.push_back(E);
    possible_dir.push_back(S);
    possible_dir.push_back(W);
    MoveDir neutral = NEUTRAL;

    State initial = {snake->get_body().front(), {{snake->get_dir()}}, snake->get_body()};
    search.push(initial);

    solution_found = false;

    // parte B

    while(not search.empty()) {
        auto trying = search.front();
        search.pop();
        solution = trying.directions;
        
        if(running_level->get_tile_type(trying.position) == Food) {
            solution.erase(solution.begin());
            curr_move = solution.begin();
            solution_found = true;
            return;
        }

        // parte C
        
        for(auto dir : possible_dir) {
            if(trying.directions.back() != neutral && dir == opposite_direction(trying.directions.back()))
                continue;

            TilePos pos = {trying.position.row + dir.dy, trying.position.col + dir.dx};
            auto type = running_level->get_tile_type(pos);

            bool isbody = false;
            for(auto j=0; j<trying.snake_state.size() - 1; ++j) {
                if(pos == trying.snake_state[j]) {
                    isbody = true;
                    break;
                }
            }
            if(type == Wall || type == InvWall || isbody) {
                continue;
            }

            std::string test_key = std::to_string(pos.row) + ", " + std::to_string(pos.col)+ ", " + std::to_string(dir.dx) + ", " + std::to_string(dir.dy);

            if (visited.count(test_key) == 1) {
                continue;
            }
            
            visited.insert(test_key);

            State candidate;
            candidate.position = pos;
            candidate.directions = trying.directions;
            candidate.directions.push_back(dir);
                
            //andando com a cobra
            candidate.snake_state = trying.snake_state;
            candidate.snake_state.push_front(pos);
            candidate.snake_state.pop_back();

            search.push(candidate);
        }
    }
    solution.clear();
    return;
}

MoveDir BTSPlayer::next_move() {
    //se temos uma nova comida, tentamos encontrar solucao
    if(!searched_solution || curr_move == solution.end()) {
        find_solution();
        searched_solution = true;
    }

    //se encontramos solucao, avancamos no vetor de direções
    if(solution_found) {
        return *curr_move++;
    }

    TilePos next_loc = snake->get_body().front();
    MoveDir dir = next_move_random();
    next_loc.row += dir.dy;
    next_loc.col += dir.dx;

    //aqui temos os dois casos quando posicionaremos uma nova comida e poderemos procurar por ela
    if(running_level->crashed(next_loc) && next_loc != snake->get_body().back()) {
        searched_solution = false;
    }
    if(running_level->get_tile_type(next_loc) == Food) {
        searched_solution = false;
    }
    return next_move_random();
}

MoveDir BTSPlayer::next_move_random() {
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

MoveDir BTSPlayer::random_chance(std::vector<MoveDir> dirs) {
    int index = rand() % dirs.size();

    return dirs[index];
}
