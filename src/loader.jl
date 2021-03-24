# load functions

# load image -- single thread
function load_st!(
  image::AbstractImage{T},
  track::Bool,
  diff_fun::Function
) where T <: Unsigned
  iter = CartesianIndices(image)
  imap = LinearIndices(iter)

  idx_first, idx_last = first(iter), last(iter)
  dd = image.d * oneunit(idx_first)

  track && (p = Progress(length(iter), 1))
  image.vc = 0
  for idx in iter
    idx_lower = max(idx_first, idx - dd)
    idx_upper = min(idx_last, idx + dd)
    src = imap[idx]
    _pi = image.data[I]
    for nidx in idx_lower : idx_upper
      dst = imap[nidx]
      (src == dst) && continue
      dist = norm(Tuple(idx - nidx))^2
      pj = image.data[nidx]

      ei[vc] = src
      ej[vc] = dst
      evd[vc] = dist
      evi[vc] = diff_fn(_pi, pj)

      vc += 1
      track && next!(p)
    end
  end

  nothing
end

# load image -- multithread
function load_mt!(
  image::AbstractImage{T},
  diff_fn::Function = __default_diff_fn,
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
