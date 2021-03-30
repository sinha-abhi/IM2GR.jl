# load functions

# ********** load image -- singlethread **********
function load_st!(
  image::AbstractImage{T},
  diff_fn::Function,
  track::Bool
) where T <: Unsigned
  iter = CartesianIndices(image.data)
  imap = LinearIndices(iter)

  idx_first, idx_last = first(iter), last(iter)
  dd = image.d * oneunit(idx_first)

  track && (p = Progress(length(iter)))
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
    end
    track && next!(p)
  end

  image.vc = _vc - 1

  nothing
end

# ********** load image -- multithread **********
function construct_graph!(
  ei::Vector{Int}, ej::Vector{Int},
  evd::Vector{Float64}, evi::Vector{Float64},
  data::AbstractArray{T}, diff_fn::Function,
  d, lo, hi, cf, cl 
) where T <: Unsigned
  dd = d * oneunit(cf)
  imap = LinearIndices(data)

  for idx in lo : hi 
    idx_lower = max(cf, idx - dd)
    idx_upper = min(cl, idx + dd)
    src = imap[idx]
    _pi = data[idx]
    for nidx in idx_lower : idx_upper
      dst = imap[nidx]
      (src == dst) && continue
      dist = norm(Tuple(idx - nidx))^2
      pj = data[nidx]

      push!(ei, src)
      push!(ej, dst)
      push!(evd, dist)
      push!(evi, diff_fn(_pi, pj))
    end
  end

  nothing 
end

function load_mt!(
  image::AbstractImage{T},
  diff_fn::Function,
  track::Bool
) where T <: Unsigned
  # https://julialang.org/blog/2019/07/multithreading/#thread-local_state
  data = image.data

  R = CartesianIndices(data)
  imap = LinearIndices(R)

  cf, cl = first(R), last(R)
  lf, ll = first(imap), last(imap)
  dd = image.d * oneunit(lf)

  nt = Threads.nthreads()

  step = ceil(typeof(lf), ll / nt)

  # allocate space for worker threads
  eis = [zeros(Int, 0) for i = 1:nt]
  ejs = [zeros(Int, 0) for i = 1:nt]
  evds = [zeros(0) for i = 1:nt]
  evis = [zeros(0) for i = 1:nt]
  @sync for t = 1 : nt
    start = lf + step * (t-1)
    stop = step + step * (t-1)
    start = start > ll ? ll : start 
    stop = stop > ll ? ll : stop

    # f = max(lf, start - dd)
    # l = min(ll, stop + dd)
    @spawn construct_graph!(
      eis[t], ejs[t], evds[t], evis[t],
      data, diff_fn, image.d, 
      R[start], R[stop], cf, cl
    )
  end

  image.ei = cat(eis..., dims=1)
  image.ej = cat(ejs..., dims=1)
  image.evd = cat(evds..., dims=1)
  image.evi = cat(evis..., dims=1)

  nothing
end

# ********** load image -- cuda **********
function load_cuda!(
  image::AbstractImage{T},
  diff_fn::Function,
  track::Bool
) where T <: Unsigned
  # TODO
  nothing
end
