// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

/*
 * Fuzz driver for lcms2 color management functions.
 * Targets: cmsXYZ2Lab, cmsGDBAddPoint, cmsDesaturateLab, 
 *          cmsGDBCheckPoint, cmsCIE2000DeltaE, cmsBFDdeltaE
 */

static void parse_xyz(const uint8_t *data, size_t size, cmsCIEXYZ *xyz) {
    if (size >= sizeof(cmsCIEXYZ)) {
        memcpy(xyz, data, sizeof(cmsCIEXYZ));
    } else {
        xyz->X = 0.0;
        xyz->Y = 0.0;
        xyz->Z = 0.0;
    }
}

static void parse_lab(const uint8_t *data, size_t size, cmsCIELab *lab) {
    if (size >= sizeof(cmsCIELab)) {
        memcpy(lab, data, sizeof(cmsCIELab));
    } else {
        lab->L = 0.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

static cmsFloat64Number parse_double(const uint8_t *data, size_t size) {
    if (size >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number val;
        memcpy(&val, data, sizeof(cmsFloat64Number));
        return val;
    }
    return 0.0;
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsCIEXYZ xyz;
    cmsCIELab lab, lab2;
    cmsCIELab whitePoint;
    cmsFloat64Number amax, amin, bmax, bmin;
    cmsFloat64Number Kl, Kc, Kh;
    cmsHANDLE hGBD;
    cmsBool result;

    // Initialize defaults
    memset(&xyz, 0, sizeof(xyz));
    memset(&lab, 0, sizeof(lab));
    memset(&lab2, 0, sizeof(lab2));
    memset(&whitePoint, 0, sizeof(whitePoint));
    
    // Parse input data into structures
    parse_xyz(Data, Size, &xyz);
    parse_lab(Data, Size, &lab);
    parse_lab(Data, Size, &lab2);
    parse_lab(Data, Size, &whitePoint);
    
    amax = parse_double(Data, Size);
    amin = parse_double(Data, Size);
    bmax = parse_double(Data, Size);
    bmin = parse_double(Data, Size);
    Kl = parse_double(Data, Size);
    Kc = parse_double(Data, Size);
    Kh = parse_double(Data, Size);

    // 1. cmsXYZ2Lab
    // Test with custom white point
    cmsXYZ2Lab(&whitePoint, &lab, &xyz);
    // Test with NULL white point (default D50)
    cmsXYZ2Lab(NULL, &lab, &xyz);

    // 3. cmsDesaturateLab
    // Perform gamut mapping
    result = cmsDesaturateLab(&lab, amax, amin, bmax, bmin);
    (void)result; // Suppress unused warning

    // 5. cmsCIE2000DeltaE
    cmsFloat64Number deltaE2000 = cmsCIE2000DeltaE(&lab, &lab2, Kl, Kc, Kh);
    (void)deltaE2000;

    // 6. cmsBFDdeltaE
    cmsFloat64Number deltaEBFD = cmsBFDdeltaE(&lab, &lab2);
    (void)deltaEBFD;

    // 2. cmsGDBAddPoint and 4. cmsGDBCheckPoint require a GBD handle.
    // cmsGDBCreate is not available in the provided API list or standard lcms2.h public API.
    // cmsGDBCompute requires an existing handle. Without a creation function, 
    // we cannot safely test these GBD functions in a standalone fuzz driver 
    // without risking undefined behavior or requiring internal API access.
    // Therefore, we skip GBD-related tests to ensure compilation and stability.
    // hGBD = cmsGDBCreate(); // Removed as it causes compilation error
    // if (hGBD != NULL) { ... }

    return 0;
}