# load functions

# ********** load image -- singlethread **********
function st_construct(
  data::AbstractArray{<: Unsigned}, d::Int,
  diff_fn::Function,
  track::Bool
)
  R = CartesianIndices(data)
  imap = LinearIndices(R)

  cf, cl = first(R), last(R)
  dd = d * oneunit(cf)

  # allocate memory
  ub = fullbound(size(data), d)
  ei = Vector{Int}(undef, ub)
  ej = Vector{Int}(undef, ub)
  evd = Vector{Float64}(undef, ub)
  evi = Vector{Float64}(undef, ub)

  track && (p = Progress(length(R)))
  vc = 1
  for idx in R
    idx_lower = max(cf, idx-dd)
    idx_upper = min(cl, idx+dd)
    src = imap[idx]
    _pi = data[idx]
    for nidx in idx_lower : idx_upper
      dst = imap[nidx]
      (src == dst) && continue
      dist = norm(Tuple(idx - nidx))^2
      pj = data[nidx]

      ei[vc] = src
      ej[vc] = dst
      evd[vc] = dist
      evi[vc] = diff_fn(_pi, pj)

      vc += 1
    end
    track && next!(p)
  end

  # fix vector sizes
  vc -= 1
  resize!(ei, vc)
  resize!(ej, vc)
  resize!(evd, vc)
  resize!(evi, vc)

  Image{eltype(data)}(d, ei, ej, evd, evi)
end

# ********** load image -- multithread **********
function mt_construct(
  data::AbstractArray{<: Unsigned}, d::Int,
  diff_fn::Function,
  track::Bool
)
  _data = data isa AxisArray ? data.data : data
  nt = Threads.nthreads()
  R = CartesianIndices(_data)
  cf, cl = first(R), last(R)
  dd = d * oneunit(cf)

  # make as large blocks as we can along the longest dimension
  sz = size(_data)
  ml, ax = findmax(sz)
  bl = ml < nt ? ml : ceil(Int, ml / nt)
  nb = ceil(Int, ml / bl)
  # nb = Int(ml / bl)

  vcs = Vector{Int}(undef, nb)
  bstarts, bstops, dstarts, dstops, eis, ejs, evds, evis = mt_init(
    sz, nb, bl, ax, dd, d, cf, cl
  )
  @sync for b in 1 : nb
    @spawn mt_construct_kernel!(
      eis[b], ejs[b], evds[b], evis[b],
      OffsetArray(
        _data[dstarts[b]:dstops[b]],
        dstarts[b][1]:dstops[b][1],
        dstarts[b][2]:dstops[b][2],
        dstarts[b][3]:dstops[b][3]
      ),
      diff_fn, dd, cf, cl,
      bstarts[b], bstops[b],
      dstarts[b], dstops[b], vcs[b]
    )
  end

  ei = cat(eis..., dims=1)
  ej = cat(ejs..., dims=1)
  evd = cat(evds..., dims=1)
  evi = cat(evis..., dims=1)

  Image{eltype(data)}(d, ei, ej, evd, evi)
end

function mt_init(
  sz::Tuple, nb::Int, bl::Int, ax::Int, 
  dd::CartesianIndex, d::Int, cf::CartesianIndex, cl::CartesianIndex
)
  bstarts = Vector{CartesianIndex}(undef, nb)
  bstops = Vector{CartesianIndex}(undef, nb)
  dstarts = Vector{CartesianIndex}(undef, nb)
  dstops = Vector{CartesianIndex}(undef, nb)

  # allocate memory for thread-local vectors
  # https://julialang.org/blog/2019/07/multithreading/#thread-local_state
  sb = partialbound(sz, bl, ax, d, Side)
  mb = partialbound(sz, bl, ax, d, Middle)
  eis = Vector{Vector{Int}}(undef, nb)
  ejs = Vector{Vector{Int}}(undef, nb)
  evds = Vector{Vector{Float64}}(undef, nb)
  evis = Vector{Vector{Float64}}(undef, nb)
  for b in 1 : nb
    start = 1 + bl * (b-1)
    stop = bl + bl * (b-1)
    stop = stop > cl[ax] ? cl[ax] : stop
    bstart = Vector{Int}(undef, 3)
    bstop = Vector{Int}(undef, 3)
    for a in 1 : 3
      bstart[a] = ax == a ? start : 1
      bstop[a] = ax == a ? stop : cl[a]
    end
    
    bstarts[b] = CartesianIndex(bstart...)
    bstops[b] = CartesianIndex(bstop...)
    dstarts[b] = max(cf, bstarts[b] - dd)
    dstops[b] = min(cl, bstops[b] + dd)
    

    if b == 1 || b == nb
      eis[b] = Vector{Int}(undef, sb)
      ejs[b] = Vector{Int}(undef, sb)
      evds[b] = Vector{Float64}(undef, sb)
      evis[b] = Vector{Float64}(undef, sb)
    else
      eis[b] = Vector{Int}(undef, mb)
      ejs[b] = Vector{Int}(undef, mb)
      evds[b] = Vector{Float64}(undef, mb)
      evis[b] = Vector{Float64}(undef, mb)
    end
  end
  
  bstarts, bstops, dstarts, dstops, eis, ejs, evds, evis
end

# ********** load image -- cuda **********
function cuda_construct(
  data::AbstractArray{<: Unsigned}, d::Int,
  diff_fn::Function,
  track::Bool
)
  nothing
end

