# result vector bound functions

function fullbound(sz::Tuple, d::Int)::UInt
  x, y, z = sz

  n = (x-2) * (y-2) * (z-2)
  ub = n * ((2*d+1)^3 - 1)

  n = 2((x-2) * (y-2) + (x-2) * (z-2) + (y-2) * (z-2))
  ub += n * ((d+1) * (2*d+1)^2 - 1)

  n = 4(x+y) + 4z - 8
  ub += n * ((2d+1) * (d+1)^2 - 1)

  ub += 8((d+1)^3 - 1)

  ub
end

function partialbound(bsz::Tuple, dim::Int, d::Int, s::Sections)::UInt
  s == Middle && return middlebound(bsz, dim, d)
  s == Side && return sidebound(bsz, dim, d)

  nothing
end

function sidebound(bsz::Tuple, dim::Int, d::Int)::UInt
  # TODO
end

function middlebound(bsz::Tuple, dim::Int, d::Int)::UInt
  # TODO
end

