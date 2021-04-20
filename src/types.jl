# types for im2gr

# ********** images **********
abstract type AbstractImage end

struct Image <: AbstractImage
  d::Int

  # result vectors
  ei::Vector{Int}
  ej::Vector{Int}
  evd::Vector{Float32}
  evi::Vector{Float32}
end

struct ImageCUDA <: AbstractImage
  d::Int

  # weighted adjacency matrix
  V::AbstractArray{Float32}
end

@enum ConstructionMode begin
  CM_SingleThread
  CM_MultiThread
  CM_CUDA
end

@enum Sections begin
  Middle
  Side
end

# ********** diff functions **********
function __default_diff_fn(
  c1::CT1, c2::CT2
) where {CT1 <: Union{Colorant, Real}, CT2 <: Union{Colorant, Real}}
  sum(abs2, (c1) - Images.accum(CT2)(c2))
end

