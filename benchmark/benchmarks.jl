using BenchmarkTools

using FileIO
using LinearAlgebra
using NRRD
using Printf

using IM2GR

#=
mri_diff_fn(xi, xj) = min(sqrt(xi) / 63, 1.0) - min(sqrt(xj) / 63, 1.0)

mri = load("data/lgemri.nrrd")

@printf("Image size: %d, %d, %d\n", size(mri)...)
println(ndims(mri))
println("Benchmarking... MultiThread")
@time im2gr(mri, 1, IM2GR.MultiThread, mri_diff_fn)
println("Done.")
=#

sz = (144, 144, 22)
fake = rand(UInt8, sz)
@printf("Image size: %d, %d, %d\n", sz...)
println("Benchmarking... SingleThread")
@btime im2gr($fake, 1, IM2GR.SingleThread)
println("Done.")

println("Benchmarking... MultiThread")
@btime im2gr($fake, 1, IM2GR.MultiThread)
println("Done.")

