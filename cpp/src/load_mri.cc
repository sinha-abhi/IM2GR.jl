//#include <iostream>
//
//#include "index.h"
//#include "utils.h"
//#include "load_mri.h"
//
//#define DEBUG 0
//
//#define MRI_FILE "../data/lgemri.txt"
//
//#define MAX_MRI_X 576
//#define MAX_MRI_Y 576
//#define MAX_MRI_Z 88
//
//#define MAX_VEC_SZ (751321624 + 1)
//
//int main() {
//    int *mri_sz = new int[3];
//    std::vector<std::vector<unsigned char>> flat_mri(parse_file<unsigned char>(mri_sz, MRI_FILE));
//
//    std::cout << "Parsed file... " << flat_mri.size() << std::endl;
//
//    if (mri_sz[0] > MAX_MRI_X || mri_sz[1] > MAX_MRI_Y || mri_sz[2] > MAX_MRI_Z) {
//        std::cerr << "MRI dimensions exceed limit:" << std::endl;
//        std::cerr << "\t x = " << mri_sz[0] << ", max = " << MAX_MRI_X << std::endl;
//        std::cerr << "\t y = " << mri_sz[1] << ", max = " << MAX_MRI_Y << std::endl;
//        std::cerr << "\t z = " << mri_sz[2] << ", max = " << MAX_MRI_Z << std::endl;
//
//        return -1;
//    }
//
//    // allocate memory for the mri image
//    unsigned char ***mri = new unsigned char**[MAX_MRI_X];
//    for (int i = 0; i < MAX_MRI_X; ++i) {
//        mri[i] = new unsigned char*[MAX_MRI_Y];
//        for (int j = 0; j < MAX_MRI_Y; ++j)
//            mri[i][j] = new unsigned char[MAX_MRI_Z];
//    }
//
//    unflatten<unsigned char>(mri, mri_sz, flat_mri);
//
//    std::cout << "Unflattened vector. Processing image..." << std::endl;
//
//    Index *mri_ei = new Index[MAX_VEC_SZ];
//    Index *mri_ej = new Index[MAX_VEC_SZ];
//    float *mri_evi = new float[MAX_VEC_SZ];
//    float *mri_evd = new float[MAX_VEC_SZ];
//    im2gr<unsigned char>(mri_ei, mri_ej, mri_evi, mri_evd, mri, mri_sz, 1, mri_diff_func);
//
//    std::cout << "Done." << std::endl;
//
//    // free memory
//    delete[] mri_sz;
//
//    for (int i = 0; i < MAX_MRI_X; ++i) {
//        for (int j = 0; j < MAX_MRI_Y; ++j)
//            delete[] mri[i][j];
//
//        delete[] mri[i];
//    }
//    delete[] mri;
//
//    delete[] mri_ei;
//    delete[] mri_ej;
//    delete[] mri_evi;
//    delete[] mri_evd;
//
//    return 0;
//}
//
