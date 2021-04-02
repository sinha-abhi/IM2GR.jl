# types for im2gr

# ********** images **********
abstract type AbstractImage{T <: Unsigned} end

struct Image{T <: Unsigned} <: AbstractImage{T}
  data::AbstractArray{T}
  d::Int

  # result vectors
  ei::Vector{Int}
  ej::Vector{Int}
  evd::Vector{Float64}
  evi::Vector{Float64}
end


@enum ConstructionMode begin
  SingleThread
  MultiThread
  CUDA
end

# ********** diff functions **********
function __default_diff_fn(
  c1::CT1, c2::CT2
) where {CT1 <: Union{Colorant, Real}, CT2 <: Union{Colorant, Real}}
  sum(abs2, (c1) - Images.accum(CT2)(c2))
end
