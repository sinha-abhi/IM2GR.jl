# result vector bound functions

function fullbound(sz::Tuple, d::Int)::UInt
  x, y, z = sz

  n = (x-2) * (y-2) * (z-2)
  ub = n * ((2*d+1)^3 - 1)

  n = 2((x-2) * (y-2) + (x-2) * (z-2) + (y-2) * (z-2))
  ub += n * ((d+1) * (2*d+1)^2 - 1)

  n = 4(x+y+z) - 8
  ub += n * ((2d+1) * (d+1)^2 - 1)

  ub += 8((d+1)^3 - 1)
end

function partialbound(sz::Tuple, bl::Int, ax::Int, d::Int, s::Sections)::UInt
  bsz = [sz...]
  bsz[ax] = bl
  bsz = Tuple(bsz)
  psz = Tuple(bsz[i] for i = 1:3 if i != ax)

  fb = fullbound(bsz, d)
  pb = planebound(psz, d)
  s == Middle && return fb + 2*pb
  s == Side && return fb + pb

  nothing
end

# count points underneath the surface p
function planebound(psz::Tuple, d::Int)::UInt
  x, y = psz

  n = (x-2) * (y-2)
  ub = n * d * (2d+1)^2

  n = 2(x+y) - 4
  ub += n * d * (d+1) * (2d+1)

  ub += 4d * (d+1)^2
end

