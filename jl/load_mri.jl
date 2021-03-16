using FileIO
using Images
using LinearAlgebra
using NRRD
using ProgressMeter

println("loaded packages")

const DATA_PATH = "../data/"
mri = load(DATA_PATH * "lgemri.nrrd")

println("loaded image")

_default_diff_fn(c1::CT1, c2::CT2) where { CT1 <: Union{Colorant, Real}, CT2 <: Union{Colorant, Real} } = sum(abs2, (c1) - Images.accum(CT2)(c2))

function image2graph(
  im::AbstractArray,
  d::Int,
  diff_fn::Function = _default_diff_fn
)
  # see https://julialang.org/blog/2016/02/iteration/
  # for some of the syntax help (in the boxcar filter)
  R = CartesianIndices(im) # get the iterator of CartesianIndices for im
  imap = LinearIndices(R)  # converge CartesianIndices to Linear Indices for the graph
  Ifirst, Ilast = first(R), last(R) # get the image bounds
  Id = d * oneunit(Ifirst)
  # allocate sparse array inputs, we could create this directly, but is' more
  # error prone
  ei = zeros(Int, 0)
  ej = zeros(Int, 0)
  evd = zeros(0)
  evi = zeros(0)

  @showprogress for I in R
    # find adjacent pixel neighbors at distance d, respecting bounds
    Ilower = max(Ifirst, I - Id)
    Iupper = min(Ilast, I + Id)
    src = imap[I]
    pi = im[I]
    for J in Ilower : Iupper
      dst = imap[J]
      if src == dst # skip self-loops
        continue
      end

      dist = norm(Tuple(I - J))^2
      pj = im[J]

      push!(ei, src)
      push!(ej, dst)
      push!(evd, dist)
      push!(evi, diff_fn(pi, pj))
    end
  end

  return ei, ej, evd, evi, imap, R
end

mri_diff_fun(xi::Real, xj::Real) = min(sqrt(xi)/63,1.0)-min(sqrt(xj)/63,1.0)

println("calling function...") 

ei, ej, evd, evi, imap, R = image2graph(mri, 2, mri_diff_fun)

println("length of ei", length(ei))
