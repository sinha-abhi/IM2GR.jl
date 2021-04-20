module IM2GR

using FileIO
using Images

using AxisArrays
using LinearAlgebra
using OffsetArrays

using NRRD
using Printf
using ProgressMeter

using CUDA

const gpu_avail = Ref(false)
function __init__()
  gpu_avail[] = CUDA.functional()
  !gpu_avail[] && @info(
    "CUDA not available on this machine.\n" *
    "Calling im2gr with mode CM_CUDA will return nothing."
  )
  # CUDA.allowscalar(false)
end

import Base.Threads.@spawn

include("types.jl")
include("bounds.jl")
include("kernels.jl")
include("construct.jl")

include("api.jl")

export
  AbstractImage,
  Image,
  ImageCUDA,
  ConstructionMode,
  im2gr

end # module
