#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

// Global board representation
// Uppercase = White, Lowercase = Black
char board[8][8];

// Function Prototypes
void initBoard();
void printBoard();
int isValidMove(int r1, int c1, int r2, int c2, int turn);
int isPathClear(int r1, int c1, int r2, int c2);
void play();

int main() {
    initBoard();
    printf("Welcome to C Console Chess!\n");
    printf("Instructions: Enter moves as four numbers: Row1 Col1 Row2 Col2\n");
    printf("Example: '6 4 4 4' moves the piece at row 6, col 4 to row 4, col 4.\n\n");
    play();
    return 0;
}

void initBoard() {
    // Initialize Black Pieces
    char blackRow[] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
    char whiteRow[] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

    for (int i = 0; i < 8; i++) {
        board[0][i] = blackRow[i];
        board[1][i] = 'p';
        board[6][i] = 'P';
        board[7][i] = whiteRow[i];
    }

    // Initialize empty spaces
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = ' ';
        }
    }
}

void printBoard() {
    printf("\n   0   1   2   3   4   5   6   7\n");
    printf(" +---+---+---+---+---+---+---+---+\n");
    for (int i = 0; i < 8; i++) {
        printf("%d|", i);
        for (int j = 0; j < 8; j++) {
            printf(" %c |", board[i][j]);
        }
        printf("\n +---+---+---+---+---+---+---+---+\n");
    }
}

// Check if path is clear (for Rook, Bishop, Queen)
int isPathClear(int r1, int c1, int r2, int c2) {
    int dr = (r2 - r1) == 0 ? 0 : (r2 - r1) / abs(r2 - r1);
    int dc = (c2 - c1) == 0 ? 0 : (c2 - c1) / abs(c2 - c1);

    int r = r1 + dr;
    int c = c1 + dc;

    while (r != r2 || c != c2) {
        if (board[r][c] != ' ') return 0; // Blocked
        r += dr;
        c += dc;
    }
    return 1;
}

int isValidMove(int r1, int c1, int r2, int c2, int turn) {
    // 1. Check bounds
    if (r1 < 0 || r1 > 7 || c1 < 0 || c1 > 7 || r2 < 0 || r2 > 7 || c2 < 0 || c2 > 7) return 0;

    char piece = board[r1][c1];
    char target = board[r2][c2];

    // 2. Check if source is empty
    if (piece == ' ') return 0;

    // 3. Check turn (White is Uppercase, Black is lowercase)
    if (turn == 0 && islower(piece)) return 0; // White's turn but moved black
    if (turn == 1 && isupper(piece)) return 0; // Black's turn but moved white

    // 4. Check friendly fire
    if (turn == 0 && isupper(target)) return 0;
    if (turn == 1 && islower(target)) return 0;

    int dr = r2 - r1;
    int dc = c2 - c1;
    char type = toupper(piece);

    // 5. Piece Logic
    switch (type) {
        case 'P': // Pawn
            if (turn == 0) { // White moves UP (decreasing index)
                if (c1 == c2 && board[r2][c2] == ' ' && r2 == r1 - 1) return 1; // 1 step
                if (c1 == c2 && board[r2][c2] == ' ' && r1 == 6 && r2 == 4 && board[5][c1] == ' ') return 1; // 2 steps
                if (abs(c2 - c1) == 1 && r2 == r1 - 1 && islower(target)) return 1; // Capture
            } else { // Black moves DOWN (increasing index)
                if (c1 == c2 && board[r2][c2] == ' ' && r2 == r1 + 1) return 1;
                if (c1 == c2 && board[r2][c2] == ' ' && r1 == 1 && r2 == 3 && board[2][c1] == ' ') return 1;
                if (abs(c2 - c1) == 1 && r2 == r1 + 1 && isupper(target)) return 1;
            }
            break;
        case 'R': // Rook
            if (r1 == r2 || c1 == c2) return isPathClear(r1, c1, r2, c2);
            break;
        case 'B': // Bishop
            if (abs(dr) == abs(dc)) return isPathClear(r1, c1, r2, c2);
            break;
        case 'Q': // Queen
            if (r1 == r2 || c1 == c2 || abs(dr) == abs(dc)) return isPathClear(r1, c1, r2, c2);
            break;
        case 'K': // King
            if (abs(dr) <= 1 && abs(dc) <= 1) return 1;
            break;
        case 'N': // Knight
            if ((abs(dr) == 2 && abs(dc) == 1) || (abs(dr) == 1 && abs(dc) == 2)) return 1;
            break;
    }

    return 0;
}

void play() {
    int turn = 0; // 0 for White, 1 for Black
    int r1, c1, r2, c2;

    while (1) {
        printBoard();
        printf("\n%s's Turn.\n", (turn == 0) ? "White" : "Black");
        printf("Enter move (Row Col Row Col): ");
        
        if (scanf("%d %d %d %d", &r1, &c1, &r2, &c2) != 4) {
            printf("Invalid input. Try again.\n");
            while(getchar() != '\n'); // Clear buffer
            continue;
        }

        if (isValidMove(r1, c1, r2, c2, turn)) {
            // Check for Game Over (King Capture)
            if (tolower(board[r2][c2]) == 'k') {
                printBoard();
                printf("\nGAME OVER! %s Wins!\n", (turn == 0) ? "White" : "Black");
                break;
            }
            
            // Execute Move
            board[r2][c2] = board[r1][c1];
            board[r1][c1] = ' ';
            
            // Switch Turn
            turn = 1 - turn;
        } else {
            printf("Illegal move! Try again.\n");
        }
    }
}