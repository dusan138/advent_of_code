#include <stdio.h>

#define LINE_LENGTH 100
#define NUMBER_SIZE 12
#define ASCII_ZERO 48


void insert_into_buffer(
    int num, int *buffer, int *buffer_len, int change_start_idx
) {
    // Empty values that are less than the number
    while (
        (*buffer_len > change_start_idx) && (buffer[*buffer_len - 1] < num)
    ) {
        *buffer_len -= 1;
    }

    if (*buffer_len < NUMBER_SIZE) {
        // Put the number on the place it needs to be
        buffer[*buffer_len] = num;
        *buffer_len += 1;
    }
}


long process_line(char *line) {
    int buffer[NUMBER_SIZE];
    int number;
    int buffer_len = 0, change_start_idx = 0;
    long result = 0;

    for (int k = 0; k < LINE_LENGTH; k++) {
        if (k > LINE_LENGTH - NUMBER_SIZE - 1) {
            // Limit the index in buffer that we can change
            change_start_idx = NUMBER_SIZE - (LINE_LENGTH - k);
        }
        number = line[k] - ASCII_ZERO;
        insert_into_buffer(number, buffer, &buffer_len, change_start_idx);
    }

    for (int k = 0; k < NUMBER_SIZE; k++) {
        result = result * 10 + buffer[k];
    }
}


int main(int argc, char *argv[]){
    FILE *fp;
    char line[LINE_LENGTH + 2];
    long joltage = 0;
    long result = 0;

    if (argc < 2) {
        printf("Please provide the input file.\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (fp != NULL) {
        while(fgets(line, sizeof(line), fp)) {
            joltage = process_line(line);
            result += joltage;
        }
    }
    else {
        printf("Not able to open the input file.\n");
        return 1;
    }
    printf("Solution: %ld\n", result);
    return 0;
}
