#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define width 80
#define height 25
#define max_speed 20

void input(int uni[height][width]);
void next_generation(int universe[height][width], int update[height][width]);
int alive_neighbour(int universe[height][width], int row, int column);
void copy(int old[height][width], int new[height][width]);
void output(int universe[height][width]);
char key(int *delay);
int stop(int old[height][width], int new[height][width]);
void preview();
void exit_func();
void exit_exit();

int main() {
    int delay = max_speed / 2;
    int universe[height][width];
    input(universe);
    initscr();
    noecho();
    nodelay(stdscr, true);
    if (freopen("/dev/tty", "r", stdin) != NULL) {
        preview();
        char c = ' ';
        while (c != 'P' && c != 'p' && c != 'Q' && c != 'q') c = getchar();
        if (c == 'p' || c == 'P') {
            while (c != 'q' && c != 'Q') {
                int nuniverse[height][width];
                output(universe);
                printw("current speed: %d", max_speed - delay);
                refresh();
                c = key(&delay);
                usleep(delay * 50000);
                next_generation(universe, nuniverse);
                if (stop(universe, nuniverse)) {
                    getchar();
                    exit_func();
                    usleep(2000000);
                    break;
                }
                copy(nuniverse, universe);
            }
            exit_func();
            usleep(3000000);
        } else if (c == 'Q' || c == 'q') {
            atexit(exit_exit);
        }
        atexit(exit_exit);
    }
    return 0;
}

void exit_exit() { endwin(); }

char key(int *delay) {
    char c = getch();
    if (c == '+' && (*delay) > 1) (*delay)--;

    if (c == '-' && (*delay) < 20) (*delay)++;

    return c;
}

int stop(int old[height][width], int new[height][width]) {
    int flag = 1;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (new[i][j] != old[i][j]) {
                flag = 0;
            }
        }
    }
    return flag;
}

void copy(int old[height][width], int new[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            new[i][j] = old[i][j];
        }
    }
}

void input(int uni[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            scanf("%d", &uni[i][j]);
        }
    }
}

void next_generation(int universe[height][width], int update[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            update[i][j] = 0;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int a = alive_neighbour(universe, i, j);
            if (universe[i][j] == 1) {
                if (a == 2 || a == 3) update[i][j] = 1;

            } else if (universe[i][j] == 0) {
                if (a == 3) update[i][j] = 1;

            } else {
                update[i][j] = 0;
            }
        }
    }
}

int alive_neighbour(int universe[height][width], int row, int column) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int indexI = (row + i + height) % height;
            int indexJ = (column + j + width) % width;
            if (universe[indexI][indexJ] == 1) count += 1;
        }
    }
    if (universe[row][column] == 1) count--;

    return count;
}

void output(int matrix[height][width]) {
    clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printw("%c", matrix[i][j] == 0 ? ' ' : '#');
        }
        printw("\n");
    }
}

void preview() {
    clear();
    printw("/-------------------------------------------------------------------------------\\\n");
    printw("|                                                                               |\n");
    printw("|      Game of                                                                  |\n");
    printw("|                                                                               |\n");
    printw("|      x x         x x   x x x x x   x x x x x                                  |\n");
    printw("|      x x         x x   x x x x x   x x x x x                                  |\n");
    printw("|      x x               x x         x x                                        |\n");
    printw("|      x x         x x   x x x x x   x x x x x                                  |\n");
    printw("|      x x         x x   x x x x x   x x x x x                                  |\n");
    printw("|      x x         x x   x x         x x                                        |\n");
    printw("|      x x x x x   x x   x x         x x x x x                                  |\n");
    printw("|      x x x x x   x x   x x         x x x x x                                  |\n");
    printw("|                                                                               |\n");
    printw("|      Press:                                                                   |\n");
    printw("|      P - to play                                                              |\n");
    printw("|      Q - to quit                                                              |\n");
    printw("|      + - to speed up                                                          |\n");
    printw("|      - - to slow down                                                         |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("\\-------------------------------------------------------------------------------/\n");
    refresh();
}

void exit_func() {
    clear();
    printw("/-------------------------------------------------------------------------------\\\n");
    printw("|                                                                               |\n");
    printw("|      x x x       x x x    x       x  x x x x x                                |\n");
    printw("|    x x x x x   x x x x x  x x   x x  x x x x x                                |\n");
    printw("|    x x   x x   x x   x x  x x x x x  x x                                      |\n");
    printw("|    x x         x x   x x  x x x x x  x x x x x                                |\n");
    printw("|    x x x x x   x x x x x  x x x x x  x x x x x                                |\n");
    printw("|    x x   x x   x x   x x  x x   x x  x x                                      |\n");
    printw("|    x x x x x   x x   x x  x x   x x  x x x x x                                |\n");
    printw("|      x x x     x x   x x  x x   x x  x x x x x                                |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("|                                    x x x    x x   x x  x x x x x    x x x     |\n");
    printw("|                                  x x x x x  x x   x x  x x x x x  x x x x x   |\n");
    printw("|                                  x x   x x  x x   x x  x x        x x   x x   |\n");
    printw("|                                  x x   x x  x x   x x  x x x x x  x x   x x   |\n");
    printw("|                                  x x   x x  x x   x x  x x x x x  x x x x     |\n");
    printw("|                                  x x   x x  x x   x x  x x        x x   x x   |\n");
    printw("|                                  x x x x x  x x x x x  x x x x x  x x   x x   |\n");
    printw("|                                    x x x      x x x    x x x x x  x x   x x   |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("|                                                                               |\n");
    printw("\\-------------------------------------------------------------------------------/\n");
    refresh();
}
