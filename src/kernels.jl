# kernels for parallel construction

function mt_construct_kernel!(
  ei::Vector{Int}, ej::Vector{Int},
  evd::Vector{Float64}, evi::Vector{Float64},
  data::AbstractArray{<: Unsigned}, diff_fn::Function, dd::CartesianIndex,
  cf::CartesianIndex, cl::CartesianIndex,
  bstart::CartesianIndex, bstop::CartesianIndex, boffset::CartesianIndex,
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

