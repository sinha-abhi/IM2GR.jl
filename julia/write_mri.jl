using FileIO
using NRRD
using ProgressMeter

mri = load("lgemri.nrrd")

mri[101, 2, 1]
mri[101, 1, 1]

function gen_file(arr::AbstractArray)
    (X, Y, Z) = size(arr)
    open("lgemri.txt", "w+") do io
        print(io, X, ' ', Y, ' ', Z, "\n")
        @showprogress for z in Base.OneTo(Z)
            mat = collect(Iterators.flatten(arr[:, :, z]))
            for e in mat
                print(io, e, ' ')
            end
            print(io, "\n")
        end
    end;
end

gen_file(mri)
