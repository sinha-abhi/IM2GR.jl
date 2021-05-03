IM2GR
=====

An exploration of algorithms for constructing graphs derived from image-like
data in parallel. See our [report](docs/im2gr.pdf) (my senior thesis).

### Examples
```julia
using IM2GR

data = <data source>
diff_fn = <image diff func>
d = <search distance>
image = im2gr(data, d, <construction mode>, diff_fn, track=true)
@show image.ei, image.ej, image.evd, image.evi
```
The result vectors are stored in `image`.

### Testing
Use `]` to enter Pkg REPL, and do:
```
pkg> add https://github.com/sinha-abhi/im2gr
pkg> test
```
or, do:
```julia
julia> import Pkg; Pkg.add("https://github.com/sinha-abhi/IM2GR.jl");
julia> Pkg.test("IM2GR)
```

### TODO
 - [ ] Option to construct graphs lazily
 - [ ] Distributed mode for construction (multi-process)
 - [ ] Better memory management for CUDA (actually, better memory management in
   general)
 - [ ] Use multiple GPUs if available
 - [ ] Tune CUDA kernel launch
 - [ ] Batching for larger images

