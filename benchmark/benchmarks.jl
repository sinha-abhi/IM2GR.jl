using BenchmarkTools

using FileIO
using LinearAlgebra
using NRRD
using Printf

using IM2GR

# setup
BenchmarkTools.DEFAULT_PARAMETERS.overhead = BenchmarkTools.estimate_overhead()
BenchmarkTools.DEFAULT_PARAMETERS.seconds = 1800 # 30 minutes

mri_diff_fn(xi, xj) = min(sqrt(xi) / 63, 1.0) - min(sqrt(xj) / 63, 1.0)
mri = load("data/lgemri.nrrd")

println("Benchmarking SingleThread")
stb = @benchmarkable im2gr($mri, 2, IM2GR.CM_SingleThread, $mri_diff_fn)
stb_ret = run(stb)
println("Saving results")
stb_res = open("benchmark/singlethread.txt", "w+")
show(stb_res, MIME"text/plain"(), stb_ret)

println("Benchmarking MultiThread")
mtb= @benchmarkable im2gr($mri, 2, IM2GR.CM_MultiThread, $mri_diff_fn)
mtb_ret = run(mtb)
println("Saving results")
mtb_res = open("benchmark/multithread.txt", "w+")
show(mtb_res, MIME"text/plain"(), mtb_ret)

println("Benchmarking CUDA")
cb = @benchmarkable im2gr($mri, 2, IM2GR.CM_CUDA, $mri_diff_fn)
cb_ret = run(cb)
println("Saving results")
cb_res = open("benchmark/cuda.txt", "w+")
show(cb_res, MIME"text/plain"(), cb_ret)

#=
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
=#

