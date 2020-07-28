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

void displayBoard(COORDINATE robots[ROBOTS], COORDINATE player, int robotCount);

void play(COORDINATE robots[ROBOTS], COORDINATE player, int robotCount);

void movePlayer(char command,COORDINATE *player);

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
    createRobots(robots, robotCount, isUsed);
//    for (int i = 0; i < n; ++i) {
//        printf("%d,%d\n", robots[i].x, robots[i].y);
//    }
    displayBoard(robots, player, robotCount);
    play(robots,player,robotCount);

    return 0;
}

void initRobots(COORDINATE robots[ROBOTS]) {
    for (int i = 0; i < ROBOTS; ++i) {
        robots[i].x = -1;
        robots[i].y = -1;
    }
}

void initScrap(COORDINATE scrap[SCRAP]) {
    for (int i = 0; i < SCRAP; ++i) {
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

void displayBoard(COORDINATE robots[ROBOTS], COORDINATE player, int robotCount) {
    char board[H][W];
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            board[i][j] = ' ';
        }
    }

    for (int i = 0; i < robotCount; ++i) {
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

void play(COORDINATE robots[ROBOTS], COORDINATE player, int robotCount){
    displayBoard(robots, player, robotCount);
    char command = getChar();
    movePlayer(command,&player);
}

void movePlayer(char command,COORDINATE *player){
    switch ((int)command) {
        case 0:
            player->x = randint(0, W - 1);
            player->y = randint(0, H - 1);
        case 1:
            player->x -= 1;
            player->y += 1;
        case 2:
            player->y += 1;
        case 3:
            player->x += 1;
            player->y += 1;
        case 4:
            player->x -= 1;
        case 5:
        case 6:
            player->x += 1;
        case 7:
            player->x -= 1;
            player->y -= 1;
        case 8:
            player->y -= 1;
        case 9:
            player->x += 1;
            player->y -= 1;
        default:
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