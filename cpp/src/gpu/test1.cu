//#include <iostream>
//
//#include "cuda_utils.h"
//
//__global__ void saxpy(int n, float a, float *x, float *y)
//{
//    int i = blockIdx.x * blockDim.x + threadIdx.x;
//
//    if (i < n)
//        y[i] = a * x[i] + y[i];
//}
//
//int main()
//{
//    int N = 1 << 20;
//    float *x, *y, *dx, *dy;
//
//    x = (float *) malloc(N * sizeof(float));
//    y = (float *) malloc(N * sizeof(float));
//
//    CHECK_CUDA_RESULT( cudaMalloc(&dx, N * sizeof(float)) );
//    CHECK_CUDA_RESULT( cudaMalloc(&dy, N * sizeof(float)) );
//
//    for (int i = 0; i < N; i++) {
//        x[i] = 1.0f;
//        y[i] = 2.0f;
//    }
//
//    CHECK_CUDA_RESULT( cudaMemcpy(dx, x, N * sizeof(float), cudaMemcpyHostToDevice) );
//    CHECK_CUDA_RESULT( cudaMemcpy(dy, y, N * sizeof(float), cudaMemcpyHostToDevice) );
//
//    saxpy<<<(N + 255) / 256, 256>>>(N, 2.0f, dx, dy);
//
//    CHECK_CUDA_RESULT( cudaMemcpy(y, dy, N * sizeof(float), cudaMemcpyDeviceToHost) );
//
//    float maxError = 0.0f;
//    for (int i = 0; i < N; i++)
//        maxError = max(maxError, abs(y[i] - 4.0f));
//
//    printf("Max error: %f\n", maxError);
//
//    CHECK_CUDA_RESULT( cudaFree(dx) );
//    CHECK_CUDA_RESULT( cudaFree(dy) );
//    free(x);
//    free(y);
//}
