#include "snake.h"

Snake::Snake(){
    // init snake body with 2 node
    vector<int> node = { 0,1 };
    snake_body.push_back(node);
    node = { 1,1 };
    snake_body.push_back(node);

    // init snake's direction
    direction = 'R';

    // init snake's head postion
    head_x = 1;
    head_y = 1;

    score = 0;
}

// control snake movement
int Snake::moveSnake(){
    // change the direction of head
    if (direction == 'U') head_y--;
    else if (direction == 'D') head_y++;
    else if (direction == 'L') head_x--;
    else if (direction == 'R') head_x++;

    // check is crash
    int head_site = 16 * head_y + head_x;
    if (head_x < 0 || head_x > 15 || head_y < 0 || head_y > 15 || board_marks[head_site] == 0) {
        return 0;
    }

      // eat food
      if (board_marks[head_site] == 2) {
        board_marks[head_site] = 0;
        vector<int> temp_node = { head_x,head_y };
        snake_body.push_back(temp_node);
        snake_length++;
        score++;
        return 2;
      }

      // above cases passed, the empty box
      // set tail as 1, and head as 0
      int temp_site = snake_body[0][1] * 16 + snake_body[0][0];
      board_marks[temp_site] = 1;
      board_marks[head_site] = 0;

      // add head and remove tail from board array
      vector<int> temp_node = { head_x,head_y };
      snake_body.push_back(temp_node);
      snake_body.erase(snake_body.begin());
      return 1;
}

// change snake direction
void Snake::changeDirection(const char &c){
    direction = c;
}
