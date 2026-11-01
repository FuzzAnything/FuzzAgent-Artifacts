// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

static void fuzz_cmsCIELab(cmsCIELab* lab, const uint8_t* data, size_t offset, size_t size) {
    if (offset + sizeof(double) <= size) {
        lab->L = *(double*)(data + offset);
        offset += sizeof(double);
    }
    if (offset + sizeof(double) <= size) {
        lab->a = *(double*)(data + offset);
        offset += sizeof(double);
    }
    if (offset + sizeof(double) <= size) {
        lab->b = *(double*)(data + offset);
    }
}

static void fuzz_cmsCIEXYZ(cmsCIEXYZ* xyz, const uint8_t* data, size_t offset, size_t size) {
    if (offset + sizeof(double) <= size) {
        xyz->X = *(double*)(data + offset);
        offset += sizeof(double);
    }
    if (offset + sizeof(double) <= size) {
        xyz->Y = *(double*)(data + offset);
        offset += sizeof(double);
    }
    if (offset + sizeof(double) <= size) {
        xyz->Z = *(double*)(data + offset);
    }
}

static cmsFloat64Number fuzz_double(const uint8_t* data, size_t offset, size_t size) {
    if (offset + sizeof(double) <= size) {
        return *(double*)(data + offset);
    }
    return 1.0;
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsCIELab lab1, lab2, lab3;
    cmsCIEXYZ xyz, whitePoint;
    cmsFloat64Number amax, amin, bmax, bmin;
    cmsFloat64Number Kl, Kc, Kh;
    cmsHANDLE hGBD;

    // Initialize default values
    memset(&lab1, 0, sizeof(lab1));
    memset(&lab2, 0, sizeof(lab2));
    memset(&lab3, 0, sizeof(lab3));
    memset(&xyz, 0, sizeof(xyz));
    memset(&whitePoint, 0, sizeof(whitePoint));
    amax = amin = bmax = bmin = Kl = Kc = Kh = 1.0;
    hGBD = NULL;

    size_t offset = 0;

    // Fuzz Lab1
    fuzz_cmsCIELab(&lab1, Data, offset, Size);
    offset += 3 * sizeof(double);
    if (offset > Size) offset = 0;

    // Fuzz Lab2
    fuzz_cmsCIELab(&lab2, Data, offset, Size);
    offset += 3 * sizeof(double);
    if (offset > Size) offset = 0;

    // Fuzz Lab3 (for GBD operations)
    fuzz_cmsCIELab(&lab3, Data, offset, Size);
    offset += 3 * sizeof(double);
    if (offset > Size) offset = 0;

    // Fuzz XYZ
    fuzz_cmsCIEXYZ(&xyz, Data, offset, Size);
    offset += 3 * sizeof(double);
    if (offset > Size) offset = 0;

    // Fuzz WhitePoint
    fuzz_cmsCIEXYZ(&whitePoint, Data, offset, Size);
    offset += 3 * sizeof(double);
    if (offset > Size) offset = 0;

    // Fuzz limits for cmsDesaturateLab
    if (offset + sizeof(double) <= Size) amax = *(double*)(Data + offset); offset += sizeof(double);
    if (offset + sizeof(double) <= Size) amin = *(double*)(Data + offset); offset += sizeof(double);
    if (offset + sizeof(double) <= Size) bmax = *(double*)(Data + offset); offset += sizeof(double);
    if (offset + sizeof(double) <= Size) bmin = *(double*)(Data + offset); offset += sizeof(double);

    // Fuzz weighting factors
    if (offset + sizeof(double) <= Size) Kl = *(double*)(Data + offset); offset += sizeof(double);
    if (offset + sizeof(double) <= Size) Kc = *(double*)(Data + offset); offset += sizeof(double);
    if (offset + sizeof(double) <= Size) Kh = *(double*)(Data + offset); offset += sizeof(double);

    // Test cmsDesaturateLab
    cmsBool res1 = cmsDesaturateLab(&lab1, amax, amin, bmax, bmin);
    (void)res1;

    // Test cmsXYZ2Lab
    cmsXYZ2Lab(&whitePoint, &lab2, &xyz);

    // Test cmsCIE2000DeltaE
    cmsFloat64Number deltaE2000 = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    (void)deltaE2000;

    // Test cmsBFDdeltaE
    cmsFloat64Number deltaEBFD = cmsBFDdeltaE(&lab1, &lab2);
    (void)deltaEBFD;

    // Test GBD functions
    // Note: cmsCreateGBD is not a public API in standard lcms2.h. 
    // It is typically internal or requires specific flags. 
    // Since the API is not available for direct creation in standard builds, 
    // we skip GBD testing to avoid compilation errors, 
    // or we would need to check if the specific version exposes it.
    // Given the error, we assume standard lcms2.h does not expose cmsCreateGBD.
    // We will skip the GBD part to ensure compilation.
    
    return 0;
}