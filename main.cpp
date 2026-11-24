
#include <ncurses.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int ROWS = 15;
const int GAME_COLS = 31;

int playerX = 1;
int playerY = 1;

int score = 0;
int currentLevel = 1;
int coinsLeft = 0;

bool gameRunning = true;
bool levelComplete = false;

vector<string> createMaze(int level) {

    vector<string> maze(ROWS, string(COLS, '.'));

    for (int i = 0; i < ROWS; i++) {
        maze[i][0] = '#';
        maze[i][COLS - 1] = '#';
    }

    for (int j = 0; j < COLS; j++) {
        maze[0][j] = '#';
        maze[ROWS - 1][j] = '#';
    }

    int gaps[3][3] = {
        {3, 7, 11},
        {5, 9, 2},
        {10, 4, 8}
    };

    int wallColumns[3] = {7, 14, 21};

    for (int wall = 0; wall < 3; wall++) {

        for (int row = 1; row < ROWS - 1; row++) {

            if (row != gaps[level - 1][wall]) {
                maze[row][wallColumns[wall]] = '#';
            }
        }
    }

    maze[1][1] = 'S';

    maze[13][29] = 'E';

    if (level == 1) {

        maze[2][3] = '$';
        maze[5][4] = '$';
        maze[5][10] = '$';
        maze[10][11] = '$';
        maze[8][17] = '$';
        maze[12][25] = '$';

    }
    else if (level == 2) {

        maze[2][3] = '$';
        maze[11][4] = '$';
        maze[7][10] = '$';
        maze[2][11] = '$';
        maze[8][17] = '$';
        maze[12][25] = '$';
        maze[3][26] = '$';

    }
    else {

        maze[2][3] = '$';
        maze[11][4] = '$';
        maze[7][10] = '$';
        maze[2][11] = '$';
        maze[8][17] = '$';
        maze[3][18] = '$';
        maze[12][25] = '$';
        maze[3][26] = '$';

    }

    return maze;
}
