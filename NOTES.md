Notes
=====

(Tight-ish?) Upperbound on neighbor count
----------------------------
Upper bound for a voxel in/on the 
- Center of the image: `(2d + 1)^3 - 1`
    - Count: `(x - 2) * (y - 2) * (z - 2)`
- Side of the image: `(d + 1)(2d + 1)^2 - 1`
    - Count: `2 * ((x - 2) * (y - 2) + (x - 2) * (z - 2) + (y - 2) * (z - 2))`
- Edge of the image: `(2d + 1)(d + 1)^2 - 1`
    - Count (ignore corners): `4 * (x + y) + 4 * z - 8` 
- Corner of the image: `(d + 1)^3 - 1`
    - Count: 8

We can make this bound tighter (when `d > 1` -- tight as possible for `d = 1`), 
but I'm not sure that it's worth it (yet). Bound can be improved by considering
the voxels `v` that are less than `d` away from the side of the image. 
