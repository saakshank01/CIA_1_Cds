# Enable multi-threading
using Base.Threads

# Size of matrices
N = 300

# Random matrices
A = rand(N, N)
B = rand(N, N)
C = zeros(N, N)  # Result matrix

# Warm-up
_ = A * B

# Matrix multiplication with threading
function threaded_matmul!(C, A, B)
    @threads for i in 1:size(A, 1)
        for j in 1:size(B, 2)
            sum = 0.0
            for k in 1:size(A, 2)
                sum += A[i, k] * B[k, j]
            end
            C[i, j] = sum
        end
    end
end

# Measure memory used for result only
allocated = @allocated threaded_matmul!(C, A, B)

# Measure execution time
println("Starting threaded multiplication...")
@time threaded_matmul!(C, A, B)

println("\nMatrix multiplication completed.")
println("→ Output matrix size: ", size(C))
println("→ Memory used for output (bytes): $allocated")
println("→ Memory used (MB): ", allocated / 1_000_000)
