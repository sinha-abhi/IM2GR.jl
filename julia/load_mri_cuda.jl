using CUDA
using FileIO
using LinearAlgebra
using NRRD

mri = load("lgemri.nrrd")

mri_diff_fn(xi::Real, xj::Real) = min(sqrt(xi)/63, 1.0) - min(sqrt(xj)/63, 1.0)

function gpu_mri_diff(xi::Real, xj::Real)
    return (
        CUDA.min(CUDA.div_fast(CUDA.sqrt(xi), 63.0), 1.0) -
        CUDA.min(CUDA.div_fast(CUDA.sqrt(xj), 63.0), 1.0)
    )
end

function gpu_nghbors!(
    ei, ej, evd, evi,
    image, lidx, d,
    idx_first, idx_last, dfn
)
    idx_x = (blockIdx().x - 1) * blockDim().x + threadIdx().x
    idx_y = (blockIdx().y - 1) * blockDim().y + threadIdx().y
    idx_z = (blockIdx().z - 1) * blockDim().z + threadIdx().z

    if idx_x <= idx_last[1] && idx_y <= idx_last[2] && idx_z <= idx_last[3]
        # FIXME: CUDA.max isn't defined for tuples
        # low = CUDA.max(first, i - d)
        # up  = CUDA.min(last, i + d)
        @cuprintln("x $idx_x, y $idx_y, z $idx_z")
    end

    nothing
end

function im2gr(
    image::AbstractArray,
    d::Int,
    dfn::Function
)
    cidx = CartesianIndices(image)
    lidx = LinearIndices(cidx)

    idx_first, idx_last = first(cidx), last(cidx)
    idx_d = d * oneunit(idx_first)

    cuda_img = CuArray(image)

    # FIXME: assuming d = 1
    # A pixel can have a max of 26 neighbors that are one pixel away
    upb = idx_last[1] * idx_last[2] * idx_last[3] * 26
    ei  = CUDA.zeros(Int, 189775872)
    ej  = CUDA.zeros(Int, 189775872)
    evd = CUDA.zeros(189775872) # Float32
    evi = CUDA.zeros(189775872) # Float32

    dimblock = (8, 8, 8)
    dimgrid = (
        ceil(Int, idx_last[1] / 8),
        ceil(Int, idx_last[2] / 8),
        ceil(Int, idx_last[3] / 8)
    )
    # TODO: kernel call
    @cuda threads=dimblock blocks=dimgrid gpu_nghbors!(
                                              ei, ej, evd, evi,
                                              cuda_img, lidx, d,
                                              idx_first, idx_last, dfn
                                          )

    return ei, ej, evd, evi, lidx, cidx
end

@time ei, ej, evd, evi, lidx, cidx = im2gr(mri, 1, gpu_mri_diff)
