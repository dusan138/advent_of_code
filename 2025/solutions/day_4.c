#include <stdio.h>

#define NUM_COLS 139
#define NUM_ROWS 139


void *load_map(FILE *fp, char map[NUM_ROWS][NUM_COLS]) {
    char line[NUM_COLS + 2];
    int row = 0;

    while (row < NUM_ROWS && (fgets(line, sizeof(line), fp))) {
        for (int k = 0; k < NUM_COLS; k++) {
            map[row][k] = line[k];
        }

        row++;
    }
}


int element_accessible_check(
    char map[NUM_ROWS][NUM_COLS], int row, int col
) {
    char elem = map[row][col];
    int cntr = 0;   // Count how many elements around are '@'

    int start_row, end_row;
    int start_col, end_col;

    start_row = row == 0 ? 0 : row - 1;
    end_row = row == NUM_ROWS - 1 ? NUM_ROWS - 1 : row + 1;
    
    start_col = col == 0 ? 0 : col - 1;
    end_col = col == NUM_COLS - 1 ? NUM_COLS - 1 : col + 1;

    for (int n = start_row; n <= end_row; n++) {
        for (int m = start_col; m <= end_col; m++) {
            if ((n == row) && (m == col)) {
                continue;
            }

            if ((map[n][m] == '@') || (map[n][m] == 'x')) {
                cntr++;
            }
        }
    }

    return cntr < 4;
}


int main(int argc, char *argv[]) {
    FILE *fp;
    char map[NUM_ROWS][NUM_COLS];
    int first_round_result = -1;
    int result = 0, old_result = -1;

    if (argc < 2) {
        printf("Please provide the input file.\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    // fp = fopen("test_input.txt", "r");
    if (fp != NULL) {
        load_map(fp, map);
    } else {
        printf("Not able to open the input file.\n");
        return 1;
    }

    while (result != old_result) {
        old_result = result;
        for (int k = 0; k < NUM_ROWS; k++) {
            for (int j = 0; j < NUM_COLS; j++) {
                if (map[k][j] == '@') {
                    if(element_accessible_check(map, k, j)) {
                        result += 1;
                        map[k][j] = 'x';
                    }
                }
            }
        }
        if (first_round_result == -1) {
            first_round_result = result;
        }
        // Transform all 'x' into '.'
        for (int k = 0; k < NUM_ROWS; k++) {
            for (int j = 0; j < NUM_COLS; j++) {
                if (map[k][j] == 'x') {
                    map[k][j] = '.';
                }
            }
        }
    }

    printf("Solution to Task 1: %d\n", first_round_result);
    printf("Solution to Task 2: %d\n", result);
    fclose(fp);

    return 0;
}