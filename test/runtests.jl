using IM2GR

using LinearAlgebra
using Printf
using Test

@testset verbose=true "IM2GR" begin
  include("bounds.jl")
  include("loader.jl")
end
