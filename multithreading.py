#multithreading
import threading
import time
import tracemalloc
import math

# Parameters
N = 1000000
NUM_THREADS = 4
primes = []

# Thread-safe lock for appending results
lock = threading.Lock()

# Prime check function
def is_prime(n):
    if n <= 1:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    sqrt_n = int(math.sqrt(n)) + 1
    for i in range(3, sqrt_n, 2):
        if n % i == 0:
            return False
    return True

# Worker function
def find_primes(start, end):
    local_primes = []
    for number in range(start, end):
        if is_prime(number):
            local_primes.append(number)
    with lock:
        primes.extend(local_primes)

# Main
if __name__ == "__main__":
    tracemalloc.start()
    start_time = time.time()

    threads = []
    chunk = N // NUM_THREADS

    for i in range(NUM_THREADS):
        start = i * chunk
        end = N if i == NUM_THREADS - 1 else (i + 1) * chunk
        t = threading.Thread(target=find_primes, args=(start, end))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    end_time = time.time()
    current, peak = tracemalloc.get_traced_memory()
    tracemalloc.stop()

    print(f"Total primes found: {len(primes)}")
    print(f"Time taken        : {end_time - start_time:.2f} seconds")
    print(f"Peak memory used : {peak / (1024 * 1024):.2f} MB")