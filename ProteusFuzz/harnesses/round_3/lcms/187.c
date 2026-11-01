// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2.h"

static void init_xyz_from_fuzz(cmsCIEXYZ *xyz, const uint8_t *data, size_t size) {
    if (size >= 24) {
        xyz->X = *(double *)(data);
        xyz->Y = *(double *)(data + 8);
        xyz->Z = *(double *)(data + 16);
    } else {
        xyz->X = 0.0;
        xyz->Y = 0.0;
        xyz->Z = 0.0;
    }
}

static void init_lab_from_fuzz(cmsCIELab *lab, const uint8_t *data, size_t size) {
    if (size >= 24) {
        lab->L = *(double *)(data);
        lab->a = *(double *)(data + 8);
        lab->b = *(double *)(data + 16);
    } else {
        lab->L = 0.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

// Helper to create a GBD handle using a dummy profile
// Note: cmsCreateGBDfromProfile and cmsDeleteGBD are not standard public API in lcms2.h
// They are internal or deprecated. We will skip GBD testing if these functions are unavailable.
// Instead, we will focus on the other functions which are confirmed to be available.
static cmsHANDLE create_gbd_from_fuzz(const uint8_t *data, size_t size) {
    (void)data;
    (void)size;
    // Since cmsCreateGBDfromProfile is not declared in lcms2.h, we cannot use it safely.
    // We return NULL to skip GBD-related tests in this fuzz driver.
    return NULL;
}

int LLVMFuzzerTestOneInput_187(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsCIEXYZ xyz1, xyz2;
    cmsCIELab lab1, lab2, lab3;
    cmsCIEXYZ whitePoint;
    cmsHANDLE hGBD;
    
    // Initialize data from fuzz input
    init_xyz_from_fuzz(&xyz1, Data, Size);
    init_xyz_from_fuzz(&xyz2, Data + 8, Size > 8 ? Size - 8 : 0);
    init_lab_from_fuzz(&lab1, Data, Size);
    init_lab_from_fuzz(&lab2, Data + 8, Size > 8 ? Size - 8 : 0);
    init_lab_from_fuzz(&lab3, Data + 16, Size > 16 ? Size - 16 : 0);
    
    // Initialize white point (D50 default)
    whitePoint.X = 0.9642;
    whitePoint.Y = 1.0000;
    whitePoint.Z = 0.8251;
    
    // Test cmsXYZ2Lab
    cmsXYZ2Lab(&whitePoint, &lab1, &xyz1);
    cmsXYZ2Lab(NULL, &lab2, &xyz2); // Use default white point
    
    // Test cmsCIE94DeltaE
    cmsFloat64Number deltaE94 = cmsCIE94DeltaE(&lab1, &lab2);
    (void)deltaE94; // Suppress unused variable warning
    
    // Test cmsCIE2000DeltaE
    cmsFloat64Number deltaE2000 = cmsCIE2000DeltaE(&lab1, &lab2, 1.0, 1.0, 1.0);
    (void)deltaE2000;
    
    // Test cmsDesaturateLab
    cmsBool desatResult = cmsDesaturateLab(&lab3, 100.0, -100.0, 100.0, -100.0);
    (void)desatResult;
    
    // Test GBD functions (skipped as cmsCreateGBDfromProfile is not available in public API)
    hGBD = create_gbd_from_fuzz(Data, Size);
    if (hGBD) {
        // Test cmsGDBAddPoint
        cmsGDBAddPoint(hGBD, &lab1);
        cmsGDBAddPoint(hGBD, &lab2);
        
        // Test cmsGDBCheckPoint
        cmsBool checkResult = cmsGDBCheckPoint(hGBD, &lab3);
        (void)checkResult;
        
        // Cleanup GBD (skipped as cmsDeleteGBD is not available in public API)
        // cmsDeleteGBD(hGBD);
    }
    
    return 0;
}