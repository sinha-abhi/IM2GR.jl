# load functions

# load image -- single thread
function load_st!(
  image::AbstractImage{T},
  diff_fn::Function,
  track::Bool
) where T <: Unsigned
  iter = CartesianIndices(image.data)
  imap = LinearIndices(iter)

  idx_first, idx_last = first(iter), last(iter)
  dd = image.d * oneunit(idx_first)

  track && (p = Progress(length(iter), 1))
  _vc = 1
  for idx in iter
    idx_lower = max(idx_first, idx - dd)
    idx_upper = min(idx_last, idx + dd)
    src = imap[idx]
    _pi = image.data[idx]
    for nidx in idx_lower : idx_upper
      dst = imap[nidx]
      (src == dst) && continue
      dist = norm(Tuple(idx - nidx))^2
      pj = image.data[nidx]

      image.ei[_vc] = src
      image.ej[_vc] = dst
      image.evd[_vc] = dist
      image.evi[_vc] = diff_fn(_pi, pj)

      _vc += 1
      track && next!(p)
    end
  end

  image.vc = _vc - 1

  nothing
end

# load image -- multithread
function load_mt!(
  image::AbstractImage{T},
  diff_fn::Function,
  track::Bool
) where T <: Unsigned
  # TODO
  nothing
end

# load image -- cuda
function load_cuda!(
  image::AbstractImage{T},
  diff_fn::Function,
  track::Bool
) where T <: Unsigned
  # TODO
  nothing
end
