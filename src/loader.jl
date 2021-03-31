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
  d, cf, cl, bstart, bstop
) where T <: Unsigned
  imap = LinearIndices(data)

  for idx in bstart : bstop
    idx_low = max(cf, idx-d)
    idx_up = min(cl, idx+d)
    src = imap[idx]
    pi = data[idx]
    for nidx in idx_low : idx_up
        dst = imap[nidx]
        (src == dst) && continue
        dist = norm(Tuple(idx - nidx))^2
        pj = data[nidx]

        push!(ei, src)
        push!(ej, dst)
        push!(evd, dist)
        push!(evi, diff_fn(pi, pj))
    end
  end
  nothing 
end

function load_mt!(
  image::AbstractImage{T},
  diff_fn::Function,
  track::Bool
) where T <: Unsigned
  data = image.data

  nt = Threads.nthreads()
  R = CartesianIndices(data)
  cf, cl = first(R), last(R)
  dd = image.d * oneunit(cf)

  # make as large blocks as we can along the longest dimension
  sz, dim = findmax(size(data))
  bsz = sz < nt ? 1 : ceil(Int, sz / nt)
  nb = Int(sz / bsz)

  # allocate memory for threads
  # https://julialang.org/blog/2019/07/multithreading/#thread-local_state
  eis = Vector{Vector{Int}}(undef, nb)
  ejs = Vector{Vector{Int}}(undef, nb)
  evds = Vector{Vector{Float64}}(undef, nb)
  evis = Vector{Vector{Float64}}(undef, nb)
  for i in 1 : nb
    eis[i] = Int[]
    ejs[i] = Int[]
    evds[i] = Float64[]
    evis[i] = Float64[]
  end

  @sync for block in 1 : nb
    start = 1 + bsz * (block-1)
    stop = bsz + bsz * (block-1)
    stop = stop > cl[dim] ? cl[dim] : stop
    bstart = Vector{Int}(undef, 3)
    bstop = Vector{Int}(undef, 3)
    for i in 1 : 3
      bstart[i] = dim == i ? start : 1
      bstop[i] = dim == i ? stop : cl[i]
    end
    bstart = CartesianIndex(Tuple(bstart))
    bstop = CartesianIndex(Tuple(bstop))
    @spawn construct_graph!(
      eis[block], ejs[block], evds[block], evis[block],
      data, diff_fn, dd, cf, cl, bstart, bstop
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
