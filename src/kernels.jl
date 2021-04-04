# kernels for parallel construction

function mt_construct_kernel!(
  ei::Vector{Int}, ej::Vector{Int},
  evd::Vector{Float64}, evi::Vector{Float64},
  data::AbstractArray{<: Unsigned}, diff_fn::Function, dd::CartesianIndex,
  cf::CartesianIndex, cl::CartesianIndex,
  bstart::CartesianIndex, bstop::CartesianIndex, boffset::CartesianIndex,
  dstart::CartesianIndex, dstop::CartesianIndex
)
  imap = LinearIndices(data)

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

      push!(ei, src)
      push!(ej, dst)
      push!(evd, dist)
      push!(evi, diff_fn(_pi, pj))
    end
  end

  nothing 
end

