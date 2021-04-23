using IM2GR

using LinearAlgebra
using Printf
using Test

using CUDA

@testset verbose=true "IM2GR" begin
  include("bounds.jl")
  include("construct.jl")
end
