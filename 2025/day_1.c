#include <stdio.h>
#include <stdlib.h>

#define LINE_LENGTH 10
#define START_POSITION 50

int process_command(
    char *line,
    int *zeros_count,
    int *zero_crosses_count,
    int current_position
) {
    // Process the command from input file
    char direction;
    int steps;
    int new_position;

    if (sscanf(line, "%c%d", &direction, &steps) == 2) {
        switch (direction) {
            case 'L':
                new_position = current_position - steps;
                break;
            case 'R':
                new_position = current_position + steps;
                break;
            default:
                break;
        }

        // Task 2: Count how many times cross over zero occured during the
        // execution of the command. Ending up at 0 does not count as crossing
        // and neither does starting at 0
        if (new_position > current_position) {
            *zero_crosses_count += new_position / 100;
            if ((steps % 100) == 100 - current_position) {
                // In this case we end up at 0, so subtract one from the
                // crosses, since it should not count as a cross
                *zero_crosses_count -= 1;
            } 
        } else if (new_position < 0) {
            *zero_crosses_count += -new_position / 100;
            // If we start at 0 and move into the negatives, we also need to
            // add 1
            *zero_crosses_count += current_position != 0;
            if ((steps % 100) == current_position) {
                // Same case, check if we end up at 0
                *zero_crosses_count -= 1;
            }
        }

        // Task 1: Count how many times we end up at 0 after a command
        new_position = new_position % 100;
        if (new_position < 0) {
            new_position = new_position + 100;
        }

        current_position = new_position;
        if (current_position == 0) {
            *zeros_count += 1;
        }
    } else {
        printf("Unable to parse command: %s", line);
    }

    return current_position;
}


int main(int argc, char *argv[]) {
    FILE *fp;
    char line[LINE_LENGTH];

    int zeros_count = 0; // For task 1
    int zero_crosses_count = 0; // For task 2
    int current_position = START_POSITION;

    if (argc < 2) {
        printf("Please provide the input file.\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (fp != NULL) {
        while(fgets(line, sizeof(line), fp)) {
            current_position = process_command(
                line, &zeros_count, &zero_crosses_count, current_position
            );
        }
    }
    else {
        printf("Not able to open the input file.");
    }

    printf("Solution for Task 1: %d\n", zeros_count);
    printf("Solution for Task 2: %d\n", zeros_count + zero_crosses_count);
    fclose(fp);
}
