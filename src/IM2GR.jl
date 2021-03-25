module IM2GR

using FileIO
using Images
using LinearAlgebra
using NRRD

using Printf
using ProgressMeter

include("types.jl")
include("utils.jl")
include("loader.jl")

include("api.jl")

export
  AbstractImage,
  Image,
  ConstructionMode,

  im2gr!,
  update_image!

end # module
