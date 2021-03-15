#ifndef _CUDA_UTILS_H_
#define _CUDA_UTILS_H_

#include <cuda.h>

#define CHECK_CUDA_RESULT(N) {                                          \
    cudaError_t result = N;                                             \
    if (result != 0) {                                                  \
        printf("CUDA call on line %d returned error %d\n", __LINE__,    \
            result);                                                    \
        exit(1);                                                        \
    } }

#endif /* _CUDA_UTILS_H_ */
