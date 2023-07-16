#include <iostream>
#include <vector>
#include <algorithm>

#define SIZE 9
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
void initializeBoard(std::vector<std::vector<Stone>>& board) {
    board.resize(SIZE, std::vector<Stone>(SIZE));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j].x = i;
            board[i][j].y = j;
            board[i][j].color = 0;  // 돌이 없는 상태를 나타내는 값 (0)
        }
    }
}

// 바둑판 출력
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

// 돌 두기
void placeStone(std::vector<std::vector<Stone>>& board, int x, int y, int color) {
    board[x][y].color = color;
}

// 게임 보드의 승리 여부 확인
bool checkWin(const std::vector<std::vector<Stone>>& board, int color) {
    // 가로로 승리 여부 확인
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

    // 세로로 승리 여부 확인
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

    // 대각선 (왼쪽 위에서 오른쪽 아래)으로 승리 여부 확인
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

    // 대각선 (오른쪽 위에서 왼쪽 아래)으로 승리 여부 확인
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

// 현재 상태 평가 함수
int evaluateBoard(const std::vector<std::vector<Stone>>& board, int color) {
    if (checkWin(board, color)) {
        return 1;  // 승리한 경우
    }
    else if (checkWin(board, 3 - color)) {
        return -1; // 패배한 경우
    }
    return 0;  // 비긴 경우
}

// 미니맥스 알고리즘
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

// AI 수 선택
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

   
    std::cout << "AI가 돌을 둔 위치: (" << bestX + 1 << ", " << bestY + 1 << ")\n";
    board[bestX][bestY].color = color;
}

int main() {
    std::vector<std::vector<Stone>> board(SIZE, std::vector<Stone>(SIZE));
    int currentPlayer = 1;
    int playerColor, aiColor;

    initializeBoard(board);

    std::cout << "======= AI 오목 게임 =======\n";
    std::cout << "게임 보드 크기: " << SIZE << " x " << SIZE << "\n";
    std::cout << "승리 조건: " << WIN_COUNT << " 돌 연속\n";
    std::cout << "===========================\n\n";

    // 플레이어 돌 색상 선택
    std::cout << "어떤 돌을 선택하시겠습니까?\n";
    std::cout << "1. 흑돌 (B)\n";
    std::cout << "2. 백돌 (W)\n";
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
            std::cout << "플레이어 차례 (";
            if (playerColor == 1) {
                std::cout << "흑돌, B)\n";
            }
            else if (playerColor == 2) {
                std::cout << "백돌, W)\n";
            }
            int x, y;
            std::cout << "돌을 둘 위치 (행 열): ";
            std::cin >> x >> y;
            if (board[x - 1][y - 1].color != 0) {
                std::cout << "이미 돌이 있습니다. 다른 위치를 선택하세요.\n";
                continue;
            }
            board[x - 1][y - 1].color = playerColor;
        }
        else {
            std::cout << "AI 차례 (";
            if (aiColor == 1) {
                std::cout << "흑돌, B)\n";
            }
            else if (aiColor == 2) {
                std::cout << "백돌, W)\n";
            }
            chooseMove(board, aiColor);
        }

        if (checkWin(board, currentPlayer)) {
            printBoard(board);
            std::cout << "플레이어가 승리했습니다!\n";
            break;
        }
        else if (checkWin(board, aiColor)) {
            printBoard(board);
            std::cout << "Ai가 승리\n";
            break;
        }

        currentPlayer = 3 - currentPlayer;  // 플레이어 전환
    }

    return 0;
}