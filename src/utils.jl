# util functions

# calculate upper bound for result vectors
function graph_vector_ub(dims::Tuple, d::Int)::UInt
  sz = length(dims)
  sz == 3 || throw(DimensionMismatch(
    "expected 3 dimensions, but only got $sz"
  ))

  x = dims[1]
  y = dims[2]
  z = dims[3]

  n = (x - 2) * (y - 2) * (z - 2)
  ub = n * ((2 * d + 1)^3 - 1)

  n = 2 * ((x - 2) * (y - 2) + (x - 2) * (z - 2) + (y - 2) * (z - 2))
  ub += n * ((d + 1) * (2 * d + 1)^2 - 1)

  n = 4 * (x + y) + 4 * z - 8
  ub += n * ((2 * d + 1) * (d + 1)^2 - 1)

  ub += 8 * ((d + 1)^3 - 1)

  ub
end

