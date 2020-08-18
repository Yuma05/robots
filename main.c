#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define W 60
#define H 20
#define ROBOTS 40
#define SCRAP 20
#define TRUE 1
#define FALSE 0


typedef struct {
    int x, y;
} COORDINATE;

extern char getChar(void);

//ロボットの初期化
void initRobots(COORDINATE robots[ROBOTS]);

//スクラップの初期化
void initScrap(COORDINATE scrap[SCRAP]);

//プレイヤーの初期位置を設定
void createPlayer(COORDINATE *player, int isUsed[H][W]);

//ロボットの初期位置の設定
void createRobots(COORDINATE robots[ROBOTS], int robotCount, int isUsed[H][W]);

//ランダム値の生成
int randint(int min, int max);

//表示
void displayBoard(COORDINATE robots[ROBOTS], COORDINATE player, COORDINATE scrap[SCRAP]);

//ゲーム進行
int play(COORDINATE robots[ROBOTS], COORDINATE player, COORDINATE scrap[SCRAP], int robotCount, int level);

//プレイヤーの移動
void movePlayer(char command, COORDINATE *player, COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]);

//ロボットの移動
void moveRobots(COORDINATE robots[ROBOTS], COORDINATE player);

//スクラップとロボットの衝突判定
void checkCollisionScrap(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]);

//ロボット同士の衝突判定
void checkCollisionRobots(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]);

//残りのロボットの確認
int countRemainingRobots(COORDINATE robots[ROBOTS]);

//スコアの表示
void displayScore(int level, int score);

//スコアの初期化
int initScore(int level);

//ロボットとプレイヤーの衝突判定
int isCollisionRobotsAndPlayer(COORDINATE robots[ROBOTS], COORDINATE player);

//プレイヤーのワープ先にロボットがいないか
int canMovePlayer(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP], COORDINATE player);

int main() {
    srand((unsigned int) time(NULL));
    COORDINATE robots[ROBOTS];
    COORDINATE scrap[ROBOTS];
    COORDINATE player;
    int level = 1;
    int isGameClear = TRUE;
    int robotCount = 5;
    while (isGameClear) {
        int isUsed[H][W] = {FALSE};
        createPlayer(&player, isUsed);
        initScrap(scrap);
        initRobots(robots);
        createRobots(robots, robotCount, isUsed);
        isGameClear = play(robots, player, scrap, robotCount, level);
        level++;
        if (level <= 8) {
            robotCount = level * 5;
        } else {
            robotCount = 40;
        }
    }

    return 0;
}

void initRobots(COORDINATE robots[ROBOTS]) {
    for (int i = 0; i < ROBOTS; ++i) {
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

void displayBoard(COORDINATE robots[ROBOTS], COORDINATE player, COORDINATE scrap[SCRAP]) {
    char board[H][W];
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            board[i][j] = ' ';
        }
    }

    //ロボットの生成
    for (int i = 0; i < ROBOTS; ++i) {
        if (robots[i].x != -1 && robots[i].y != -1) {

            board[robots[i].y][robots[i].x] = '+';
        }
    }

    //スクラップの生成
    for (int i = 0; i < SCRAP; ++i) {
        if (scrap[i].x != -1 && scrap[i].y != -1) {
            board[scrap[i].y][scrap[i].x] = '*';
        }
    }

    //プレイヤーの生成
    board[player.y][player.x] = '@';

    //外枠
    printf("+");
    for (int i = 0; i < W; ++i) {
        printf("-");
    }
    printf("+\n");

    //表示
    for (int i = 0; i < H; ++i) {
        printf("|");
        for (int j = 0; j < W; ++j) {
            printf("%c", board[i][j]);
        }
        printf("|\n");
    }
    printf("+");

    //外枠
    for (int i = 0; i < W; ++i) {
        printf("-");
    }
    printf("+\n");
}

