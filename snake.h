#pragma once

#include <vector>
using std::vector;

class Snake {
public:
    // snake moving direction: ‘U’==Up，‘D’=Down，‘L’=Left，‘R’=Right
    char direction;
    // 2d-array store the coordinates of body
    vector<vector<int>> snake_body;
    // snake's head coordinate
    int head_x, head_y;
    // snake's body length
    int snake_length;
    vector<int> board_marks{vector<int>(256,1)};
    int score;

    // constructor
    Snake();
    // control snake movement
    int moveSnake();
    // change snake direction
    void changeDirection(const char &c);
};

