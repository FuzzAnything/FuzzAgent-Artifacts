// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void init_lab_from_data(const uint8_t *data, size_t size, cmsCIELab *lab) {
    if (size >= 24) {
        memcpy(&lab->L, data, sizeof(double));
        memcpy(&lab->a, data + 8, sizeof(double));
        memcpy(&lab->b, data + 16, sizeof(double));
    } else {
        lab->L = 0.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

static void init_xyz_from_data(const uint8_t *data, size_t size, cmsCIEXYZ *xyz) {
    if (size >= 24) {
        memcpy(&xyz->X, data, sizeof(double));
        memcpy(&xyz->Y, data + 8, sizeof(double));
        memcpy(&xyz->Z, data + 16, sizeof(double));
    } else {
        xyz->X = 0.0;
        xyz->Y = 0.0;
        xyz->Z = 0.0;
    }
}

static cmsFloat64Number extract_double(const uint8_t *data, size_t size, size_t offset) {
    if (size >= offset + 8) {
        double val;
        memcpy(&val, data + offset, sizeof(double));
        return (cmsFloat64Number)val;
    }
    return 1.0;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsCIELab Lab1, Lab2, Lab3, Lab4;
    cmsCIEXYZ XYZ1, XYZ2, WhitePoint;
    cmsFloat64Number Kl, Kc, Kh;
    cmsFloat64Number amax, amin, bmax, bmin;
    cmsHANDLE hGBD = NULL;
    cmsBool ret;
    cmsFloat64Number delta;

    // Initialize Lab1
    init_lab_from_data(Data, Size, &Lab1);

    // Initialize Lab2
    init_lab_from_data(Data, Size, &Lab2);

    // Initialize Lab3
    init_lab_from_data(Data, Size, &Lab3);

    // Initialize Lab4
    init_lab_from_data(Data, Size, &Lab4);

    // Initialize XYZ1
    init_xyz_from_data(Data, Size, &XYZ1);

    // Initialize XYZ2
    init_xyz_from_data(Data, Size, &XYZ2);

    // Initialize WhitePoint (D50 default if not enough data)
    WhitePoint.X = 0.9642;
    WhitePoint.Y = 1.0000;
    WhitePoint.Z = 0.8251;
    if (Size >= 24) {
        init_xyz_from_data(Data, Size, &WhitePoint);
    }

    // Extract weighting factors
    Kl = extract_double(Data, Size, 0);
    Kc = extract_double(Data, Size, 8);
    Kh = extract_double(Data, Size, 16);

    // Extract gamut limits
    amax = extract_double(Data, Size, 24);
    amin = extract_double(Data, Size, 32);
    bmax = extract_double(Data, Size, 40);
    bmin = extract_double(Data, Size, 48);

    // 1. Test cmsCIE2000DeltaE
    delta = cmsCIE2000DeltaE(&Lab1, &Lab2, Kl, Kc, Kh);
    (void)delta;

    // 2. Test cmsDeltaE
    delta = cmsDeltaE(&Lab1, &Lab2);
    (void)delta;

    // 3. Test cmsXYZ2Lab
    cmsXYZ2Lab(&WhitePoint, &Lab3, &XYZ1);
    cmsXYZ2Lab(NULL, &Lab4, &XYZ2);

    // 4. Test cmsDesaturateLab
    ret = cmsDesaturateLab(&Lab3, amax, amin, bmax, bmin);
    (void)ret;

    // 5. Test GBD functions (cmsGDBCheckPoint, cmsGDBAddPoint)
    // Create a GBD handle using cmsCreateGBD (if available in lcms2.h)
    // Note: cmsCreateGBD might not be available in all versions, so we check if it's defined
    #ifdef cmsCreateGBD
    hGBD = cmsCreateGBD();
    if (hGBD != NULL) {
        // Add points to GBD
        cmsGDBAddPoint(hGBD, &Lab1);
        cmsGDBAddPoint(hGBD, &Lab2);
        cmsGDBAddPoint(hGBD, &Lab3);
        cmsGDBAddPoint(hGBD, &Lab4);

        // Check points
        cmsGDBCheckPoint(hGBD, &Lab1);
        cmsGDBCheckPoint(hGBD, &Lab2);
        cmsGDBCheckPoint(hGBD, &Lab3);
        cmsGDBCheckPoint(hGBD, &Lab4);

        // Cleanup GBD
        cmsDeleteGBD(hGBD);
    }
    #endif

    return 0;
}