int play(COORDINATE robots[ROBOTS], COORDINATE player, COORDINATE scrap[SCRAP], int robotCount, int level) {
    char command;
    int remainingRobot = robotCount;
    int tmpRemainingRobots;
    int score = initScore(level);
    int isGameOver = FALSE;
    displayBoard(robots, player, scrap);
    displayScore(level, score);
    while (remainingRobot != 0) {
        command = getChar();
        movePlayer(command, &player, robots, scrap);
        moveRobots(robots, player);
        isGameOver = isCollisionRobotsAndPlayer(robots, player);
        if (isGameOver) {
            displayBoard(robots, player, scrap);
            displayScore(level, score);
            printf("Game Over!\n");
            return FALSE;
        }
        checkCollisionRobots(robots, scrap);
        checkCollisionScrap(robots, scrap);
        tmpRemainingRobots = remainingRobot;
        remainingRobot = countRemainingRobots(robots);
        if (tmpRemainingRobots != remainingRobot) {
            score += tmpRemainingRobots - remainingRobot;
        }
        displayBoard(robots, player, scrap);
        displayScore(level, score);
    }
    return TRUE;
}

void movePlayer(char command, COORDINATE *player, COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]) {
    COORDINATE temp;
    int isMove = FALSE;
    temp.x = player->x;
    temp.y = player->y;
    switch ((int) command - '0') {
        case 0:
            while (!isMove) {
                temp.x = randint(0, W - 1);
                temp.y = randint(0, H - 1);
                isMove = canMovePlayer(robots, scrap, temp);
            }
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

void moveRobots(COORDINATE robots[ROBOTS], COORDINATE player) {
    for (int i = 0; i < ROBOTS; ++i) {
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


void checkCollisionRobots(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]) {
    for (int i = 0; i < ROBOTS - 1; ++i) {
        for (int j = i + 1; j < ROBOTS; ++j) {
            if (robots[i].x == robots[j].x && robots[i].y == robots[j].y) {
                for (int k = 0; k < SCRAP - 1; ++k) {
                    if (scrap[k].x == -1 && scrap[k].y == -1) {
                        //衝突した位置にスクラップ生成
                        scrap[k].x = robots[j].x;
                        scrap[k].y = robots[j].y;
                        break;
                    }
                }
                //衝突したロボットは削除
                robots[j].x = -1;
                robots[j].y = -1;
            }
        }
    }
}

void checkCollisionScrap(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP]) {
    for (int i = 0; i < ROBOTS; ++i) {
        for (int j = 0; j < SCRAP; ++j) {
            if (robots[i].x == scrap[j].x && robots[i].y == scrap[j].y) {
                robots[i].x = -1;
                robots[i].y = -1;
            }
        }
    }
}

int countRemainingRobots(COORDINATE robots[ROBOTS]) {
    int countRobots = 0;
    for (int i = 0; i < ROBOTS; ++i) {
        if (robots[i].x != -1 && robots[i].y != -1) {
            countRobots++;
        }
    }
    return countRobots;
}

void displayScore(int level, int score) {
    printf("(level:%d score:%d)?\n", level, score);
}

int initScore(int level) {
    int score = (level - 1) * 10;
    if (level <= 8) {
        score += (level - 1) * 5;
    } else {
        score += 40;
    }

    return score;
}

int isCollisionRobotsAndPlayer(COORDINATE robots[ROBOTS], COORDINATE player) {
    for (int i = 0; i < ROBOTS; ++i) {
        if (player.x == robots[i].x && player.y == robots[i].y) {
            return TRUE;
        }
    }
    return FALSE;
}

int canMovePlayer(COORDINATE robots[ROBOTS], COORDINATE scrap[SCRAP], COORDINATE player) {
    int isUsed[H][W] = {FALSE};
    //ロボットが使用している座標
    for (int i = 0; i < ROBOTS; ++i) {
        if (robots[i].x != -1 && robots[i].y != -1) {
            isUsed[robots[i].y][robots[i].x] = TRUE;
        }
    }
    //スクラップが使用している座標
    for (int i = 0; i < SCRAP; ++i) {
        if (scrap[i].x != -1 && scrap[i].y != -1) {
            isUsed[scrap[i].y][scrap[i].x] = TRUE;
        }
    }

    if (isUsed[player.y][player.x]) {
        return FALSE;
    } else {
        return TRUE;
    }
}