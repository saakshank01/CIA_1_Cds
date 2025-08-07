/*MATRIX*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

#define N 300

int main() {
    static int A[N][N], B[N][N], R[N][N];

    printf("Initializing matrices...\n");

    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            R[i][j] = 0;
        }
    }

    HANDLE hProcess = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc_before, pmc_after;
    GetProcessMemoryInfo(hProcess, &pmc_before, sizeof(pmc_before));

    printf("Starting matrix multiplication for %dx%d matrices...\n", N, N);

    clock_t start = clock();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                R[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    clock_t end = clock();

    GetProcessMemoryInfo(hProcess, &pmc_after, sizeof(pmc_after));
    CloseHandle(hProcess);

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Matrix multiplication complete.\n");
    printf("Execution Time     : %.4f seconds\n", time_taken);

    SIZE_T mem_before = pmc_before.PeakWorkingSetSize;
    SIZE_T mem_after = pmc_after.PeakWorkingSetSize;

    printf("Peak Memory Before : %.2f MB\n", mem_before / (1024.0 * 1024.0));
    printf("Peak Memory After  : %.2f MB\n", mem_after / (1024.0 * 1024.0));
    printf("Memory Used During : %.2f MB\n", 
           (mem_after - mem_before) / (1024.0 * 1024.0));

    return 0;
}