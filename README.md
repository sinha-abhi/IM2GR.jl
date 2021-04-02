Image Graphs
============

An exploration of algorithms for constructing graphs derived from image-like
data in parallel. See our [research statment](docs/rs/research-statement.pdf).

# Examples

## Julia
```julia
using IM2GR

data= <data source>
diff_fn = <image diff func>
d = <search distance>
image = im2gr!(data, d, <construction mode>, diff_fn, track=true)
@show image.ei, image.ej, image.evd, image.evi
```
The result vectors are stored in `image`.

## C++
_Temporarily hidden: On the back burner till Julia implementation is finished._

