Image Graphs
============

An exploration of algorithms for constructing graphs derived from image-like
data in parallel. See our [research statment](docs/rs/research-statement.pdf).

# Examples

## Julia
```julia
using IM2GR

data = <data source>
diff_fn = <image diff func>
d = <search distance>
image = im2gr(data, d, <construction mode>, diff_fn, track=true)
@show image.ei, image.ej, image.evd, image.evi
```
The result vectors are stored in `image`.

## C++
### Executable
```bash
make im2gr-<mode> && ./bin/im2gr-<mode> <data path> <d>
```
Optional `OPT` and `DEBUG` flags can be set for compilation.
Try `make list` for all compilation options.

### API
```cpp
Data *data = new Data(<data path>);
int d = <d>;
diff_fn diff = <image diff function>;
Image image = im2gr(data, d, <construction mode>, diff);

Index *ei = image.get_ei();
Index *ei = image.get_ei();
float *evd = image.get_evd();
float *evi = image.get_evi();
size_t vc = image.get_vc();   // length of result vectors
```
Image diff functions must accept two `float` arguments and have a return type
of `float`.

# Testing

## Julia
Use `]` to enter Pkg REPL, and do:
```
pkg> add https://github.com/sinha-abhi/im2gr
pkg> test
```
or, do:
```julia
julia> import Pkg; Pkg.add("https://github.com/sinha-abhi/im2gr");
julia> Pkg.test("IM2GR)
```

## C++
```bash
make test && ./bin/im2gr-test --log_level=test_suite
```
C++ tests use the [Boost Unit Test Framework](https://github.com/boostorg/test).

