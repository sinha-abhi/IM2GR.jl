using Pkg
if isfile("Project.toml") && isfile("Manifest.toml")
  Pkg.activate(".")
end

using BenchmarkTools

using FileIO
using LinearAlgebra
using NRRD
using Printf

using IM2GR
import IM2GR: __default_diff_fn

BenchmarkTools.DEFAULT_PARAMETERS.seconds = 100

function bm_singlethread(data, d, f, diff_fn=__default_diff_fn) 
  println("Running once to compile...")
  im2gr(data, d, IM2GR.CM_SingleThread, diff_fn)
  println("Running singlethread benchmarks...", f)
  b = @benchmarkable im2gr($data, $d, IM2GR.CM_SingleThread, $diff_fn)
  bret = run(b)
  res = open(f, "w+")
  show(res, MIME"text/plain"(), bret)

  nothing
end

function bm_multithread(data, d, f, diff_fn=__default_diff_fn)
  println("Running once to compile...")
  im2gr(data, d, IM2GR.CM_MultiThread, diff_fn)
  println("Running multithread benchmarks...", f)
  b = @benchmarkable im2gr($data, $d, IM2GR.CM_MultiThread, $diff_fn)
  bret = run(b)
  res = open(f, "w+")
  show(res, MIME"text/plain"(), bret)

  nothing
end

function bm_cuda(data, d, f, diff_fn=__default_diff_fn)
  println("Running once to compile...")
  im2gr(data, d, IM2GR.CM_CUDA, diff_fn)
  println("Running CUDA benchmarks...", f)
  b = @benchmarkable im2gr($data, $d, IM2GR.CM_CUDA, $diff_fn)
  bret = run(b)
  res = open(f, "w+")
  show(res, MIME"text/plain"(), bret)

  nothing
end


mri_diff_fn(xi, xj) = min(sqrt(xi) / 63, 1.0) - min(sqrt(xj) / 63, 1.0)
mri = load("data/lgemri.nrrd")

fake = rand(UInt8, (144, 144, 22))

#bm_singlethread(mri, 1, "benchmark/st-mri-unimodular-1.txt", mri_diff_fn)
#bm_singlethread(mri, 2, "benchmark/st-mri-unimodular-2.txt", mri_diff_fn)
#bm_singlethread(fake, 1, "benchmark/st-fake-razr-1.txt")
#bm_singlethread(fake, 2, "benchmark/st-fake-razr-2.txt")

#bm_multithread(mri, 1, "benchmark/384/mt-mri-unimodular-1.txt", mri_diff_fn)
#bm_multithread(mri, 2, "benchmark/384/mt-mri-unimodular-2.txt", mri_diff_fn)
#bm_multithread(fake, 1, "benchmark/mt-fake-razr-1.txt")
bm_multithread(fake, 2, "benchmark/mt-fake-razr-2.txt")

#bm_cuda(mri, 1, "benchmark/cuda-mri-unimodular.txt", mri_diff_fn)
#bm_cuda(fake, 1, "benchmark/cuda-fake-razr-1.txt")
#bm_cuda(fake, 2, "benchmark/cuda-fake-razr-2.txt")

