## Experiment on boundaries in images.

## Setup
using Images, LinearAlgebra, SparseArrays
_default_diff_fn(c1::CT1,c2::CT2) where {CT1<:Union{Colorant,Real}, CT2<:Union{Colorant,Real}} = sum(abs2,(c1)-Images.accum(CT2)(c2))
function image2graph(im::AbstractArray, r::Real, sigmap2::Real, sigmax2::Real)
  # see https://julialang.org/blog/2016/02/iteration/
  # for some of the syntax help (in the boxcar filter)
  d = ceil(Int,sqrt(r))
  R = CartesianIndices(im) # get the iterator of CartesianIndices for im
  imap = LinearIndices(R)  # converge CartesianIndices to Linear Indices for the graph
  Ifirst, Ilast = first(R), last(R) # get the image bounds
  Id = d*oneunit(Ifirst)
  # allocate sparse array inputs, we could create this directly, but is' more
  # error prone
  ei = zeros(Int, 0)
  ej = zeros(Int, 0)
  ev = zeros(0)
  for I in CartesianIndices(im)
    # find adjacent pixel neighbors at distance d, respecting bounds
    Ilower = max(Ifirst,I-Id)
    Iupper = min(Ilast,I+Id)
    src = imap[I]
    pi = im[I]
    for J in Ilower:Iupper
      dst = imap[J]
      if src == dst # skip self-loops
        continue
      end
      dx2 = norm(Tuple(I-J))^2
      if dx2 <= r
        pj = im[J]
        dp2 = _default_diff_fn(pi,pj)
        w = exp(-dp2/sigmap2-dx2/sigmax2)
        push!(ei, src)
        push!(ej, dst)
        push!(ev, w)
      end
    end
  end
  return sparse(ei,ej,ev,length(im),length(im)), imap
end


using Test
@testset "image2graph" begin
  using TestImages
  im = testimage("cameraman")
  @test_nowarn image2graph(im, 80, maximum(size(im))/10, 100)
end

include("PageRank.jl")
include("SLQ.jl")

##
using TestImages
@time A = testimage("house")
@time G,imap = image2graph(A, 40, 0.001, maximum(size(A))/10)
xymap = CartesianIndices(imap)
xy = [vec(map(x->x[1], xymap)) vec(map(x->x[2], xymap)) ]
##
using SparseArrays
function graphlines(A::SparseMatrixCSC,xy::AbstractArray{T,2}) where T
  px,py = zeros(T,0),zeros(T,0)
  P = [px,py]
  v = zeros(eltype(A),0)
  rows = rowvals(A)
  skip = NaN.*xy[:,begin] # first row
  for j=1:size(A,2) # for each column
    for nzi in nzrange(A, j)
      i = rows[nzi]
      if i > j
        push!.(P, @view xy[:,i])
        push!.(P, @view xy[:,j])
        push!.(P, skip)
        push!(v, A[i,j])
      end
    end
  end
  return px, py, v
end

##
heatmap(A, framestyle=:none, margin=-20mm, size=(400,400))
@time savefig("figures/boundary-image.png")
##
using Plots, Measures
px,py,v = graphlines(dropzeros!(G .>= 0.9),Float64.(xy'))
@time plot(py,px,alpha=0.25*v,line_z=v,linewidth=0.5, dpi=300, size=(600,600),
 yflip=true, framestyle=:none, background=:transparent, margin=-100mm,  color=:blues, legend=false)
@time savefig("figures/boundary-graph.png")
