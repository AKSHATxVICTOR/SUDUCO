#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9

// Structure for each cell in the Sudoku board
typedef struct Cell {
    int value;
    int fixed;  // 1 if the cell is part of the initial board, 0 otherwise
} Cell;

// Structure for each row in the Sudoku board
typedef struct Row {
    Cell cells[SIZE];
    struct Row* next;
} Row;

// Structure for the entire Sudoku board
typedef struct {
    Row* head;
} SudokuBoard;

// Function to create an empty Sudoku board
SudokuBoard* createBoard() {
    SudokuBoard* board = (SudokuBoard*)malloc(sizeof(SudokuBoard));
    board->head = NULL;
    Row* current = NULL;

    for (int i = 0; i < SIZE; i++) {
        Row* newRow = (Row*)malloc(sizeof(Row));
        for (int j = 0; j < SIZE; j++) {
            newRow->cells[j].value = 0;
            newRow->cells[j].fixed = 0;
        }
        newRow->next = NULL;

        if (board->head == NULL) {
            board->head = newRow;
        } else {
            current->next = newRow;
        }
        current = newRow;
    }

    return board;
}

// Function to print the Sudoku board
void printBoard(SudokuBoard* board) {
    Row* current = board->head;
    for (int i = 0; i < SIZE; i++) {
        if (i % 3 == 0 && i != 0) {
            printf("---------------------\n");
        }
        for (int j = 0; j < SIZE; j++) {
            if (j % 3 == 0 && j != 0) {
                printf("| ");
            }
            if (current->cells[j].value == 0) {
                printf(". ");
            } else {
                printf("%d ", current->cells[j].value);
            }
        }
        printf("\n");
        current = current->next;
    }
}

// Function to check if a move is valid
int isValidMove(SudokuBoard* board, int row, int col, int num) {
    // Check row
    Row* currentRow = board->head;
    for (int i = 0; i < row; i++) {
        currentRow = currentRow->next;
    }
    for (int j = 0; j < SIZE; j++) {
        if (currentRow->cells[j].value == num) {
            return 0;
        }
    }

    // Check column
    currentRow = board->head;
    for (int i = 0; i < SIZE; i++) {
        if (currentRow->cells[col].value == num) {
            return 0;
        }
        currentRow = currentRow->next;
    }

    // Check 3x3 box
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    currentRow = board->head;
    for (int i = 0; i < startRow; i++) {
        currentRow = currentRow->next;
    }
    for (int i = 0; i < 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (currentRow->cells[j].value == num) {
                return 0;
            }
        }
        currentRow = currentRow->next;
    }

    return 1;
}

// Function to initialize the board with some fixed numbers
void initializeBoard(SudokuBoard* board) {
    srand(time(NULL));
    Row* current = board->head;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (rand() % 5 == 0) {  // 20% chance of placing a fixed number
                int num = rand() % 9 + 1;
                if (isValidMove(board, i, j, num)) {
                    current->cells[j].value = num;
                    current->cells[j].fixed = 1;
                }
            }
        }
        current = current->next;
    }
}

// Function to make a move
void makeMove(SudokuBoard* board, int row, int col, int num) {
    Row* current = board->head;
    for (int i = 0; i < row; i++) {
        current = current->next;
    }
    if (!current->cells[col].fixed) {
        current->cells[col].value = num;
    } else {
        printf("This cell is fixed and cannot be changed.\n");
    }
}

// Main function to demonstrate the game
int main() {
    SudokuBoard* board = createBoard();
    initializeBoard(board);

    int row, col, num;
    while (1) {
        printBoard(board);
        printf("Enter row (0-8), column (0-8), and number (1-9) separated by spaces (or -1 to quit): ");
        scanf("%d", &row);
        if (row == -1) break;
        scanf("%d %d", &col, &num);

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || num < 1 || num > 9) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (isValidMove(board, row, col, num)) {
            makeMove(board, row, col, num);
        } else {
            printf("Invalid move. Try again.\n");
        }
    }

    // Free allocated memory
    Row* current = board->head;
    while (current != NULL) {
        Row* temp = current;
        current = current->next;
        free(temp);
    }
    free(board);

    return 0;
}