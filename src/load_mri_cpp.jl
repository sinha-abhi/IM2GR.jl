"""
Load the graph from Atrial MRI using C++.
"""

using Cxx
using FileIO
using Libdl
using NRRD
using ProgressMeter

const DATA_PATH = "data/"
const LIB_PATH = pwd() * "/cpp/"


function gen_file(arr::AbstractArray, fname::String)
  (X, Y, Z) = size(arr)
  open(fname, "w+") do io
    print(io, X, ' ', Y, ' ', Z, "\n")
    @showprogress for z in Base.OneTo(Z)
      mat = collect(Iterators.flatten(arr[:, :, z]))
      for e in mat
        print(io, e, ' ')
      end
      print(io, "\n")
    end
  end
end

function image2graph_cpp(
  im::AbstractArray,
  d::Int
)
  R = CartesianIndices(im)
  imap = LinearIndices(R)

  gen_file(mri, DATA_PATH * "lgemri.txt")

  addHeaderDir(LIB_PATH, kind=C_System)
  Libdl.dlopen(joinpath(LIB_PATH, "load_mri.so"), Libdl.RTLD_GLOBAL)
  cxxinclude("include/MRILoader.h")
  cxxinclude("string")

  loader = @cxxnew MRILoader(pointer("data/lgemri.txt"))

  @cxx loader->im2gr(d)

  vc  = @cxx loader->get_vc()
  ei  = @cxx loader->get_ei()
  ej  = @cxx loader->get_ei()
  evd = @cxx loader->get_evd()
  evi = @cxx loader->get_evi()

  ei_jl  = unsafe_wrap(Array, ei, vc)
  ei_jl  = unsafe_wrap(Array, ei, vc)

  # FIXME: this is broken
  evd_jl = unsafe_wrap(Array, evd, vc)
  evi_jl = unsafe_wrap(Array, evi, vc)

  return ei_jl, ej_jl, evd_jl, evi_jl, R, imap
end

image2graph_cpp(mri, 1)
