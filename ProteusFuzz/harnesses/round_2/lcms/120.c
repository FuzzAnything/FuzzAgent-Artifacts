// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

/* Helper to generate a cmsCIELab struct from fuzz data */
static void generate_lab_from_data(const uint8_t *data, size_t offset, cmsCIELab *lab, size_t size) {
    if (offset + 24 <= size) {
        memcpy(&lab->L, data + offset, sizeof(double));
        memcpy(&lab->a, data + offset + 8, sizeof(double));
        memcpy(&lab->b, data + offset + 16, sizeof(double));
    } else {
        lab->L = 0.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

/* Helper to generate a cmsCIEXYZ struct from fuzz data */
static void generate_xyz_from_data(const uint8_t *data, size_t offset, cmsCIEXYZ *xyz, size_t size) {
    if (offset + 24 <= size) {
        memcpy(&xyz->X, data + offset, sizeof(double));
        memcpy(&xyz->Y, data + offset + 8, sizeof(double));
        memcpy(&xyz->Z, data + offset + 16, sizeof(double));
    } else {
        xyz->X = 0.0;
        xyz->Y = 0.0;
        xyz->Z = 0.0;
    }
}

/* Helper to generate a double from 8 bytes of data */
static double get_double_from_data(const uint8_t *data, size_t offset, size_t size) {
    if (offset + 8 <= size) {
        double val;
        memcpy(&val, data + offset, sizeof(double));
        return val;
    }
    return 1.0; /* Default value */
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 24) {
        return 0;
    }

    cmsCIELab Lab1, Lab2, Lab3, Lab4;
    cmsCIEXYZ xyz, whitePoint;
    cmsFloat64Number Kl, Kc, Kh;
    cmsHANDLE hGBD;
    cmsBool result;
    cmsFloat64Number deltaE;

    /* Initialize structures from input data */
    generate_lab_from_data(Data, 0, &Lab1, Size);
    generate_lab_from_data(Data, 24, &Lab2, Size);
    generate_lab_from_data(Data, 48, &Lab3, Size);
    generate_lab_from_data(Data, 72, &Lab4, Size);
    generate_xyz_from_data(Data, 96, &xyz, Size);
    generate_xyz_from_data(Data, 120, &whitePoint, Size);

    Kl = get_double_from_data(Data, 144, Size);
    Kc = get_double_from_data(Data, 152, Size);
    Kh = get_double_from_data(Data, 160, Size);

    /* 1. Fuzz cmsCIE2000DeltaE */
    deltaE = cmsCIE2000DeltaE(&Lab1, &Lab2, Kl, Kc, Kh);
    (void)deltaE;

    /* 2. Fuzz cmsGDBCheckPoint and cmsGDBAddPoint */
    /* cmsCreateGDB is not a public API in standard lcms2.h, so we skip GDB fuzzing 
       or rely on internal headers if available. Since we cannot assume internal headers,
       we skip GDB functions to avoid compilation errors. 
       If GDB functions are needed, they require specific internal API access.
    */
    // hGBD = cmsCreateGDB(NULL); // Commented out as it's not in public API
    // if (hGBD != NULL) {
    //     cmsGDBAddPoint(hGBD, &Lab3);
    //     result = cmsGDBCheckPoint(hGBD, &Lab4);
    //     (void)result;
    //     cmsDeleteGDB(hGBD);
    // }

    /* 3. Fuzz cmsDesaturateLab */
    double amax = get_double_from_data(Data, 168, Size);
    double amin = get_double_from_data(Data, 176, Size);
    double bmax = get_double_from_data(Data, 184, Size);
    double bmin = get_double_from_data(Data, 192, Size);
    result = cmsDesaturateLab(&Lab1, amax, amin, bmax, bmin);
    (void)result;

    /* 4. Fuzz cmsCIE94DeltaE */
    deltaE = cmsCIE94DeltaE(&Lab1, &Lab2);
    (void)deltaE;

    /* 5. Fuzz cmsXYZ2Lab */
    cmsCIELab labResult;
    cmsXYZ2Lab(&whitePoint, &labResult, &xyz);
    (void)labResult;

    return 0;
}