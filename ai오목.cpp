#include <iostream>
#include <vector>
#include <algorithm>

#define SIZE 9
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
void initializeBoard(std::vector<std::vector<Stone>>& board) {
    board.resize(SIZE, std::vector<Stone>(SIZE));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j].x = i;
            board[i][j].y = j;
            board[i][j].color = 0;  // ���� ���� ���¸� ��Ÿ���� �� (0)
        }
    }
}

// �ٵ��� ���
void printBoard(const std::vector<std::vector<Stone>>& board) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j].color == 0) {
                std::cout << "+";
            }
            else if (board[i][j].color == 1) {
                std::cout << "O";
            }
            else if (board[i][j].color == 2) {
                std::cout << "X";
            }
          
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

// �� �α�
void placeStone(std::vector<std::vector<Stone>>& board, int x, int y, int color) {
    board[x][y].color = color;
}

// ���� ������ �¸� ���� Ȯ��
bool checkWin(const std::vector<std::vector<Stone>>& board, int color) {
    // ���η� �¸� ���� Ȯ��
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j <= SIZE - WIN_COUNT; j++) {
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
    for (int i = 0; i <= SIZE - WIN_COUNT; i++) {
        for (int j = 0; j < SIZE; j++) {
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
    for (int i = 0; i <= SIZE - WIN_COUNT; i++) {
        for (int j = 0; j <= SIZE - WIN_COUNT; j++) {
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
    for (int i = 0; i <= SIZE - WIN_COUNT; i++) {
        for (int j = WIN_COUNT - 1; j < SIZE; j++) {
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
int evaluateBoard(const std::vector<std::vector<Stone>>& board, int color) {
    if (checkWin(board, color)) {
        return 1;  // �¸��� ���
    }
    else if (checkWin(board, 3 - color)) {
        return -1; // �й��� ���
    }
    return 0;  // ��� ���
}

// �̴ϸƽ� �˰���
int minimax(std::vector<std::vector<Stone>>& board, int depth, int alpha, int beta, bool maximizingPlayer, int color) {
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
                    bestScore = std::max(bestScore, score);
                    alpha = std::max(alpha, bestScore);
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
                    bestScore = std::min(bestScore, score);
                    beta = std::min(beta, bestScore);
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
void chooseMove(std::vector<std::vector<Stone>>& board, int color) {
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

   
    std::cout << "AI�� ���� �� ��ġ: (" << bestX + 1 << ", " << bestY + 1 << ")\n";
    board[bestX][bestY].color = color;
}

int main() {
    std::vector<std::vector<Stone>> board(SIZE, std::vector<Stone>(SIZE));
    int currentPlayer = 1;
    int playerColor, aiColor;

    initializeBoard(board);

    std::cout << "======= AI ���� ���� =======\n";
    std::cout << "���� ���� ũ��: " << SIZE << " x " << SIZE << "\n";
    std::cout << "�¸� ����: " << WIN_COUNT << " �� ����\n";
    std::cout << "===========================\n\n";

    // �÷��̾� �� ���� ����
    std::cout << "� ���� �����Ͻðڽ��ϱ�?\n";
    std::cout << "1. �浹 (B)\n";
    std::cout << "2. �鵹 (W)\n";
    std::cin >> playerColor;

    if (playerColor == 1) {
        currentPlayer = 1;
        aiColor = 2;
    }
    else if (playerColor == 2) {
        currentPlayer = 2;
        aiColor = 1;
    }

    while (true) {
        printBoard(board);

        if (currentPlayer == 1) {
            std::cout << "�÷��̾� ���� (";
            if (playerColor == 1) {
                std::cout << "�浹, B)\n";
            }
            else if (playerColor == 2) {
                std::cout << "�鵹, W)\n";
            }
            int x, y;
            std::cout << "���� �� ��ġ (�� ��): ";
            std::cin >> x >> y;
            if (board[x - 1][y - 1].color != 0) {
                std::cout << "�̹� ���� �ֽ��ϴ�. �ٸ� ��ġ�� �����ϼ���.\n";
                continue;
            }
            board[x - 1][y - 1].color = playerColor;
        }
        else {
            std::cout << "AI ���� (";
            if (aiColor == 1) {
                std::cout << "�浹, B)\n";
            }
            else if (aiColor == 2) {
                std::cout << "�鵹, W)\n";
            }
            chooseMove(board, aiColor);
        }

        if (checkWin(board, currentPlayer)) {
            printBoard(board);
            std::cout << "�÷��̾ �¸��߽��ϴ�!\n";
            break;
        }
        else if (checkWin(board, aiColor)) {
            printBoard(board);
            std::cout << "Ai�� �¸�\n";
            break;
        }

        currentPlayer = 3 - currentPlayer;  // �÷��̾� ��ȯ
    }

    return 0;
}