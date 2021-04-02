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
### Executable
```bash
make im2gr-<mode> && ./bin/im2gr-<mode> <data path> <d>
```
Optional `OPT` and `DEBUG` flags can be set for compilation.
Try `make list` for all compilation options.

### API
```cpp
std::string path(<data path>);
int d = <d>;
diff_fn dfn = <image diff function>;
Loader<short> loader(path, d);
loader.im2gr(dfn);

Index<short> *ei = loader.get_ei();
Index<short> *ei = loader.get_ei();
float *evd = loader.get_evd();
float *evi = loader.get_evi();
```
Image diff functions must accept two `float` arguments and have a return type
of `float`.

