# kernels for parallel construction

function mt_construct_kernel!(
  ei::Vector{Int}, ej::Vector{Int},
  evd::Vector{Float32}, evi::Vector{Float32},
  data::AbstractArray{<: Unsigned}, diff_fn::Function, dd::CartesianIndex,
  bstart::CartesianIndex, bstop::CartesianIndex,
  dstart::CartesianIndex, dstop::CartesianIndex, vc::Int
)
  imap = LinearIndices(data)

  vc = 1
  for idx in bstart : bstop
    idx_low = max(dstart, idx-dd)
    idx_up = min(dstop, idx+dd)
    src = imap[idx]
    _pi = data[idx]
    for nidx in idx_low : idx_up
      dst = imap[nidx]
      (src == dst) && continue
      dist = norm(Tuple(idx-nidx))^2
      pj = data[nidx]

      ei[vc] = src
      ej[vc] = dst
      evd[vc] = dist
      evi[vc] = diff_fn(_pi, pj)

      vc += 1
    end
  end

  # fix vector sizes
  vc -= 1
  resize!(ei, vc)
  resize!(ej, vc)
  resize!(evd, vc)
  resize!(evi, vc)

  nothing 
end

function cuda_construct_kernel!(V, data, R, cf, cl, dd, imap, jmap, diff_fn, _one)
  I = CartesianIndex(
    (blockIdx().z-1) * blockDim().z + threadIdx().z,
    (blockIdx().y-1) * blockDim().y + threadIdx().y,
    (blockIdx().x-1) * blockDim().x + threadIdx().x
  )

  if I in R
    lower = Base.max(cf, I-dd)
    upper = Base.min(cl, I+dd)
    _pi = data[I]
    src = imap[I]
    for J in lower : upper
      dst = imap[J]
      src == dst && continue
      pj = data[J]
      noffset = jmap[J-lower+_one]
      V[noffset, src] = diff_fn(_pi, pj)
    end
  end

  nothing
end

