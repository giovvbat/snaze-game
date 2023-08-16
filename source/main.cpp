/**!
 *  This program implements the Snaze simulation.
 *
 *  @author Selan R. dos Santos
 *
 *  To compile: use cmake script.
 */

#include <cstdlib> // EXIT_SUCCESS
#include <iostream>

#include "include/SnakeGame.h"
#include "include/structs.h"

int main(int argc, char *argv[]) {
  SnakeGame simulation;

  auto result = simulation.init(argc, argv);
  if (result.type == game_result_e::ERROR) {
    std::cerr << result.message << "\n";
    return EXIT_FAILURE;
  }

  

  //Initial message.
  simulation.render();
  // // The Game Loop.
  while (not simulation.game_over()) {
    simulation.process_events();
    simulation.update();
    simulation.render();
  }

  return EXIT_SUCCESS;
}
