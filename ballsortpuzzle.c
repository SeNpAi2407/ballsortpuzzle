#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "ballsortpuzzle.h"


void generator(const int rows, const int columns, char field[rows][columns]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            field[i][j] = ' ';
        }
    }

    char symbols[10] = {'*', '&', '+', '^', '$', '#', '%', '-', '=', '@'};
    int symbolCount = (rows * (columns - 2)) / rows;
    if (symbolCount > 10) symbolCount = 10;
    char allSymbols[rows * (columns - 2)];
    int index = 0;
    for (int i = 0; i < symbolCount; i++) {
        for (int j = 0; j < rows; j++) {
            allSymbols[index++] = symbols[i];
        }
    }
    for (int i = index - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = allSymbols[i];
        allSymbols[i] = allSymbols[j];
        allSymbols[j] = temp;
    }

    int col1 = rand() % columns;
    int col2;
    do {
        col2 = rand() % columns;
    } while (col1 == col2);

    index = 0;
    for (int j = 0; j < columns; j++) {
        if (j == col1 || j == col2) continue;
        for (int i = rows - 1; i >= 0; i--) {
            field[i][j] = allSymbols[index++];
        }
    }
}



void down_possible(const int rows, const int columns, char field[rows][columns], int x, int y) {
    if (x == y || x > columns || y > columns || x < 1 || y < 1) {
        printf("Error: Invalid column numbers.\n");
    } else {
        char tmp = '\0';
        int getIndex = -1;
        for (int i = 0; i < rows; i++) {
            if (field[i][x - 1] != ' ') {
                tmp = field[i][x - 1];
                field[i][x - 1] = ' ';
                getIndex = i;
                break;
            }
        }
        if (tmp == '\0') {
            printf("Error: Column to move from is empty.\n");
        } else {
            if (field[rows - 1][y - 1] == ' ') {
                field[rows - 1][y - 1] = tmp;
            } else if (field[0][y - 1] != ' ') {
                printf("Error: Column to move to is full.\n");
                field[getIndex][x - 1] = tmp;
            } else {
                for (int i = 1; i < rows; i++) {
                    if (field[i][y - 1] != ' ' && field[i][y - 1] == tmp) {
                        field[i - 1][y - 1] = tmp;
                        break;
                    } else if (field[i][y - 1] != ' ' && field[i][y - 1] != tmp) {
                        field[getIndex][x - 1] = tmp;
                        break;
                    } else if (field[i][y - 1] == ' ' && i == rows - 1) {
                        field[i][y - 1] = tmp;
                        break;
                    }
                }
            }
        }
    }
}

bool check(const int rows, const int columns, char field[rows][columns]) {
    for (int j = 0; j < columns; j++) {
        char firstSymbol = ' ';
        bool fullColumn = true;
        for (int i = 0; i < rows; i++) {
            if (field[i][j] != ' ') {
                if (firstSymbol == ' ') {
                    firstSymbol = field[i][j];
                } else if (field[i][j] != firstSymbol) {
                    return false;
                }
            } else {
                fullColumn = false;
            }
        }
        if (firstSymbol != ' ' && !fullColumn) {
            return false;
        }
    }
    return true;
}

bool move_possible(const int rows, const int columns, char field[rows][columns]) {
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < columns; j++) {
            if (i != j) {
                char topSymbolFrom = '\0';
                char topSymbolTo = '\0';

                for (int k = 0; k < rows; k++) {
                    if (field[k][i] != ' ') {
                        topSymbolFrom = field[k][i];
                        break;
                    }
                }

                for (int k = 0; k < rows; k++) {
                    if (field[k][j] != ' ') {
                        topSymbolTo = field[k][j];
                        break;
                    }
                }

                if (topSymbolFrom != '\0') {
                    if (topSymbolTo == ' ') {
                        return true;
                    } else if (topSymbolTo == topSymbolFrom) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void print_field(const int rows, const int columns, char field[rows][columns]) {
printf("\n");
    for (int i = 0; i < rows; i++) {
        printf("%d|", i + 1);
        for (int j = 0; j < columns; j++) {
            printf(" %c ", field[i][j]);
            if (j < columns) {
                printf("|");
            }
        }
        printf("\n");
    }
    printf("  ");
    for (int j = 0; j < columns; j++) {
        printf("--- ");
    }
printf("\n");
for (int j = 0; j < columns; j++) {
printf("   %d", j + 1);
}
    printf("\n");
}

void ball_sort_puzzle() {
    srand(time(NULL));
    int rows, cols;
    printf("Welcome to Ball Sort Puzzle!\n");
    printf("Enter number of rows: ");
    scanf("%d", &rows);

    printf("Enter number of columns: ");
    scanf("%d", &cols);

    char field[rows][cols];

    do {
        generator(rows, cols, field);
    } while (check(rows, cols, field));

    print_field(rows, cols, field);

    int x, y;
    while (1) {
        printf("Enter column to move from and column to move to (or 0 0 to check and exit): ");
        scanf("%d", &x);
        scanf("%d", &y);

        if (x == 0 && y == 0) {
            if (check(rows, cols, field)) {
                printf("Congratulations! You won!\n");
            } else {
                printf("Not all columns are completed. Exiting game.\n");
            }
            break;
        }

        down_possible(rows, cols, field, x, y);
        print_field(rows, cols, field);

        if (check(rows, cols, field)) {
            printf("Congratulations! You won!\n");
            break;
        }

        if (!move_possible(rows, cols, field)) {
            printf("No more possible moves. Exiting game.\n");
            break;
        }
    }
}

