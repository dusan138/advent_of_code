# Advent of Code 2025 Walkthrough

## Table of Contents

* [Day 1 - Secret Entrance](#day-1---secret-entrance)
* [Day 2 - Gift Shop](#day-2---gift-shop)
* [Day 3 - Lobby](#day-3---lobby)
* [Day 4 - Printing Department](#day-4---printing-department)

## Day 1 - Secret Entrance

[Problem Statement](https://adventofcode.com/2025/day/1)

### Task 1

For the first task, we have a circular sequence of integers from `0` to `99`, and a pointer that starts at `50`. The input that we receive consists of commands of how to move that pointer around the sequence, and the first task is to determine the number of times we end up at `0` after finishing each of the commands.

To solve this, I we will go line by line in the input file, parsing each command using `sscanf`:

```c
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
```

After obtaining the new current position, we need to check whether we ended up at a multiple of `100`. If we did, that means that we are actually at `0`, since we 'ticked over'. This is a simple `%` operation, regardless of whether we moved to the left or to the right.

However, if we ended up in a negative position (by going left), we need to calculate our new position. This is simple, we just add `100`. So, if we ended at `-5`, we are actually at `95` due to the cyclical nature of the sequence. Finally, we check if we are at 0 and increase our counter by 1 if we did.

### Task 2

This one is a bit tricker. We need to count how many times we have crossed over `0` during the execution of a single command, while keeping in mind that ending up at `0` should not count as crossing over it.

Conceptually, we can use a similar approach with calculating the `new_position` and then just take the quotient of the division of `new_position` by `100`. We have to keep in mind that doing this with negative numbers will give a floor quotient, which is fine.

There are a couple of edge cases here. Calculating the crossings from `99` to `0` this way will also include the cases where we end up at `0`. However, this should not be counted (as per the problem definition and example given there). To cover for this, we need to check whether the modulo of the `new_position` is equal to the number of steps needed to arrive at `0`, which is calculated by:

```c
if (new_position > current_position) {
    // Rest of code ...
    if ((steps % 100) == 100 - current_position) {
        // In this case we end up at 0, so subtract one from the
        // crosses, since it should not count as a cross
        *zero_crosses_count -= 1;
    } 
} else if (new_position < 0) {
    // Rest of code ...
    if ((steps % 100) == current_position) {
        // Same case, check if we end up at 0
        *zero_crosses_count -= 1;
    }
}
```

And finally, starting from `0` and going into negative numbers will not be counted this way, but it should be. That is why we need to add:

```c
*zero_crosses_count += current_position != 0;
```

[Back to top](#table-of-contents)

## Day 2 - Gift Shop

[Problem Statement](https://adventofcode.com/2025/day/2)

### Task 1

The input for this day is a file with ranges of numbers separated by `,`. Our task is to sum all the numbers in these ranges that consist only of some sequence of digits repeated twice (so, `55`, `123123`, `6464`, etc.).

> [!IMPORTANT]
> A quick check gives us that the largest number in the input data is a 10 digits number. This means that we will probably have to deal with `long` and `long long` numbers to avoid overflow.

A brute force approach to this is pretty easy: we parse the input, split the lines, generate all the numbers in the given ranges, get their string representations and check if the first part of the string is equal to the second part. Pretty easy to do and to write (in `python`, at least). However, the main issue with this approach is that it's really slow if we are dealing with large numbers, which we are. Ideally, we would like to avoid this brute force solution.

One way to do this is to generate the numbers consisting of two repeated sequences and then check if they fall somewhere between the ranges that we are given.

Since the biggest number we have as input is 10 digits long, we only need to check the sequences up to 5 digits and repeat them. We loop through these digits, construct sequences of numbers and then create the numbers with repeated sequences:

```c
for (int digits = 1; digits <= 5; digits++) {
        long power_of_10 = powers_of_10[digits];
        long sequence_end = power_of_10;
        long sequence_start = power_of_10 / 10;

        if (sequence_start == 0) {
            sequence_start = 1;
        }

        for (long seq = sequence_start; seq < sequence_end; seq++) {
            long number = seq + seq * power_of_10;
            // Loop through the ranges and check if the number is in one
            // of them ...
        }
}
```

### Task 2

The solution for task 2 extends on the previous. We only looked at sequences that repeat once. To solve task 2, we extend this soultion to repeat sequences multiple times and then check if they are in the ranges. We do this as many times before the number represented by repeated sequences becomes larger than the largest number in the range.

An issue that occurs here is that we can arrive at some numbers by repeating different sequences. So, for example, we can get `12121212` by repeating `12` four times, but also by repeating `1212` two times. To solve this, we just need to keep track of the already generated numbers.

[Back to top](#table-of-contents)

## Day 3 - Lobby

[Problem Statement](https://adventofcode.com/2025/day/3)

### Task 1

The first task for this day is to find two numbers form the largest two digit number in a sequence of numbers. So, for a sequence `13112` the output would be `32`. We actually need to sum these two digit numbers for multiple sequences, but this is the main problem to solve today.

A brute force solution, and the one I have first thought of, is to track these two digits - `left_digit` and `right_digit`. The two digit number is then calculated as `left_digit * 10 + right_digit`. We go through the array once and at each iteration do the following:

* Check if the current number is higher than the `left_digit`. 
    * If it is, we use it as the `left_digit` and set the `right_digit` to the next number.
    * If it is not higher than the `left_digit`, check if it is higher than `right_digit` and set it as the right one if it is.

We replace the left digit only if we are not at the last digit. One way to implement this is with a `while` loop:

```c
int process_line_task_1(char *line) {
    int cntr = 1;
    int left_digit = line[0] - 48, right_digit = line[1] - 48;

    while (cntr < LINE_LENGTH) {
        int current_digit = line[cntr] - 48;
        if ((current_digit > left_digit) && (cntr != LINE_LENGTH - 1)) {
            left_digit = current_digit;
            right_digit = line[cntr + 1] - 48;
        }
        else if (current_digit > right_digit) {
            right_digit = current_digit;
        }
        cntr++;
    }

    return left_digit * 10 + right_digit;
}
```

### Task 2

As with previous day, this is an extension to the Task 1. Except that we need to find a 12 digit number now. The solution from [Task 1](#task-1-2) can be extendend to recursively handle this, but I wanted to see if I can come up with something smarter.

In order to solve this, we will use an array (which I will call `buffer`) to store the largest digits encountered so far (in order). We go through the input line and store the elements in this `buffer`. The elements are stored in ordered maner, and we discard any elements that are lower than the one we are storing.

So, for example, let us imagine that our input is `2142312` and we need to find a three digits number. We initialize the `buffer` with size `3` and loop through the input. The logic is as follows:

1) Input number: `2`; Buffer: empty -> Store the number in `buffer`
2) Input number: `1`; Buffer: `2` -> Since `1` is smaller than `2`, add it to the end of `buffer`
3) Input number: `4`; Buffer: `21` -> Since `4` is larger than both `1` and `2`, empty the buffer and store `4`
4) Input number: `2`; Buffer: `4` -> Add `2` to the `buffer`
5) Input number: `3`; Buffer: `42` -> Since `3` is larger than `2`, it replaces it. The `buffer` is now `43`

And so forth. As we approach the end of the input array, we need to limit the number of places that can be replaced in the buffer. So, in our example where we are constructing the three digit number, second to last number in input cannot replace the first number in buffer, and last number in input can only replace the last number in buffer.

This solution is linear when solving each line of input, and generalizes well. It can als be used to solve the first task of this day.

[Back to top](#table-of-contents)

## Day 4 - Printing Department

[Problem Statement](https://adventofcode.com/2025/day/4)

### Task 1

The solution for this is pretty straightforward. We loop through the map and, if the current element is `'@'`, check whether the elements around the current one are `'@'`. We implement this checking through `for` loops to avoid hardcoding. We also need to account for some edge cases, but this is, again, pretty straightforward:

```c
int element_accessible_check(
    char map[NUM_ROWS][NUM_COLS], int row, int col
) {
    char elem = map[row][col];
    int cntr = 0;

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

            if (map[n][m] == '@') {
                cntr++;
            }
        }
    }

    return cntr < 4;
}
```

### Task 2

The second task requires us to modify the approach slightly. We again pass through the whole map once, but we mark the elements that are accessible. These elements are then removed from the map and replaced with `'.'`. We then repeat the same process until the number of accessible elements has not changed. So, basically, we envelop the Task 1 in a `while` loop.

[Back to top](#table-of-contents)
