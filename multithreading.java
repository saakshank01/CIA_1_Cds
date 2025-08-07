import java.util.*;


public class  multithreading {
    static final int N = 1000000;
    static final int NUM_THREADS = 4;
    static final List<Integer> primes = Collections.synchronizedList(new ArrayList<>());

    public static void main(String[] args) {
        long startTime = System.currentTimeMillis();

        // Get memory usage before computation
        Runtime runtime = Runtime.getRuntime();
        runtime.gc(); // suggest garbage collection
        long startMemory = runtime.totalMemory() - runtime.freeMemory();

        int chunk = N / NUM_THREADS;
        Thread[] threads = new Thread[NUM_THREADS];

        for (int i = 0; i < NUM_THREADS; i++) {
            int start = i * chunk;
            int end = (i == NUM_THREADS - 1) ? N : (i + 1) * chunk;
            threads[i] = new Thread(new PrimeWorker(start, end));
            threads[i].start();
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        long endTime = System.currentTimeMillis();
        long endMemory = runtime.totalMemory() - runtime.freeMemory();

        // Output
        System.out.println("Total primes found: " + primes.size());
        System.out.printf("⏱ Time taken        : %.2f seconds%n", (endTime - startTime) / 1000.0);
        System.out.printf(" Peak memory used : %.2f MB%n", (endMemory - startMemory) / (1024.0 * 1024.0));
    }

    static class PrimeWorker implements Runnable {
        int start, end;

        PrimeWorker(int start, int end) {
            this.start = start;
            this.end = end;
        }

        @Override
        public void run() {
            List<Integer> localPrimes = new ArrayList<>();
            for (int i = start; i < end; i++) {
                if (isPrime(i)) {
                    localPrimes.add(i);
                }
            }
            primes.addAll(localPrimes);
        }

        boolean isPrime(int n) {
            if (n <= 1) return false;
            if (n == 2) return true;
            if (n % 2 == 0) return false;
            int sqrt = (int) Math.sqrt(n);
            for (int i = 3; i <= sqrt; i += 2) {
                if (n % i == 0) return false;
            }
            return true;
        }
    }
}
