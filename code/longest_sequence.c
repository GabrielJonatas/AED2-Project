#include <stdio.h>

int longest_repeated_pattern_occurrences(int shifts[], int n, int pattern_length) {
    if (n == 0) return 0; // No shifts, no occurrences
    
    int max_length = 1;
    int current_length = 1;

    for (int i = 1; i < n; i++) {
        if (shifts[i] - shifts[i - 1] == pattern_length) {
            current_length++;
            if (current_length > max_length) {
                max_length = current_length;
            }
        } else {
            current_length = 1;
        }
    }

    return max_length;
}

int main() {
    // Example input: shifts array and pattern length
    int shifts[] = {0, 3, 6, 9, 15}; // Example positions where the pattern occurs
    int n = sizeof(shifts) / sizeof(shifts[0]); // Number of shifts
    int pattern_length = 3; // Length of the pattern

    int result = longest_repeated_pattern_occurrences(shifts, n, pattern_length);

    printf("Longest sequence of repeated pattern occurrences: %d\n", result);
    return 0;
}
