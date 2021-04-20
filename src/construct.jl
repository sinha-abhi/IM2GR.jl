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
  evd = Vector{Float32}(undef, ub)
  evi = Vector{Float32}(undef, ub)

  track && (p = Progress(length(R)))
  vc = 1
  for I in R
    lower = max(cf, I-dd)
    upper = min(cl, I+dd)
    src = imap[I]
    _pi = data[I]
    for J in lower : upper
      dst = imap[J]
      (src == dst) && continue
      dist = norm(Tuple(I - J))^2
      pj = data[J]

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

  Image(d, ei, ej, evd, evi)
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
      diff_fn, dd,
      bstarts[b], bstops[b],
      dstarts[b], dstops[b], vcs[b]
    )
  end

  ei = cat(eis..., dims=1)
  ej = cat(ejs..., dims=1)
  evd = cat(evds..., dims=1)
  evi = cat(evis..., dims=1)

  Image(d, ei, ej, evd, evi)
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
  sb = partialbound(sz, bl, ax, d, Side)
  mb = partialbound(sz, bl, ax, d, Middle)
  eis = Vector{Vector{Int}}(undef, nb)
  ejs = Vector{Vector{Int}}(undef, nb)
  evds = Vector{Vector{Float32}}(undef, nb)
  evis = Vector{Vector{Float32}}(undef, nb)
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
      evds[b] = Vector{Float32}(undef, sb)
      evis[b] = Vector{Float32}(undef, sb)
    else
      eis[b] = Vector{Int}(undef, mb)
      ejs[b] = Vector{Int}(undef, mb)
      evds[b] = Vector{Float32}(undef, mb)
      evis[b] = Vector{Float32}(undef, mb)
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
  data_d = CuArray(data)
  R = CartesianIndices(data)
  imap = LinearIndices(R)
  cf, cl = first(R), last(R)
  sz = size(data)

  _one = oneunit(cf)
  dd = d * _one
  bm = 3d * _one
  jmap = LinearIndices(dd:bm)

  n = length(imap)
  V_d = CuArray(fill(NaN32, (2d+1)^ndims(data), n))

  threads = (8, 8, 8)
  blocks = cld.(sz, threads)
  @cuda blocks=blocks threads=threads cuda_construct_kernel!(
    V_d, data_d, R, cf, cl, dd, imap, jmap, diff_fn, _one
  )

  ImageCUDA(d, Array(V_d))
end
