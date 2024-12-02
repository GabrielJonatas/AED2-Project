#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Function prototypes
void brute_force_matcher(const char *T, const char *P, int n, int m);
double benchmark_brute_force(const char *text, const char *pattern, int text_size, int pattern_size);
void run_benchmark(const char *file_path, const char *pattern, int pattern_size, int k, FILE *results_file);
char *generate_pattern_multiple(const char *pattern, int pattern_length, int multiplier);


void brute_force_matcher(const char *T, const char *P, int n, int m) {
    for (int s = 0; s <= n - m; s++) {
        int match = 1; // Assume a match until proven otherwise
        for (int j = 0; j < m; j++) {
            if (P[j] != T[s + j]) {
                match = 0; // Mismatch found
                break;
            }
        }
        if (match) {
            printf("Pattern occurs with offset %d\n", s);
        }
    }
}

// Benchmark a single execution of the KMP algorithm
double benchmark_brute_force(const char *text, const char *pattern, int text_size, int pattern_size) {
    clock_t start_time = clock();
    brute_force_matcher(text, pattern, text_size, pattern_size);
    clock_t end_time = clock();
    return ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
}

void run_benchmark(const char *file_path, const char *pattern, int pattern_size, int k, FILE *results_file) {
    // Load the text from the file
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *text = malloc(file_size + 1);
    if (!text) {
        perror("Failed to allocate memory");
        fclose(file);
        exit(1);
    }

    fread(text, 1, file_size, file);
    text[file_size] = '\0';
    fclose(file);

    // Run the benchmark `k` times for the given pattern
    for (int i = 0; i < k; i++) {
        double execution_time = benchmark_brute_force(text, pattern, file_size, pattern_size);
        fprintf(results_file, "Run %d: %.6f seconds\n", i + 1, execution_time);
        printf("Run %d: %.6f seconds\n", i + 1, execution_time); // Optional: print to console
    }

    // Clean up
    free(text);
}


// Generate a pattern by concatenating it `multiplier` times
char *generate_pattern_multiple(const char *pattern, int pattern_length, int multiplier) {
    int total_length = pattern_length * multiplier;

    char *result = malloc(total_length + 1);
    if (!result) {
        perror("Failed to allocate memory for pattern multiple");
        exit(1);
    }

    result[0] = '\0'; // Start with an empty string
    for (int i = 0; i < multiplier; i++) {
        strcat(result, pattern);
    }

    return result;
}

// Main function
int main() {
    const char *file_path = "HTT.txt";    // Input text file
    const char *base_pattern = "cag";    // Base pattern
    const int base_pattern_length = strlen(base_pattern);
    int k = 5;                            // Number of test runs for each pattern
    int L = 4;                            // Maximum multiplier for the pattern
    const char *output_file = "brute_force_results.txt"; // Output file to store results

    // Open the results file once in write mode
    FILE *results_file = fopen(output_file, "w");
    if (!results_file) {
        perror("Failed to open output file");
        exit(1);
    }

    for (int multiplier = 1; multiplier <= L; multiplier++) {
        char *current_pattern = generate_pattern_multiple(base_pattern, base_pattern_length, multiplier);

        fprintf(results_file, "Benchmarking pattern of length %ld (multiplier %d):\n", 
                strlen(current_pattern), multiplier);
        printf("Benchmarking pattern of length %ld (multiplier %d):\n", 
                strlen(current_pattern), multiplier);

        run_benchmark(file_path, current_pattern, base_pattern_length * multiplier, k, results_file);

        free(current_pattern);
    }

    fclose(results_file); // Close the results file at the end
    printf("Benchmark complete. Results saved to '%s'.\n", output_file);

    return 0;
}

