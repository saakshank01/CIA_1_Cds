#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <psapi.h>

#define N 1000000
#define NUM_THREADS 4

int *primes;
int prime_count = 0;
CRITICAL_SECTION lock;

typedef struct {
    int start;
    int end;
} ThreadArgs;

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    int sqrt_n = (int)sqrt(n);
    for (int i = 3; i <= sqrt_n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

DWORD WINAPI find_primes(LPVOID param) {
    ThreadArgs* range = (ThreadArgs*)param;
    int start = range->start;
    int end = range->end;

    for (int i = start; i < end; ++i) {
        if (is_prime(i)) {
            EnterCriticalSection(&lock);
            primes[prime_count++] = i;
            LeaveCriticalSection(&lock);
        }
    }
    free(range);
    return 0;
}

double get_time_in_seconds() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart / 10000000.0; // Convert to seconds
}

double get_memory_in_mb() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.PeakWorkingSetSize / (1024.0 * 1024.0);
    }
    return 0.0;
}

int main() {
    HANDLE threads[NUM_THREADS];
    primes = (int*)malloc(N * sizeof(int));
    if (!primes) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    InitializeCriticalSection(&lock);

    double start_time = get_time_in_seconds();

    int chunk = N / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->start = i * chunk;
        args->end = (i == NUM_THREADS - 1) ? N : (i + 1) * chunk;
        threads[i] = CreateThread(NULL, 0, find_primes, args, 0, NULL);
    }

    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

    double end_time = get_time_in_seconds();
    double elapsed = end_time - start_time;
    double memory_used = get_memory_in_mb();

    printf("\nTotal primes found : %d\n", prime_count);
    printf("Time taken          : %.4f seconds\n", elapsed);
    printf("Peak memory used   : %.2f MB\n", memory_used);

    DeleteCriticalSection(&lock);
    free(primes);

    return 0;
}


