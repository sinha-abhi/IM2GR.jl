# types for im2gr

# ********** images **********
abstract type AbstractImage{T <: Unsigned} end

mutable struct Image{T <: Unsigned} <: AbstractImage{T}
  data::AbstractArray{T}
  d::Int
  vc::UInt # vector count
  ub::UInt # result vector upperbound

  # result vectors
  ei::Vector{Int}
  ej::Vector{Int}
  evd::Vector{Float64}
  evi::Vector{Float64}

  function Image{T}(data::AbstractArray{T}, d::Int) where T <: Unsigned
    image = new{T}()

    image.data = data
    image.d = d
    image.vc = 0

    ub = graph_vector_ub(size(image.data), d)
    image.ub = ub
    image.ei = Vector{Int}(undef, ub)
    image.ej = Vector{Int}(undef, ub)
    image.evd = Vector{Int}(undef, ub)
    image.evi = Vector{Int}(undef, ub)

    image
  end
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
