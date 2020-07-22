#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W 60
#define H 20
typedef struct {
    int x, y;
} COORDINATE;

COORDINATE *initRobots(int n);

void createRobots(COORDINATE *robots, int n);

int randint(int min, int max);

void displayBoard(COORDINATE *robots, COORDINATE player, int n);

int main() {
    int n = 5;
    srand((unsigned int) time(NULL));
    COORDINATE *robots = initRobots(n);
    COORDINATE player;
    player.x = 10;
    player.y = 10;
    initRobots(n);
    createRobots(robots, n);
    for (int i = 0; i < n; ++i) {
        printf("%d,%d\n", robots[i].x, robots[i].y);
    }
    displayBoard(robots, player, n);

    return 0;
}

COORDINATE *initRobots(int n) {
    return (COORDINATE *) malloc(n * sizeof(COORDINATE));
}

void createRobots(COORDINATE *robots, int n) {
    for (int i = 0; i < n; ++i) {
        robots[i].x = randint(0, W - 1);
        robots[i].y = randint(0, H - 1);
    }
}

int randint(int min, int max) {
    return min + (int) (rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

void displayBoard(COORDINATE *robots, COORDINATE player, int n) {
    char board[H][W];
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            board[i][j] = ' ';
        }
    }

    for (int i = 0; i < n; ++i) {
        board[robots[i].y][robots[i].x] = '+';
    }
    board[player.y][player.x] = '@';

    printf("+");
    for (int i = 0; i < W; ++i) {
        printf("-");
    }
    printf("+\n");

    for (int i = 0; i < H; ++i) {
        printf("|");
        for (int j = 0; j < W; ++j) {
            printf("%c", board[i][j]);
        }
        printf("|\n");
    }
    printf("+");

    for (int i = 0; i < W; ++i) {
        printf("-");
    }
    printf("+\n");
}