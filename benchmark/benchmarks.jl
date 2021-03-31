using BenchmarkTools

using FileIO
using LinearAlgebra
using NRRD

using IM2GR

#=
mri_diff_fn(xi, xj) = min(sqrt(xi) / 63, 1.0) - min(sqrt(xj) / 63, 1.0)

mri_data = load("data/lgemri.nrrd")

image = Image{UInt8}(mri_data, 1)
println("Benchmarking... SingleThread")

# @btime im2gr!($image, IM2GR.SingleThread, mri_diff_fn)

println("Done.")

# reset image
update_image!(image, data=mri_data, d=1)

println("Benchmarking... MultiThread")
@time im2gr!(image, IM2GR.MultiThread, mri_diff_fn)
println("Done.")
=#

fake = rand(UInt8, (144, 144, 22))
image = Image{UInt8}(fake, 1)
println("Benchmarking... SingleThread")
@btime im2gr!($image, IM2GR.SingleThread)
println("Done.")

update_image!(image, data=fake, d=1)
println("Benchmarking... MultiThread")
@btime im2gr!($image, IM2GR.MultiThread)
println("Done.")

