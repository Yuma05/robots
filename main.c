#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termio.h>

#define W 60
#define H 20
#define ROBOTS 40
#define SCRAP 20
#define TRUE 1
#define FALSE 0

typedef struct {
    int x, y;
} COORDINATE;

char getChar(void);

void initRobots(COORDINATE robots[ROBOTS]);

void initScrap(COORDINATE scrap[SCRAP]);

void createPlayer(COORDINATE *player, int isUsed[H][W]);

void createRobots(COORDINATE robots[ROBOTS], int robotCount, int isUsed[H][W]);

int randint(int min, int max);

void displayBoard(COORDINATE robots[ROBOTS], COORDINATE player, COORDINATE scrap[SCRAP], int robotCount);

void play(COORDINATE robots[ROBOTS], COORDINATE player, COORDINATE scrap[SCRAP], int robotCount);

void movePlayer(char command, COORDINATE *player);

void moveRobots(COORDINATE robots[ROBOTS], COORDINATE player);

int checkCollisionScrap(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]);

int checkCollisionRobots(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]);

int main() {
    int robotCount = 5;
    srand((unsigned int) time(NULL));
    COORDINATE robots[ROBOTS];
    COORDINATE scrap[ROBOTS];
    COORDINATE player;
    int isUsed[H][W] = {FALSE};
    createPlayer(&player, isUsed);
    initScrap(scrap);
    initRobots(robots);
    printf("%d  %d\n", robots[10].x, robots[10].y);
    createRobots(robots, robotCount, isUsed);
    printf("%d  %d\n", robots[10].x, robots[10].y);
//    for (int i = 0; i < n; ++i) {
//        printf("%d,%d\n", robots[i].x, robots[i].y);
//    }
    play(robots, player, scrap, robotCount);

    return 0;
}

void initRobots(COORDINATE robots[ROBOTS]) {
    for (int i = 0; i < ROBOTS - 1; ++i) {
        robots[i].x = -1;
        robots[i].y = -1;
    }
}

void initScrap(COORDINATE scrap[SCRAP]) {
    for (int i = 0; i < SCRAP - 1; ++i) {
        scrap[i].x = -1;
        scrap[i].y = -1;
    }
}

void createPlayer(COORDINATE *player, int isUsed[H][W]) {
    int newX, newY;
    int flag = TRUE;
    while (flag) {
        newX = randint(0, W - 1);
        newY = randint(0, H - 1);
        if (!isUsed[newY][newX]) {
            player->x = newX;
            player->y = newY;
            isUsed[newY][newX] = TRUE;
            flag = FALSE;
        }
    }
}

void createRobots(COORDINATE robots[ROBOTS], int robotCount, int isUsed[H][W]) {
    int newX, newY;
    for (int i = 0; i < robotCount; ++i) {
        newX = randint(0, W - 1);
        newY = randint(0, H - 1);
        if (!isUsed[newY][newX]) {
            robots[i].x = newX;
            robots[i].y = newY;
            isUsed[newY][newX] = TRUE;
        } else {
            i--;
        }
    }
}

int randint(int min, int max) {
    return min + (int) (rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

void displayBoard(COORDINATE robots[ROBOTS], COORDINATE player, COORDINATE scrap[SCRAP], int robotCount) {
    char board[H][W];
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            board[i][j] = ' ';
        }
    }

    for (int i = 0; i < ROBOTS - 1; ++i) {
        if (robots[i].x != -1 && robots[i].y != -1) {
            board[robots[i].y][robots[i].x] = '+';
        }
    }
    for (int i = 0; i < SCRAP - 1; ++i) {
        if (scrap[i].x != -1 && scrap[i].y != -1) {
            board[scrap[i].y][scrap[i].x] = '*';
        }
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

void play(COORDINATE robots[ROBOTS], COORDINATE player, COORDINATE scrap[SCRAP], int robotCount) {
    displayBoard(robots, player, scrap, robotCount);
    char command;
    while (1) {
        printf("%d  %d\n", robots[10].x, robots[10].y);
        command = getChar();
//        printf("%c",command);
        movePlayer(command, &player);
        moveRobots(robots, player);
        printf("%d  %d\n\n\n", robots[10].x, robots[10].y);
        checkCollisionRobots(robots, scrap);
        checkCollisionScrap(robots, scrap);
        displayBoard(robots, player, scrap, robotCount);

    }
}

void movePlayer(char command, COORDINATE *player) {
    COORDINATE temp;
    temp.x = player->x;
    temp.y = player->y;
    switch ((int) command - '0') {
        case 0:
            temp.x = randint(0, W - 1);
            temp.y = randint(0, H - 1);
            break;
        case 1:
            temp.x = player->x - 1;
            temp.y = player->y + 1;
            break;
        case 2:
            temp.y = player->y + 1;
            break;
        case 3:
            temp.x = player->x + 1;
            temp.y = player->y + 1;
            break;
        case 4:
            temp.x = player->x - 1;
            break;
        case 5:
            break;
        case 6:
            temp.x = player->x + 1;
            break;
        case 7:
            temp.x = player->x - 1;
            temp.y = player->y - 1;
            break;
        case 8:
            temp.y = player->y - 1;
            break;
        case 9:
            temp.x = player->x + 1;
            temp.y = player->y - 1;
            break;
        default:
            printf("Input error\n");
            break;
    }

    if (0 <= temp.x && temp.x < W && 0 <= temp.y && temp.y < H) {
        player->x = temp.x;
        player->y = temp.y;
    }
}

char getChar(void) {
    struct termio old_term, new_term;

    char c;

    /* 現在の設定を得る */
    ioctl(0, TCGETA, &old_term);

    /* 設定のコピーをつくる */
    new_term = old_term;

    /* 入力文字のエコーを抑止する場合 */
    new_term.c_lflag &= ~(ICANON | ECHO);

    /* エコーは止めない場合 */
    //new_term.c_lflag &= ~(ICANON);

    /* 新しい設定を反映する */
    ioctl(0, TCSETAW, &new_term);

    /* 1 文字入力 */
    c = getchar();

    /* 古い設定に戻す */
    ioctl(0, TCSETAW, &old_term);

    return (c);
}

void moveRobots(COORDINATE robots[ROBOTS], COORDINATE player) {
    for (int i = 0; i < ROBOTS - 1; ++i) {
        if (robots[i].x != -1 && robots[i].y != -1) {
            if (player.x < robots[i].x) {
                robots[i].x -= 1;
            } else if (player.x > robots[i].x) {
                robots[i].x += 1;
            }

            if (player.y < robots[i].y) {
                robots[i].y -= 1;
            } else if (player.y > robots[i].y) {
                robots[i].y += 1;
            }
        }
    }
}


int checkCollisionRobots(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]) {
    for (int i = 0; i < ROBOTS - 2; ++i) {
        for (int j = i + 1; j < ROBOTS - 1; ++j) {
            if (robots[i].x == robots[j].x && robots[i].y == robots[j].y) {
                for (int k = 0; k < SCRAP - 1; ++k) {
                    if (scrap[k].x == -1 && scrap[k].y == -1) {
                        scrap[k].x = robots[j].x;
                        scrap[k].y = robots[j].y;
                        break;
                    }
                }
                robots[j].x = -1;
                robots[j].y = -1;
            }
        }
    }
}

int checkCollisionScrap(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]) {
    for (int i = 0; i < ROBOTS - 1; ++i) {
        for (int j = 0; j < SCRAP - 1; ++j) {
            if (robots[i].x == scrap[j].x && robots[i].y == scrap[j].y) {
                robots[i].x = -1;
                robots[i].y = -1;
            }
        }
    }
}