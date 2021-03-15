#ifndef _MRILOADER_H_
#define _MRILOADER_H_

#include <algorithm>
#include <cctype>
#include <cmath>

#include "index.h"
#include "utils.h"

#define MAX_MRI_X 600
#define MAX_MRI_Y 600
#define MAX_MRI_Z 100

class MRILoader {
public:
	MRILoader(const char *fname, int d);
	~MRILoader();
	int*     get_sz()  { return sz;  }
	unsigned get_vc()  { return vc;  }
	Index*   get_ei()  { return ei;  }
	Index*   get_ej()  { return ej;  }
	float*   get_evi() { return evi; }
	float*   get_evd() { return evd; }

	void im2gr();

private:
	uint8_t ***mri;
	int 	  *sz, d;
	unsigned   vc;
	Index     *ei, *ej;
	float     *evi, *evd;
};


#endif /* _MRILOADER_H_ */
