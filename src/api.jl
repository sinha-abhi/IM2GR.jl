# api functions

# probably don't use `track` when benchmarking
function im2gr!(
  data::AbstractArray{T}, d::Int,
  mode::ConstructionMode,
  diff_fn::Function = __default_diff_fn;
  track::Bool = false
) where T <: Unsigned
  if track
    sz = size(data)
    @printf("Image size: %d, %d, %d\n", sz[1], sz[2], sz[3])
    @printf("Search distance: %d\n", d)
    @printf("Constructing graph... %s\n", mode)
  end

  (mode == SingleThread) && return build_st(data, d, diff_fn, track)
  (mode == MultiThread) && return build_mt(data, d, diff_fn, track)
  (mode == CUDA) && return build_cuda(data, d, diff_fn, track)

  nothing
end

