#include <ctime>
#include <random>
#include "food.h"

Food::Food(Snake* snake){
    srand((unsigned)time(NULL));
    int site_food = rand() % (256 - snake->snake_length);
    int i = 0;
    for (; i < 256; i++) {
    if (snake->board_marks[i] == 1) {
      site_food--;
      if (site_food == -1) break;
    }
    }
    x = i % 16;
    y = i / 16;
    snake->board_marks[i] = 2;
}

void Food::new_food(Snake* snake){
    srand((unsigned)time(NULL));
    int site_food = rand() % (256 - snake->snake_length);
    int i = 0;
    for (; i < 256; i++) {
    if (snake->board_marks[i] == 1) {
      site_food--;
      if (site_food == -1) break;
    }
    }
    x = i % 16;
    y = i / 16;
    snake->board_marks[i] = 2;
}
