using Base.Threads
using Printf

const N = 1_000_000
const NUM_THREADS = 4

# Thread-safe array and lock
const primes = Vector{Int}()
const lock = ReentrantLock()

# Prime checking function
function is_prime(n::Int)
    if n ≤ 1
        return false
    elseif n == 2
        return true
    elseif iseven(n)
        return false
    end
    limit = isqrt(n)
    for i in 3:2:limit
        if n % i == 0
            return false
        end
    end
    return true
end

# Worker function (used within threads)
function find_primes_range(start::Int, stop::Int)
    local_primes = Int[]
    for n in start:stop
        if is_prime(n)
            push!(local_primes, n)
        end
    end
    lock(lock) do
        append!(primes, local_primes)
    end
end

# Main execution
function main()
    println(" Finding primes up to $N using $NUM_THREADS threads...")
    GC.gc()
    start_time = time()

    # Thread pool (manual thread splitting)
    chunk = div(N, NUM_THREADS)
    threads = []

    allocated = @allocated begin
        for i in 1:NUM_THREADS
            start_idx = (i - 1) * chunk + 1
            end_idx = (i == NUM_THREADS) ? N : i * chunk
            t = @spawn find_primes_range(start_idx, end_idx)
            push!(threads, t)
        end

        # Wait for all threads
        for t in threads
            fetch(t)
        end
    end

    elapsed = time() - start_time

    println(" Total primes found: ", length(primes))
    @printf("  Time taken       : %.2f seconds\n", elapsed)
    @printf(" Memory used      : %.2f MB\n", allocated / 1_000_000)
end

main()
