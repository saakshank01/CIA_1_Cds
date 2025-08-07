#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>

#define N 100000

void mrg(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void mrg_srt(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mrg_srt(arr, l, m);
        mrg_srt(arr, m + 1, r);
        mrg(arr, l, m, r);
    }
}

int main() {
    int *arr = (int *)malloc(N * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++) {
        arr[i] = rand();
    }

    HANDLE hProcess = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc_before, pmc_after;
    GetProcessMemoryInfo(hProcess, &pmc_before, sizeof(pmc_before));

    clock_t start = clock();

    mrg_srt(arr, 0, N - 1);

    clock_t end = clock();
    GetProcessMemoryInfo(hProcess, &pmc_after, sizeof(pmc_after));
    CloseHandle(hProcess);

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    SIZE_T mem_before = pmc_before.PeakWorkingSetSize;
    SIZE_T mem_after = pmc_after.PeakWorkingSetSize;

    printf("Sorting complete.\n");
    printf("Execution Time     : %.4f seconds\n", time_spent);
    printf("Peak Memory Before : %.2f MB\n", mem_before / (1024.0 * 1024.0));
    printf("Peak Memory After  : %.2f MB\n", mem_after / (1024.0 * 1024.0));
    printf("Memory Used During : %.2f MB\n", (mem_after - mem_before) / (1024.0 * 1024.0));

    free(arr);
    return 0;
}
