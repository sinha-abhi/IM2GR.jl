using BenchmarkTools

using FileIO
using LinearAlgebra
using NRRD
using Printf

using IM2GR

mri_diff_fn(xi, xj) = min(sqrt(xi) / 63, 1.0) - min(sqrt(xj) / 63, 1.0)
mri = load("data/lgemri.nrrd")

#=
println("SingleThread")
@time image = im2gr(mri, 1, IM2GR.CM_SingleThread, mri_diff_fn)
println(norm(image.evi))

println("CUDA")
@time image = im2gr(mri, 1, IM2GR.CM_CUDA, mri_diff_fn)
println(norm(filter(!isnan, image.V)))
=#

sz = (144, 144, 22)
fake = rand(UInt8, sz)
@printf("Image size: %d, %d, %d\n", sz...)
println("Benchmarking... SingleThread")
@btime im2gr($fake, 1, IM2GR.CM_SingleThread)
println("Done.")

println("Benchmarking... MultiThread")
@btime im2gr($fake, 1, IM2GR.CM_MultiThread)
println("Done.")

println("Benchmarking... CUDA")
im2gr(fake, 1, IM2GR.CM_CUDA) # call once to compile kernel
@btime image = im2gr($fake, 1, IM2GR.CM_CUDA)
println("done")

