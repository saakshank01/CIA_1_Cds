import java.util.Random;

public class matrix {
    public static void main(String[] args) {
        int n = 1000;
        int[][] A = new int[n][n];
        int[][] B = new int[n][n];
        int[][] R = new int[n][n];

        Random rand = new Random();

        // Fill matrices A and B with random integers
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i][j] = rand.nextInt(10);
                B[i][j] = rand.nextInt(10);
            }
        }

        Runtime runtime = Runtime.getRuntime();
        runtime.gc();  // Garbage collect before measuring memory
        long memoryBefore = runtime.totalMemory() - runtime.freeMemory();

        long startTime = System.nanoTime();

        // Matrix Multiplication
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                R[i][j] = 0;
                for (int k = 0; k < n; k++) {
                    R[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        long endTime = System.nanoTime();

        // DO NOT call gc() here
        long memoryAfter = runtime.totalMemory() - runtime.freeMemory();

        double timeInSeconds = (endTime - startTime) / 1e9;
        double memoryUsedDuringCalc = (memoryAfter - memoryBefore) / (1024.0 * 1024.0);
        double totalMemoryAfterMB = memoryAfter / (1024.0 * 1024.0);
        double memoryBeforeMB = memoryBefore / (1024.0 * 1024.0);

        System.out.printf("Process Memory Before : %.2f MB\n", memoryBeforeMB);
        System.out.printf("Execution Time         : %.4f seconds\n", timeInSeconds);
        System.out.printf("Memory Used for Calc   : %.2f MB\n", memoryUsedDuringCalc);
        System.out.printf("Total Memory After     : %.2f MB\n", totalMemoryAfterMB);
    }
}