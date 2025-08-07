#using numpy->faster thn normal (~3sec)
import numpy as np
import time
import tracemalloc

N = 1000
A = np.random.randint(1, 10, size=(N, N))
B = np.random.randint(1, 10, size=(N, N))

tracemalloc.start()
start_time = time.time()

R = np.matmul(A, B)

end_time = time.time()
curr, peak = tracemalloc.get_traced_memory()
tracemalloc.stop()

print(f"Time Taken     : {end_time - start_time:.4f} seconds")
print(f"Peak Memory    : {peak / (1024 * 1024):.2f} MB")


#using loops
import random
import time
import tracemalloc
import psutil
import os

N = 300

def get_memory_MB():
    process = psutil.Process(os.getpid())
    return process.memory_info().rss / (1024 * 1024)

# Memory before anything
mem_before = get_memory_MB()
print(f"Initial Process Memory : {mem_before:.2f} MB")

# Generate matrices
A = [[random.randint(1, 9) for _ in range(N)] for _ in range(N)]
B = [[random.randint(1, 9) for _ in range(N)] for _ in range(N)]
R = [[0 for _ in range(N)] for _ in range(N)]

# Memory after allocation
mem_after_alloc = get_memory_MB()
print(f"After Allocation Memory: {mem_after_alloc:.2f} MB")

# Start measuring with tracemalloc (Python object only)
tracemalloc.start()
start_time = time.perf_counter()

# Matrix multiplication
for i in range(N):
    for j in range(N):
        for k in range(N):
            R[i][j] += A[i][k] * B[k][j]

end_time = time.perf_counter()
current, peak = tracemalloc.get_traced_memory()
tracemalloc.stop()

# Final memory
mem_after = get_memory_MB()

# Results
print(f"\nExecution Time         : {end_time - start_time:.4f} seconds")
print(f"Peak Python Memory     : {peak / (1024 * 1024):.2f} MB")  # Only Python object memory
print(f"Process Memory Before  : {mem_before:.2f} MB")
print(f"Process Memory After   : {mem_after:.2f} MB")
print(f"Total Process Mem Used : {mem_after - mem_before:.2f} MB")

