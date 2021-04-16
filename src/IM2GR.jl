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

import Base.Threads.@spawn

include("types.jl")
include("bounds.jl")
include("kernels.jl")
include("construct.jl")

include("api.jl")

export
  AbstractImage,
  Image,
  ConstructionMode,
  im2gr

end # module
