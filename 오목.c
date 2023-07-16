#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

#define SIZE 19 
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define WIN_COUNT 5

struct Stone {
    int x;  // ���� x ��ǥ
    int y;  // ���� y ��ǥ
    int color;  // ���� ���� (1: �浹, 2: �鵹)
};



// ������ �ʱ�ȭ
void initializeBoard(struct Stone board[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j].x = i;
            board[i][j].y = j;
            board[i][j].color = 0;  // ���� ���� ���¸� ��Ÿ���� �� (0)
        }
    }
}

// �ٵ��� �α�
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
            else if (board[i][j].color == 3) {  // AI�� �� ��ġ�� ���
                printf("X");
            }
            printf(" ");
        }
        printf("\n");
    }
}

// �� �α�
void placeStone(struct Stone board[SIZE][SIZE], int x, int y, int color) {
    board[x][y].color = color;
}


// ���� ������ �¸� ���� Ȯ��
bool checkWin(const struct Stone board[SIZE][SIZE], int color) {
    int i, j;

    // ���η� �¸� ���� Ȯ��
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

    // ���η� �¸� ���� Ȯ��
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

    // �밢�� (���� ������ ������ �Ʒ�)���� �¸� ���� Ȯ��
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

    // �밢�� (������ ������ ���� �Ʒ�)���� �¸� ���� Ȯ��
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

// ���� ���� �� �Լ�
int evaluateBoard(const struct Stone board[SIZE][SIZE], int color) {
    if (checkWin(board, color)) {
        return 1;  // �¸��� ���
    }
    else if (checkWin(board, 3 - color)) {
        return -1; // �й��� ���
    }
    return 0;  // ��� ���
}

// �̴ϸƽ� �˰���
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

// AI �� ����
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
    printf("AI�� ���� �� ��ġ: (%d, %d)\n", bestX + 1, bestY + 1);
    board[bestX][bestY].color = 3;
}



int main() {
    struct Stone board[SIZE][SIZE];
    int currentPlayer = 1;
    int playerColor, aiColor;

    initializeBoard(board);

    printf("======= AI ���� ���� =======\n");
    printf("���� ���� ũ��: %d x %d\n", SIZE, SIZE);
    printf("�¸� ����: %d �� ����\n", WIN_COUNT);
    printf("===========================\n\n");

    // �÷��̾� �� ���� ����
    printf("� ���� �����Ͻðڽ��ϱ�?\n");
    printf("1. �浹 (B)\n");
    printf("2. �鵹 (W)\n");
    scanf("%d", &playerColor);

    if (playerColor == 1) {
        currentPlayer = 1;
        aiColor = 2;
    }
    else if (playerColor == 2) {
        currentPlayer = 2;
        aiColor = 1;
        chooseMove(board, aiColor); // AI�� ���� ���� ��
    }


    while (1) {

        printBoard(board);

        if (currentPlayer == 1) {
            printf("�÷��̾� ���� (");
            if (playerColor == 1) {
                printf("�浹, B)\n");
            }
            else if (playerColor == 2) {
                printf("�鵹, W)\n");
            }
            int x, y;
            printf("���� �� ��ġ (�� ��): ");
            scanf("%d %d", &x, &y);
            if (board[x - 1][y - 1].color != 0) {
                printf("�̹� ���� �ֽ��ϴ�. �ٸ� ��ġ�� �����ϼ���.\n");
                continue;
            }
            board[x - 1][y - 1].color = playerColor;
        }
        else {
            printf("AI ���� (");
            if (aiColor == 1) {
                printf("�浹, B)\n");
            }
            else if (aiColor == 2) {
                printf("�鵹, W)\n");
            }
            chooseMove(board, aiColor);
        }

        if (checkWin(board, currentPlayer)) {
            printBoard(board);
            printf("�÷��̾� %d�� �¸��߽��ϴ�!\n", currentPlayer);
            break;
        }

        currentPlayer = 3 - currentPlayer;  // �÷��̾� ��ȯ
    }

    return 0;
}



       