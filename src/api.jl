# api functions

# probably don't use `track` when benchmarking
function im2gr!(
  data::AbstractArray{ <: Unsigned}, d::Int,
  mode::ConstructionMode,
  diff_fn::Function = __default_diff_fn;
  track::Bool = false
)
  dims = ndims(data)
  dims == 3 || throw(DimensionMismatch(
    "expected 3 dimensions, but only got $dims"
  ))

  if track
    sz = size(data)
    @printf("Image size: %d, %d, %d\n", sz[1], sz[2], sz[3])
    @printf("Search distance: %d\n", d)
    @printf("Constructing graph... %s\n", mode)
  end

  (mode == SingleThread) && return st_construct(data, d, diff_fn, track)
  (mode == MultiThread) && return mt_construct(data, d, diff_fn, track)
  (mode == CUDA) && return cuda_construct(data, d, diff_fn, track)

  nothing
end

