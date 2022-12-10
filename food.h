#pragma once
#include "food.h"
#include "snake.h"

class Food {
public:
    int x, y;
    Food(Snake* snake);
    void new_food(Snake* snake);
};

