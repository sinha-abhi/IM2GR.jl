module IM2GR

using FileIO
using Images
using LinearAlgebra
using OffsetArrays

using NRRD
using Printf
using ProgressMeter

import Base.Threads.@spawn

include("types.jl")
include("bounds.jl")
include("kernels.jl")
include("loader.jl")

include("api.jl")

export
  AbstractImage,
  Image,
  ConstructionMode,
  im2gr!

end # module
