import random
import time
import tracemalloc
import psutil
import os

SIZE = 100000

def get_memory_MB():
    process = psutil.Process(os.getpid())
    return process.memory_info().rss / (1024 * 1024)

mem_before = get_memory_MB()
print(f"Initial Process Memory : {mem_before:.2f} MB")

arr = [random.randint(0, 1000000) for _ in range(SIZE)]

mem_after_alloc = get_memory_MB()
print(f"After Allocation Memory: {mem_after_alloc:.2f} MB")

def merge_sort(arr):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    return merge(left, right)

def merge(left, right):
    result = []
    l = r = 0
    while l < len(left) and r < len(right):
        if left[l] < right[r]:
            result.append(left[l])
            l += 1
        else:
            result.append(right[r])
            r += 1
    result.extend(left[l:])
    result.extend(right[r:])
    return result

tracemalloc.start()
start_time = time.perf_counter()

sorted_arr = merge_sort(arr)

end_time = time.perf_counter()
current, peak = tracemalloc.get_traced_memory()
tracemalloc.stop()

mem_after = get_memory_MB()

print(f"\nExecution Time         : {end_time - start_time:.4f} seconds")
print(f"Peak Python Memory     : {peak / (1024 * 1024):.2f} MB")
print(f"Process Memory Before  : {mem_before:.2f} MB")
print(f"Process Memory After   : {mem_after:.2f} MB")
print(f"Total Process Mem Used : {mem_after - mem_before:.2f} MB")