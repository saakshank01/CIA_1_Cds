import java.util.Random;

public class mergesort{
    static final int N = 100000;

    static void merge(int[] a, int l, int m, int r) {
        int n1 = m - l + 1;
        int n2 = r - m;

        int[] L = new int[n1];
        int[] R = new int[n2];

        for (int i = 0; i < n1; i++) L[i] = a[l + i];
        for (int j = 0; j < n2; j++) R[j] = a[m + 1 + j];

        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            a[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
        }
        while (i < n1) a[k++] = L[i++];
        while (j < n2) a[k++] = R[j++];
    }

    static void mergeSort(int[] a, int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(a, l, m);
            mergeSort(a, m + 1, r);
            merge(a, l, m, r);
        }
    }

    public static void main(String[] args) {
        int[] a = new int[N];
        Random rnd = new Random();
        for (int i = 0; i < N; i++) a[i] = rnd.nextInt();

        Runtime rt = Runtime.getRuntime();
        rt.gc(); // Request GC to clean up before measurement

        long memBefore = rt.totalMemory() - rt.freeMemory();
        double memBeforeMB = memBefore / (1024.0 * 1024);

        long startTime = System.nanoTime();
        mergeSort(a, 0, N - 1);
        long endTime = System.nanoTime();

        long memAfter = rt.totalMemory() - rt.freeMemory();
        double memAfterMB = memAfter / (1024.0 * 1024);

        double usedMemory = memAfterMB - memBeforeMB;
        double timeSec = (endTime - startTime) / 1e9;

        System.out.printf("Execution Time         : %.4f seconds\n", timeSec);
        System.out.printf("Heap Memory Before     : %.2f MB\n", memBeforeMB);
        System.out.printf("Heap Memory After      : %.2f MB\n", memAfterMB);
        System.out.printf("Memory Used for Sort   : %.2f MB\n", usedMemory);
    }
}