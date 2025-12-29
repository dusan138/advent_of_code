# Advent of Code 2025 Walkthrough

## Table of Contents

* [Day 1 - Secret Entrance](#day-1---secret-entrance)

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

