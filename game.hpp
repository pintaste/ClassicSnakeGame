#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <QDebug>

using std::pair;
using std::vector;
const int BOARD_SIZE = 16;

class Game
{
public:
    // Moving direction:‘U’==Up, ‘D’=Down, ‘L’=Left, ‘R’=Right
    char snake_direction;
    // snake's length
    int snake_length;
    // snake's head coordinate
    pair<int, int> snake_head;
    // 2d-array store the coordinates of body
    vector<vector<int>> snake_body;
    // 0: empty, 1: snake, 2: food
    vector<int> board;
    // food coordinate
    pair<int, int> food;
    // game score
    int score;

    // constructor
    Game()
    {
        // init game board with all 0 (empty)
        board = std::vector<int>(BOARD_SIZE * BOARD_SIZE, 0);
        // init snake's direction
        snake_direction = 'R';
        snake_length = 2;
        // init snake's head
        snake_head.first = 1;
        snake_head.second = 1;
        // init snake's body
        std::vector<int> node = {0, 1};
        snake_body.push_back(node);
        node = {1, 1};
        snake_body.push_back(node);
        // update val in board vector
        for(int i = 0; i < snake_length; i++)
        {
            board[BOARD_SIZE * snake_body[i][1] + snake_body[i][0]] = 1;
        }
        gen_food();
        score = 0;
    }
    std::pair<int,int> gen_food()
    {
        // Create a random number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        // Check if the number of occupied points is close to the maximum possible number
        if (std::count(board.begin(), board.end(), 1) >= 240)
        {

            return std::make_pair(-1,-1);
        }
        // Generate random x and y coordinates for the point
        std::uniform_int_distribution<> dis(0, 15);
        do
        {
            food = std::make_pair(dis(gen), dis(gen));
        } while (board[BOARD_SIZE * food.second + food.first] == 1);
        // Keep generating points until an unoccupied one is found

        // update food info in the board
        board[BOARD_SIZE * food.second + food.first] = 2;
    }
    // control snake movement
    int moveSnake()
    {
        // 0: Collision 
        // 1: Normal Move 
        // 2: Eat Food

        // change the direction of head
        if (snake_direction == 'U')
            snake_head.second--;
        else if (snake_direction == 'D')
            snake_head.second++;
        else if (snake_direction == 'L')
            snake_head.first--;
        else if (snake_direction == 'R')
            snake_head.first++;

        int head_site = BOARD_SIZE * snake_head.second + snake_head.first;

        if (snake_head.first < 0 || snake_head.first > 15 || snake_head.second < 0 || snake_head.second > 15 || board[head_site] == 1)
        {
            return 0;
        }

        if (board[head_site] == 2)
        {
            board[head_site] = 1;
            vector<int> temp_node = {snake_head.first, snake_head.second};
            snake_body.push_back(temp_node);
            snake_length++;
            score ++;
            return 2;
        }

        // The empty box, move the snake
        int temp_site = snake_body[0][1] * BOARD_SIZE + snake_body[0][0];
        board[temp_site] = 0;
        board[head_site] = 1;

        // add head and remove tail from board array
        vector<int> temp_node = {snake_head.first, snake_head.second};
        snake_body.push_back(temp_node);
        snake_body.erase(snake_body.begin());
        return 1;
    }
};
