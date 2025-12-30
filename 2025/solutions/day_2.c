#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE_STEP 128
#define MAX_NUMBER 9999999999L


typedef struct {
    long start;
    long end;
} Range;


char* read_input(FILE *fp) {
    // Reads the input file into one single character array
    char *line = NULL;
    int c;
    int len = 0;
    int capacity = 0;

    while ((c = fgetc(fp)) != EOF && c != '\n') {
        if (len + 1 > capacity) {
            char *new_line = realloc(line, len + INPUT_BUFFER_SIZE_STEP);
            if (new_line == NULL) {
                printf("Memory allocation failed");
                free(line);
                return NULL;
            }

            line = new_line;
            capacity += INPUT_BUFFER_SIZE_STEP;
        }
        line[len++] = (char)c;
    }

    if (line) {
        line[len] = '\0';
    }
    return line;
}


Range* exrtact_ranges(char *line, int *range_count) {
    // Extracts ranges from the character array into an array
    Range *ranges = NULL;
    long start, end;
    char *token;

    *range_count = 0;

    token = strtok(line, ",");
    while (token != NULL) {
        ranges = realloc(ranges, (*range_count + 1)*sizeof(Range));

        if (sscanf(token, "%ld-%ld", &start, &end) == 2) {
            ranges[*range_count].start = start;
            ranges[*range_count].end = end;
            (*range_count)++;
        } else {
            printf("Could not parse token: %s\n", token);
        }
        token = strtok(NULL, ",");
    }

    return ranges;
}


long long solve_task_1(Range *ranges, int range_count) {
    long powers_of_10[] = {1, 10, 100, 1000, 10000, 100000};
    long long result = 0;

    // We loop form 1 to 5 digits and construct numbers that are repeated
    // sequences by doing sequence + power_of_10 * sequence (multiplying by
    // power of 10 for a given digit will basically give us the left part of 
    // final number). Finally, check if a number constructed this way falls
    // within some of the ranges.
    for (int digits = 1; digits <= 5; digits++) {
        long power_of_10 = powers_of_10[digits];
        long sequence_end = power_of_10;
        long sequence_start = power_of_10 / 10;

        if (sequence_start == 0) {
            sequence_start = 1;
        }

        for (long seq = sequence_start; seq < sequence_end; seq++) {
            long number = seq + seq * power_of_10;
            for (int range_cntr = 0; range_cntr < range_count; range_cntr++) {
                if (
                    (number >= ranges[range_cntr].start)
                    && (number <= ranges[range_cntr].end)
                ) {
                    result += number;
                    break;
                }
            }
        }
    }

    return result;
}


int is_already_checked(long num, long *arr, int count) {
    // Binary search to check if the number is already in checked numbers
    int low = 0, high = count - 1;
    while (low <= high) {
        int mid = (high + low) / 2;

        if (arr[mid] == num) {
            return 1;
        } else if (arr[mid] < num) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return 0;
}


void expand_checked_array(long num, long **arr, int *count) {
    // Performs a sorted insertion of a number into array
    *count += 1;
    *arr = realloc(*arr, (*count) * sizeof(long));

    
    int k = *count - 2;  // We want to start from the end before the expansion
    while(k >= 0 && (*arr)[k] > num) {
        (*arr)[k + 1] = (*arr)[k];
        k--;
    }
    (*arr)[k + 1] = num;
}


long long solve_task_2(Range *ranges, int range_count, long limit) {
    long powers_of_10[] = {1, 10, 100, 1000, 10000, 100000};
    long long result = 0;

    // Here we need to create sequences that can repeat more than twice as
    // well. To do that, we construct a sequence and then repeat it as many
    // times as necessary before it becomes larger than the maximum number
    // from the ranges

    // There's a problem here: sequence 2222 can be constructed by either
    // repeating 2 four times, or 22 two times. Because of that, we need to
    // store the already checked numbers somewhere and then check if we
    // already checked the number

    long *checked_numbers = NULL;
    int checked_count = 0;

    for (int digits = 1; digits <= 5; digits++) {
        long power_of_10 = powers_of_10[digits];
        long sequence_end = power_of_10;
        long sequence_start = power_of_10 / 10;

        if (sequence_start == 0) {
            sequence_start = 1;
        }

        for (long seq = sequence_start; seq < sequence_end; seq++) {
            long number = seq + seq * power_of_10;
            long current_power_of_10 = power_of_10;

            while (number > 0 && number <= limit) {
                for (
                    int range_cntr = 0; range_cntr < range_count; range_cntr++
                ) {
                    if (
                        (number >= ranges[range_cntr].start)
                        && (number <= ranges[range_cntr].end)
                    ) {
                        if (!is_already_checked(
                            number, checked_numbers, checked_count
                        )) {
                            result += number;
                            expand_checked_array(
                                number, &checked_numbers, &checked_count
                            );
                        }
                        break;
                    }
                }

                if (current_power_of_10 > limit / power_of_10) {
                    break;
                }
                current_power_of_10 *= power_of_10;
                
                if (number > limit - seq) {
                    break;
                }
                number = number * power_of_10 + seq;
            }
        }
    }

    free(checked_numbers);
    return result;
}


int main(int argc, char *argv[]) {

    FILE *fp;
    Range *ranges;
    int range_count = 0;
    char *line = NULL;
    long long result_1, result_2;

    if (argc < 2) {
        printf("Please provide the input file.\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (fp != NULL) {
        line = read_input(fp);
        if (line == NULL) {
            fclose(fp);
            return 1;
        }

        ranges = exrtact_ranges(line, &range_count);

        long limit = 0;
        for (int k = 0; k < range_count; k++) {
            if (ranges[k].end > limit) {
                limit = ranges[k].end;
            }
        }
        result_1 = solve_task_1(ranges, range_count);
        result_2 = solve_task_2(
            ranges, range_count, limit
        );
        free(line);
    }
    else {
        printf("Not able to open the input file.");
        return 1;
    }

    printf("Solution for Task 1: %lld\n", result_1);
    printf("Solution for Task 2: %lld\n", result_2);
    fclose(fp);
}