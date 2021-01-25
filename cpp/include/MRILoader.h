#ifndef _MRILOADER_H_
#define _MRILOADER_H_

#include <algorithm>
#include <cctype>
#include <cmath>

#include "index.h"
#include "utils.h"

#define MAX_MRI_X 576
#define MAX_MRI_Y 576
#define MAX_MRI_Z 88

#define MAX_VEC_SZ (751321624 + 1)

// TODO: pass function ptr from Julia
//typedef float (*diff_func_call)(float x, float y);

class MRILoader {
public:
	MRILoader(const char *fname);
	~MRILoader();
	int*     get_sz()  { return sz; }
	unsigned get_vc()  { return vc; }
	Index*   get_ei()  { return ei; }
	Index*   get_ej()  { return ej; }
	float*   get_evi() { return evi; }
	float*   get_evd() { return evd; }

	void im2gr(int d);

private:
	uint8_t ***mri;
	int *sz;
	unsigned vc;
	Index *ei, *ej;
	float *evi, *evd;
};


#endif /* _MRILOADER_H_ */
