#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

#define SIZE 19 
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define WIN_COUNT 5

struct Stone {
    int x;  // 돌의 x 좌표
    int y;  // 돌의 y 좌표
    int color;  // 돌의 색깔 (1: 흑돌, 2: 백돌)
};



// 오목판 초기화
void initializeBoard(struct Stone board[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j].x = i;
            board[i][j].y = j;
            board[i][j].color = 0;  // 돌이 없는 상태를 나타내는 값 (0)
        }
    }
}

// 바둑판 두기
void printBoard(const struct Stone board[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j].color == 0) {
                printf("+");
            }
            else if (board[i][j].color == 1) {
                printf("O");
            }
            else if (board[i][j].color == 2) {
                printf("O");
            }
            else if (board[i][j].color == 3) {  // AI가 둔 위치인 경우
                printf("X");
            }
            printf(" ");
        }
        printf("\n");
    }
}

// 돌 두기
void placeStone(struct Stone board[SIZE][SIZE], int x, int y, int color) {
    board[x][y].color = color;
}


// 게임 보드의 승리 여부 확인
bool checkWin(const struct Stone board[SIZE][SIZE], int color) {
    int i, j;

    // 가로로 승리 여부 확인
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j <= SIZE - WIN_COUNT; j++) {
            bool win = true;
            for (int k = 0; k < WIN_COUNT; k++) {
                if (board[i][j + k].color != color) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return true;
            }
        }
    }

    // 세로로 승리 여부 확인
    for (i = 0; i <= SIZE - WIN_COUNT; i++) {
        for (j = 0; j < SIZE; j++) {
            bool win = true;
            for (int k = 0; k < WIN_COUNT; k++) {
                if (board[i + k][j].color != color) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return true;
            }
        }
    }

    // 대각선 (왼쪽 위에서 오른쪽 아래)으로 승리 여부 확인
    for (i = 0; i <= SIZE - WIN_COUNT; i++) {
        for (j = 0; j <= SIZE - WIN_COUNT; j++) {
            bool win = true;
            for (int k = 0; k < WIN_COUNT; k++) {
                if (board[i + k][j + k].color != color) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return true;
            }
        }
    }

    // 대각선 (오른쪽 위에서 왼쪽 아래)으로 승리 여부 확인
    for (i = 0; i <= SIZE - WIN_COUNT; i++) {
        for (j = WIN_COUNT - 1; j < SIZE; j++) {
            bool win = true;
            for (int k = 0; k < WIN_COUNT; k++) {
                if (board[i + k][j - k].color != color) {
                    win = false;
                    break;
                }
            }
            if (win) {
                return true;
            }
        }
    }

    return false;
}

// 현재 상태 평가 함수
int evaluateBoard(const struct Stone board[SIZE][SIZE], int color) {
    if (checkWin(board, color)) {
        return 1;  // 승리한 경우
    }
    else if (checkWin(board, 3 - color)) {
        return -1; // 패배한 경우
    }
    return 0;  // 비긴 경우
}

// 미니맥스 알고리즘
int minimax(struct Stone board[SIZE][SIZE], int depth, int alpha, int beta, bool maximizingPlayer, int color) {
    if (depth == 0 || checkWin(board, 1) || checkWin(board, 2)) {
        return evaluateBoard(board, color);
    }

    if (maximizingPlayer) {
        int bestScore = -1000;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j].color == 0) {
                    board[i][j].color = color;
                    int score = minimax(board, depth - 1, alpha, beta, false, color);
                    board[i][j].color = 0;
                    bestScore = max(bestScore, score);
                    alpha = max(alpha, bestScore);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = 1000;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j].color == 0) {
                    board[i][j].color = 3 - color;
                    int score = minimax(board, depth - 1, alpha, beta, true, color);
                    board[i][j].color = 0;
                    bestScore = min(bestScore, score);
                    beta = min(beta, bestScore);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestScore;
    }
}

// AI 수 선택
void chooseMove(struct Stone board[SIZE][SIZE], int color) {
    int bestScore = -1000;
    int bestX = -1, bestY = -1;
    int alpha = -1000;
    int beta = 1000;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j].color == 0) {
                board[i][j].color = color;
                int score = minimax(board, 4, alpha, beta, false, color);
                board[i][j].color = 0;
                if (score > bestScore) {
                    bestScore = score;
                    bestX = i;
                    bestY = j;
                }
            }
        }
    }

    board[bestX][bestY].color = color;
    printf("AI가 돌을 둔 위치: (%d, %d)\n", bestX + 1, bestY + 1);
    board[bestX][bestY].color = 3;
}



int main() {
    struct Stone board[SIZE][SIZE];
    int currentPlayer = 1;
    int playerColor, aiColor;

    initializeBoard(board);

    printf("======= AI 오목 게임 =======\n");
    printf("게임 보드 크기: %d x %d\n", SIZE, SIZE);
    printf("승리 조건: %d 돌 연속\n", WIN_COUNT);
    printf("===========================\n\n");

    // 플레이어 돌 색상 선택
    printf("어떤 돌을 선택하시겠습니까?\n");
    printf("1. 흑돌 (B)\n");
    printf("2. 백돌 (W)\n");
    scanf("%d", &playerColor);

    if (playerColor == 1) {
        currentPlayer = 1;
        aiColor = 2;
    }
    else if (playerColor == 2) {
        currentPlayer = 2;
        aiColor = 1;
        chooseMove(board, aiColor); // AI가 먼저 수를 둠
    }


    while (1) {

        printBoard(board);

        if (currentPlayer == 1) {
            printf("플레이어 차례 (");
            if (playerColor == 1) {
                printf("흑돌, B)\n");
            }
            else if (playerColor == 2) {
                printf("백돌, W)\n");
            }
            int x, y;
            printf("돌을 둘 위치 (행 열): ");
            scanf("%d %d", &x, &y);
            if (board[x - 1][y - 1].color != 0) {
                printf("이미 돌이 있습니다. 다른 위치를 선택하세요.\n");
                continue;
            }
            board[x - 1][y - 1].color = playerColor;
        }
        else {
            printf("AI 차례 (");
            if (aiColor == 1) {
                printf("흑돌, B)\n");
            }
            else if (aiColor == 2) {
                printf("백돌, W)\n");
            }
            chooseMove(board, aiColor);
        }

        if (checkWin(board, currentPlayer)) {
            printBoard(board);
            printf("플레이어 %d가 승리했습니다!\n", currentPlayer);
            break;
        }

        currentPlayer = 3 - currentPlayer;  // 플레이어 전환
    }

    return 0;
}



       