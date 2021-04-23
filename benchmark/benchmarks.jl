using BenchmarkTools

using FileIO
using LinearAlgebra
using NRRD
using Printf

using IM2GR

# setup
BenchmarkTools.DEFAULT_PARAMETERS.overhead = BenchmarkTools.estimate_overhead()
BenchmarkTools.DEFAULT_PARAMETERS.seconds = 300 

println("overhead = ", BenchmarkTools.DEFAULT_PARAMETERS.overhead)

mri_diff_fn(xi, xj) = min(sqrt(xi) / 63, 1.0) - min(sqrt(xj) / 63, 1.0)
mri = load("data/lgemri.nrrd")

println("Running once to compile")
im2gr(mri, 2, IM2GR.CM_SingleThread, mri_diff_fn)
println("Benchmarking SingleThread")
stb = @benchmarkable im2gr($mri, 2, IM2GR.CM_SingleThread, $mri_diff_fn)
stb_ret = run(stb)
println("Saving results")
stb_res = open("benchmark/singlethread-mri-arrowhead.txt", "w+")
show(stb_res, MIME"text/plain"(), stb_ret)

println("Running once to compile")
im2gr(mri, 2, IM2GR.CM_MultiThread, mri_diff_fn)
println("Benchmarking MultiThread")
mtb= @benchmarkable im2gr($mri, 2, IM2GR.CM_MultiThread, $mri_diff_fn)
mtb_ret = run(mtb)
println("Saving results")
mtb_res = open("benchmark/multithread-mri-arrowhead.txt", "w+")
show(mtb_res, MIME"text/plain"(), mtb_ret)

#=
println("Benchmarking CUDA")
cb = @benchmarkable im2gr($mri, 2, IM2GR.CM_CUDA, $mri_diff_fn)
cb_ret = run(cb)
println("Saving results")
cb_res = open("benchmark/cuda.txt", "w+")
show(cb_res, MIME"text/plain"(), cb_ret)
=#

sz = (144, 144, 22)
fake = rand(UInt8, sz)

println("Running once to compile")
im2gr(fake, 1, IM2GR.CM_SingleThread)
println("Benchmarking... SingleThread")
fstb = @benchmarkable im2gr($fake, 1, IM2GR.CM_SingleThread)
fstb_ret = run(fstb)
println("Saving results")
fstb_res = open("benchmark/singlethread-fake-arrowhead.txt", "w+")
show(fstb_res, MIME"text/plain"(), fstb_ret)

println("Running once to compile")
im2gr(fake, 1, IM2GR.CM_MultiThread)
println("Benchmarking... MultiThread")
fmtb = @benchmarkable im2gr($fake, 1, IM2GR.CM_MultiThread)
fmtb_ret = run(fmtb)
println("Saving results")
fmtb_res = open("benchmark/multithread-fake-arrowhead.txt", "w+")
show(fmtb_res, MIME"text/plain"(), fmtb_ret)

#=
println("Running once to compile")
im2gr(fake, 1, IM2GR.CM_CUDA)
println("Benchmarking... CUDA")
fcb = @benchmarkable im2gr($fake, 1, IM2GR.CM_CUDA)
fcb_ret = run(fcb)
println("Saving results")
fcb_res = open("benchmark/cuda-fake-17-blocks.txt", "w+")
show(fcb_res, MIME"text/plain"(), fcb_ret)
=#
