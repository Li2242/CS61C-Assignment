#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

int main() {
    // Allocate a lookup table to mark which 16-bit states have appeared.
    // We use int8_t to keep memory usage small (0 = unseen, 1 = seen).
    int8_t *numbers = (int8_t*) malloc(sizeof(int8_t) * 65535);
    if (numbers == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }

    // Initialize all states as unseen.
    memset(numbers, 0, sizeof(int8_t) * 65535);

    // Start from a non-zero seed (LFSR gets stuck at 0).
    uint16_t reg = 0x1;

    // count tracks how many unique states we generate before repeating.
    uint32_t count = 0;
    int i;

    do {
        count++;

        // Mark the current register value as seen.
        numbers[reg] = 1;

        // Print only the first few values to avoid excessive output.
        if (count < 24) {
            printf("My number is: %u\n", reg);
        } else if (count == 24) {
            printf(" ... etc etc ... \n");
        }

        // Advance the LFSR 32 times before checking for repetition again.
        // This is just how this test is configured; it still validates period.
        for (i = 0; i < 32; i++)
            lfsr_calculate(&reg);

        // Stop when we revisit a previously seen state.
    } while (numbers[reg] != 1);

    printf("Got %u numbers before cycling!\n", count);

    // A maximal 16-bit LFSR should cycle through 2^16 - 1 = 65535 states.
    if (count == 65535) {
        printf("Congratulations! It works!\n");
    } else {
        printf("Did I miss something?\n");
    }

    // Clean up heap memory before exiting.
    free(numbers);

    return 0;
}
