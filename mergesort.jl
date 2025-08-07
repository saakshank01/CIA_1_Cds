
using BenchmarkTools

function merge_sort(arr)
    if length(arr) ≤ 1
        return arr
    end
    mid = div(length(arr), 2)
    left = merge_sort(arr[1:mid])
    right = merge_sort(arr[mid+1:end])
    return merge(left, right)
end

function merge(left, right)
    result = Int[]
    i, j = 1, 1
    while i ≤ length(left) && j ≤ length(right)
        if left[i] ≤ right[j]
            push!(result, left[i])
            i += 1
        else
            push!(result, right[j])
            j += 1
        end
    end
    append!(result, left[i:end])
    append!(result, right[j:end])
    return result
end

arr = rand(1:1_000_000, 100_000)

# Use BenchmarkTools to measure memory usage
bench_result = @benchmark merge_sort($arr)

# Extract memory in bytes
bytes = minimum(bench_result).memory
mib = bytes / (1024^2)
mb = mib / 1.048576

println("Peak memory used:")
println("→ In bytes: $bytes")
println("→ In MiB: $mib")
println("→ In MB : $mb")'


