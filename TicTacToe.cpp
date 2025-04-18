#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype> // for toupper

using namespace std;

// ANSI color codes for console output
#define RED "\033[31m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

// Function declarations
void drawBoard(const vector<vector<char>>& board, int N);
bool makeMove(vector<vector<char>>& board, const string& position, char player, int N);
bool checkWin(const vector<vector<char>>& board, char player, int N);
bool isBoardFull(const vector<vector<char>>& board, int N);
string getComputerMove(const vector<vector<char>>& board, int N, int difficulty);
void displayScores(int scores[], int N);

int main() {
    int N, difficulty = 1; // Default difficulty: Easy (1=Easy, 2=Medium)
    char opponent, playAgainChoice;
    int scores[2] = {0, 0}; // scores[0] for X, scores[1] for O
    bool gameWon;

    cout << "Welcome to Enhanced Tic-Tac-Toe!\n";

    do {
        // Menu system
        cout << "\n=== Game Setup ===\n";
        cout << "Enter board size (N x N, 3-10): ";
        cin >> N;
        if (N < 3 || N > 10) {
            cout << "Invalid size. Defaulting to 3x3.\n";
            N = 3;
        }

        cout << "Opponent (H for Human, C for Computer): ";
        cin >> opponent;
        opponent = toupper(opponent);
        if (opponent != 'H' && opponent != 'C') {
            cout << "Invalid choice. Defaulting to Human.\n";
            opponent = 'H';
        }

        if (opponent == 'C') {
            cout << "Difficulty (1=Easy, 2=Medium): ";
            cin >> difficulty;
            if (difficulty < 1 || difficulty > 2) difficulty = 1;
        }

        // Initialize game
        vector<vector<char>> board(N, vector<char>(N, ' '));
        char currentPlayer = 'X';
        gameWon = false;

        while (!gameWon && !isBoardFull(board, N)) {
            drawBoard(board, N);

            string move;
            if (currentPlayer == 'X' || (currentPlayer == 'O' && opponent == 'H')) {
                cout << "Player " << currentPlayer << "'s turn (e.g., A1): ";
                cin >> move;
            } else {
                move = getComputerMove(board, N, difficulty);
                cout << "Computer chooses: " << move << "\n";
            }

            if (makeMove(board, move, currentPlayer, N)) {
                if (checkWin(board, currentPlayer, N)) {
                    drawBoard(board, N);
                    cout << "Player " << currentPlayer << " wins!\n";
                    scores[currentPlayer == 'X' ? 0 : 1]++;
                    gameWon = true;
                } else {
                    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                }
            } else {
                cout << "Invalid move! Try again.\n";
            }
        }

        if (!gameWon) {
            drawBoard(board, N);
            cout << "It's a tie!\n";
        }

        displayScores(scores, N);
        cout << "Play again? (Y/N): ";
        cin >> playAgainChoice;
    } while (toupper(playAgainChoice) == 'Y');

    cout << "Thanks for playing!\n";
    return 0;
}

// Draw the board with row letters, column numbers, and colored symbols
void drawBoard(const vector<vector<char>>& board, int N) {
    cout << "\n  ";
    for (int i = 1; i <= N; i++) cout << i << "   ";
    cout << "\n";
    for (int i = 0; i < N; i++) {
        cout << (char)('A' + i) << " ";
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 'X') cout << RED << "X" << RESET;
            else if (board[i][j] == 'O') cout << BLUE << "O" << RESET;
            else cout << " ";
            if (j < N - 1) cout << " | ";
        }
        cout << "\n";
        if (i < N - 1) {
            cout << "  ";
            for (int j = 0; j < N - 1; j++) cout << "---";
            cout << "\n";
        }
    }
    cout << "\n";
}

// Process a player's move
bool makeMove(vector<vector<char>>& board, const string& position, char player, int N) {
    if (position.length() < 2) return false;
    char rowChar = toupper(position[0]);
    if (rowChar < 'A' || rowChar >= 'A' + N) return false;
    int row = rowChar - 'A';
    int col = position[1] - '1';
    if (col < 0 || col >= N) return false;
    if (board[row][col] == ' ') {
        board[row][col] = player;
        return true;
    }
    return false;
}

// Check for a win (N in a row, column, or diagonal)
bool checkWin(const vector<vector<char>>& board, char player, int N) {
    // Rows
    for (int i = 0; i < N; i++) {
        bool win = true;
        for (int j = 0; j < N; j++) {
            if (board[i][j] != player) { win = false; break; }
        }
        if (win) return true;
    }
    // Columns
    for (int j = 0; j < N; j++) {
        bool win = true;
        for (int i = 0; i < N; i++) {
            if (board[i][j] != player) { win = false; break; }
        }
        if (win) return true;
    }
    // Main diagonal
    bool win = true;
    for (int i = 0; i < N; i++) {
        if (board[i][i] != player) { win = false; break; }
    }
    if (win) return true;
    // Anti-diagonal
    win = true;
    for (int i = 0; i < N; i++) {
        if (board[i][N - 1 - i] != player) { win = false; break; }
    }
    return win;
}

// Check if the board is full
bool isBoardFull(const vector<vector<char>>& board, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == ' ') return false;
    return true;
}

// Computer move logic with difficulty levels
string getComputerMove(const vector<vector<char>>& board, int N, int difficulty) {
    vector<string> emptyPositions;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == ' ') {
                string pos = string(1, 'A' + i) + string(1, '1' + j);
                emptyPositions.push_back(pos);
            }
        }
    }
    if (emptyPositions.empty()) return "";

    srand(time(NULL));
    if (difficulty == 2) { // Medium: Try to block player's win
        char opponent = 'X'; // Assuming computer is O
        for (const string& pos : emptyPositions) {
            vector<vector<char>> tempBoard = board;
            int row = toupper(pos[0]) - 'A';
            int col = pos[1] - '1';
            tempBoard[row][col] = opponent;
            if (checkWin(tempBoard, opponent, N)) {
                tempBoard[row][col] = 'O';
                if (!checkWin(tempBoard, 'O', N)) return pos; // Block opponent's win
            }
        }
    }
    // Easy or fallback: Random move
    int index = rand() % emptyPositions.size();
    return emptyPositions[index];
}

// Display current scores
void displayScores(int scores[], int N) {
    cout << "\n=== Scores ===\n";
    cout << "Player X: " << scores[0] << "\n";
    cout << "Player O: " << scores[1] << "\n";
}
