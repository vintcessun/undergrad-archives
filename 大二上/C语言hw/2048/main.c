#include <stdio.h>   // 标准输入输出库
#include <stdlib.h>  // 标准库，包含 rand、srand 等函数
#include <time.h>    // 时间库，用于随机数种子
#include <stdbool.h> // 布尔类型支持

#define FIELD_WIDTH 4   // 棋盘宽度
#define FIELD_HEIGHT 4  // 棋盘高度

static int map[FIELD_WIDTH][FIELD_HEIGHT]; // 游戏棋盘数组，存储方块数值

void spawn();

/**
 * 重置游戏棋盘
 * 将所有格子置为 0，并生成一个新的方块
 */
void reset() {
    // 遍历棋盘，将所有格子置为 0
    for (int x = 0; x < FIELD_WIDTH; x++)
        for (int y = 0; y < FIELD_HEIGHT; y++)
            map[x][y] = 0;
    // 生成一个新的方块
    spawn();
}

/**
 * 在棋盘的空位置生成一个新的方块（2 或 4）
 * 如果棋盘已满，则重置游戏
 */
void spawn() {
    int emptyCount = 0; // 空格子数量
    int emptyPos[FIELD_WIDTH * FIELD_HEIGHT][2]; // 存储空格子坐标
    for (int x = 0; x < FIELD_WIDTH; x++) {
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            if (map[x][y] == 0) {
                emptyPos[emptyCount][0] = x;
                emptyPos[emptyCount][1] = y;
                emptyCount++;
            }
        }
    }
    if (emptyCount == 0) {
        printf("Game Over! Resetting...\n");
        reset();
        return;
    }
    // 随机选择一个空格子
    int idx = rand() % emptyCount;
    // 90% 概率生成 2，10% 概率生成 4
    map[emptyPos[idx][0]][emptyPos[idx][1]] = (rand() % 10) < 9 ? 2 : 4;
}

/**
 * 打印当前棋盘状态到命令行
 * 空格子用 '.' 表示
 */
void printMap() {
    printf("\n");
    // 按行打印棋盘
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (map[x][y] == 0) printf(".\t");
            else printf("%d\t", map[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * 按指定方向移动棋盘上的方块
 * dx, dy 表示方向：(-1,0) 左，(1,0) 右，(0,-1) 上，(0,1) 下
 * 返回值表示是否成功移动
 */
bool move(int dx, int dy) {
    bool moved = false;
    int temp[FIELD_WIDTH][FIELD_HEIGHT]; // 临时棋盘，用于计算移动结果
    for (int x = 0; x < FIELD_WIDTH; x++)
        for (int y = 0; y < FIELD_HEIGHT; y++)
            temp[x][y] = map[x][y];

    if (dx == -1) { // left
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            for (int x = 1; x < FIELD_WIDTH; x++) {
                if (temp[x][y] == 0) continue;
                int nx = x; // 当前方块位置
                // 向左移动直到遇到非空格子或边界
                while (nx - 1 >= 0 && temp[nx - 1][y] == 0) nx--;
                // 如果相邻方块数值相同，则合并
                if (nx - 1 >= 0 && temp[nx - 1][y] == temp[x][y]) {
                    temp[nx - 1][y] *= 2; // 合并为更大数值
                    temp[x][y] = 0;       // 原位置清空
                    moved = true;
                } else if (nx != x) { // 否则仅移动
                    temp[nx][y] = temp[x][y];
                    temp[x][y] = 0;
                    moved = true;
                }
            }
        }
    } else if (dx == 1) { // right
        for (int y = 0; y < FIELD_HEIGHT; y++) {
            for (int x = FIELD_WIDTH - 2; x >= 0; x--) {
                if (temp[x][y] == 0) continue;
                int nx = x; // 当前方块位置
                // 向右移动直到遇到非空格子或边界
                while (nx + 1 < FIELD_WIDTH && temp[nx + 1][y] == 0) nx++;
                // 如果相邻方块数值相同，则合并
                if (nx + 1 < FIELD_WIDTH && temp[nx + 1][y] == temp[x][y]) {
                    temp[nx + 1][y] *= 2; // 合并为更大数值
                    temp[x][y] = 0;       // 原位置清空
                    moved = true;
                } else if (nx != x) { // 否则仅移动
                    temp[nx][y] = temp[x][y];
                    temp[x][y] = 0;
                    moved = true;
                }
            }
        }
    } else if (dy == -1) { // up
        for (int x = 0; x < FIELD_WIDTH; x++) {
            for (int y = 1; y < FIELD_HEIGHT; y++) {
                if (temp[x][y] == 0) continue;
                int ny = y; // 当前方块位置
                // 向上移动直到遇到非空格子或边界
                while (ny - 1 >= 0 && temp[x][ny - 1] == 0) ny--;
                // 如果相邻方块数值相同，则合并
                if (ny - 1 >= 0 && temp[x][ny - 1] == temp[x][y]) {
                    temp[x][ny - 1] *= 2; // 合并为更大数值
                    temp[x][y] = 0;       // 原位置清空
                    moved = true;
                } else if (ny != y) { // 否则仅移动
                    temp[x][ny] = temp[x][y];
                    temp[x][y] = 0;
                    moved = true;
                }
            }
        }
    } else if (dy == 1) { // down
        for (int x = 0; x < FIELD_WIDTH; x++) {
            for (int y = FIELD_HEIGHT - 2; y >= 0; y--) {
                if (temp[x][y] == 0) continue;
                int ny = y; // 当前方块位置
                // 向下移动直到遇到非空格子或边界
                while (ny + 1 < FIELD_HEIGHT && temp[x][ny + 1] == 0) ny++;
                // 如果相邻方块数值相同，则合并
                if (ny + 1 < FIELD_HEIGHT && temp[x][ny + 1] == temp[x][y]) {
                    temp[x][ny + 1] *= 2; // 合并为更大数值
                    temp[x][y] = 0;       // 原位置清空
                    moved = true;
                } else if (ny != y) { // 否则仅移动
                    temp[x][ny] = temp[x][y];
                    temp[x][y] = 0;
                    moved = true;
                }
            }
        }
    }

    if (moved) {
        // 将临时棋盘结果复制回主棋盘
        for (int x = 0; x < FIELD_WIDTH; x++)
            for (int y = 0; y < FIELD_HEIGHT; y++)
                map[x][y] = temp[x][y];
        spawn();
    }
    return moved;
}

/**
 * 主函数
 * 初始化随机数种子，重置棋盘并进入游戏循环
 */
int main() {
    srand((unsigned)time(NULL)); // 初始化随机数种子
    reset();                     // 初始化棋盘并生成首个方块
    printMap();                   // 打印初始棋盘
    char cmd;
    while (1) {
        printf("输入方向 (WASD): ");
        scanf(" %c", &cmd); // 读取用户输入的方向
        bool moved = false;
        // 根据输入执行移动
        if (cmd == 'a' || cmd == 'A') moved = move(-1, 0); // 左
        else if (cmd == 'd' || cmd == 'D') moved = move(1, 0);  // 右
        else if (cmd == 'w' || cmd == 'W') moved = move(0, -1); // 上
        else if (cmd == 's' || cmd == 'S') moved = move(0, 1);  // 下
        
        // 如果成功移动则打印新棋盘，否则提示无法移动
        if (moved) printMap();
        else printf("无法移动！\n");
    }
    return 0;
}
