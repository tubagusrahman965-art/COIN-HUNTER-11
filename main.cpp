
#include <ncurses.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// KONSTANTA GAME
// ==========================================

const int ROWS = 15;
const int GAME_COLS = 31;

// ==========================================
// VARIABEL GAME
// ==========================================

int playerX = 1;
int playerY = 1;

int score = 0;
int currentLevel = 1;
int coinsLeft = 0;

bool gameRunning = true;
bool levelComplete = false;

// ==========================================
// MEMBUAT PETA LABIRIN
// ==========================================

vector<string> createMaze(int level) {

    vector<string> maze(ROWS, string(COLS, '.'));

    // Membuat dinding luar
    for (int i = 0; i < ROWS; i++) {
        maze[i][0] = '#';
        maze[i][COLS - 1] = '#';
    }

    for (int j = 0; j < COLS; j++) {
        maze[0][j] = '#';
        maze[ROWS - 1][j] = '#';
    }

    // Posisi celah pada setiap level
    int gaps[3][3] = {
        {3, 7, 11},
        {5, 9, 2},
        {10, 4, 8}
    };

    int wallColumns[3] = {7, 14, 21};

    // Membuat dinding labirin
    for (int wall = 0; wall < 3; wall++) {

        for (int row = 1; row < ROWS - 1; row++) {

            if (row != gaps[level - 1][wall]) {
                maze[row][wallColumns[wall]] = '#';
            }
        }
    }

    // Posisi awal pemain
    maze[1][1] = 'S';

    // Pintu keluar
    maze[13][29] = 'E';

    // Menempatkan koin
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

// ==========================================
// MENU UTAMA
// ==========================================

void showMenu() {

    clear();

    mvprintw(3, 10, "================================");
    mvprintw(5, 17, "MAZE ADVENTURE");
    mvprintw(7, 10, "================================");

    mvprintw(10, 13, "1. START GAME");
    mvprintw(11, 13, "2. HOW TO PLAY");
    mvprintw(12, 13, "3. EXIT");

    mvprintw(15, 8, "Pilih menu menggunakan keyboard");

    refresh();
}

// ==========================================
// PETUNJUK PERMAINAN
// ==========================================

void showHelp() {

    clear();

    mvprintw(3, 12, "CARA BERMAIN");

    mvprintw(6, 8, "W / Panah Atas    : Bergerak ke atas");
    mvprintw(7, 8, "S / Panah Bawah   : Bergerak ke bawah");
    mvprintw(8, 8, "A / Panah Kiri    : Bergerak ke kiri");
    mvprintw(9, 8, "D / Panah Kanan   : Bergerak ke kanan");

    mvprintw(11, 8, "P : Pause");
    mvprintw(12, 8, "Q : Kembali ke menu");

    mvprintw(15, 8, "Kumpulkan semua koin ($).");
    mvprintw(16, 8, "Temukan pintu keluar (E).");

    mvprintw(19, 8, "Tekan tombol apa saja untuk kembali.");

    refresh();

    getch();
}

// ==========================================
// MENGGAMBAR LABIRIN
// ==========================================

void drawMaze(vector<string>& maze) {

    clear();

    mvprintw(0, 5, "========= MAZE ADVENTURE =========");

    mvprintw(1, 5, "Level : %d / 3", currentLevel);
    mvprintw(2, 5, "Score : %d", score);
    mvprintw(3, 5, "Koin tersisa : %d", coinsLeft);

    mvprintw(4, 5, "Kumpulkan semua koin untuk membuka pintu!");

    for (int i = 0; i < ROWS; i++) {

        for (int j = 0; j < COLS; j++) {

            char symbol = maze[i][j];

            if (symbol == 'S') {
                symbol = '.';
            }

            if (i == playerY && j == playerX) {
                symbol = 'P';
            }

            mvaddch(i + 6, j + 5, symbol);
        }
    }

    mvprintw(22, 5, "WASD / Arrow : Bergerak");
    mvprintw(23, 5, "P : Pause | Q : Kembali ke menu");

    refresh();
}

// ==========================================
// SISTEM PAUSE
// ==========================================

void pauseGame() {

    timeout(-1);

    mvprintw(25, 5, "GAME PAUSED - Tekan tombol untuk lanjut");
    refresh();

    getch();

    timeout(100);
}

// ==========================================
// SISTEM PERMAINAN
// ==========================================

void playGame() {

    score = 0;

    for (currentLevel = 1; currentLevel <= 3; currentLevel++) {

        vector<string> maze = createMaze(currentLevel);

        playerX = 1;
        playerY = 1;

        levelComplete = false;

        // Menghitung jumlah koin
        coinsLeft = 0;

        for (int i = 0; i < ROWS; i++) {

            for (int j = 0; j < COLS; j++) {

                if (maze[i][j] == '$') {
                    coinsLeft++;
                }
            }
        }

        timeout(100);

        while (!levelComplete && gameRunning) {

            drawMaze(maze);

            int key = getch();

            if (key == ERR) {
                continue;
            }

            // Keluar ke menu
            if (key == 'q' || key == 'Q') {
                timeout(-1);
                return;
            }

            // Pause
            if (key == 'p' || key == 'P') {
                pauseGame();
                continue;
            }

            int nextX = playerX;
            int nextY = playerY;

            // Kontrol keyboard
            if (key == KEY_UP || key == 'w' || key == 'W') {
                nextY--;
            }

            else if (key == KEY_DOWN || key == 's' || key == 'S') {
                nextY++;
            }

            else if (key == KEY_LEFT || key == 'a' || key == 'A') {
                nextX--;
            }

            else if (key == KEY_RIGHT || key == 'd' || key == 'D') {
                nextX++;
            }

            else {
                continue;
            }

            // Memeriksa batas labirin dan tembok
            if (nextX < 0 || nextX >= COLS ||
                nextY < 0 || nextY >= ROWS) {
                continue;
            }

            if (maze[nextY][nextX] == '#') {
                continue;
            }

            char destination = maze[nextY][nextX];

            // Mengambil koin
            if (destination == '$') {

                score += 10;
                coinsLeft--;

                maze[nextY][nextX] = '.';
            }

            // Memeriksa pintu keluar
            if (destination == 'E') {

                if (coinsLeft == 0) {

                    levelComplete = true;

                }
                else {

                    mvprintw(25, 5,
                        "Pintu terkunci! Kumpulkan semua koin.");

                    refresh();
                    napms(1000);
                }
            }

            playerX = nextX;
            playerY = nextY;
        }

        if (!gameRunning) {
            break;
        }

        timeout(-1);

        // Tampilan level selesai
        clear();

        if (currentLevel < 3) {

            score += 50;

            mvprintw(8, 12, "LEVEL %d SELESAI!", currentLevel);

            mvprintw(10, 12, "Bonus: +50 poin");

            mvprintw(13, 8,
                "Tekan tombol apa saja untuk lanjut.");

        }
        else {

            score += 100;

            mvprintw(7, 8, "SELAMAT! SEMUA LEVEL SELESAI!");

            mvprintw(10, 12, "SKOR AKHIR: %d", score);

            mvprintw(13, 8,
                "Tekan tombol apa saja untuk kembali ke menu.");
        }

        refresh();

        getch();
    }

    timeout(-1);
}

// ==========================================
// PROGRAM UTAMA
// ==========================================

int main() {

    initscr();

    noecho();

    curs_set(0);

    keypad(stdscr, TRUE);

    gameRunning = true;

    while (gameRunning) {

        showMenu();

        int choice = getch();

        if (choice == '1') {

            playGame();

        }

        else if (choice == '2') {

            showHelp();

        }

        else if (choice == '3' ||
                 choice == 'q' ||
                 choice == 'Q') {

            gameRunning = false;

        }
    }

    endwin();

    return 0;
}
