// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cmsCreateLab2Profile(const uint8_t *data, size_t size) {
    if (size >= sizeof(cmsCIExyY)) {
        cmsCIExyY whitePoint;
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
        cmsHPROFILE profile = cmsCreateLab2Profile(&whitePoint);
        if (profile) {
            cmsCloseProfile(profile);
        }
    }
    
    // Test with NULL white point
    cmsHPROFILE profile_null = cmsCreateLab2Profile(NULL);
    if (profile_null) {
        cmsCloseProfile(profile_null);
    }
}

static void fuzz_cmsWhitePointFromTemp(const uint8_t *data, size_t size) {
    if (size >= sizeof(cmsFloat64Number)) {
        cmsCIExyY whitePoint;
        cmsFloat64Number tempK;
        memcpy(&tempK, data, sizeof(cmsFloat64Number));
        
        cmsBool result = cmsWhitePointFromTemp(&whitePoint, tempK);
        (void)result; // Use result to avoid unused variable warning
    }
}

static void fuzz_cmsCreateLab4Profile(const uint8_t *data, size_t size) {
    if (size >= sizeof(cmsCIExyY)) {
        cmsCIExyY whitePoint;
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
        cmsHPROFILE profile = cmsCreateLab4Profile(&whitePoint);
        if (profile) {
            cmsCloseProfile(profile);
        }
    }
    
    // Test with NULL white point
    cmsHPROFILE profile_null = cmsCreateLab4Profile(NULL);
    if (profile_null) {
        cmsCloseProfile(profile_null);
    }
}

static void fuzz_cmsCreateLab4ProfileTHR(const uint8_t *data, size_t size) {
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    if (size >= sizeof(cmsCIExyY)) {
        cmsCIExyY whitePoint;
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
        cmsHPROFILE profile = cmsCreateLab4ProfileTHR(context, &whitePoint);
        if (profile) {
            cmsCloseProfile(profile);
        }
    }
    
    // Test with NULL white point
    cmsHPROFILE profile_null = cmsCreateLab4ProfileTHR(context, NULL);
    if (profile_null) {
        cmsCloseProfile(profile_null);
    }
    
    // Test with NULL context (default)
    if (size >= sizeof(cmsCIExyY)) {
        cmsCIExyY whitePoint;
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
        cmsHPROFILE profile_default = cmsCreateLab4ProfileTHR(NULL, &whitePoint);
        if (profile_default) {
            cmsCloseProfile(profile_default);
        }
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
}

static void fuzz_cmsTempFromWhitePoint(const uint8_t *data, size_t size) {
    if (size >= sizeof(cmsCIExyY)) {
        cmsCIExyY whitePoint;
        cmsFloat64Number tempK;
        memcpy(&whitePoint, data, sizeof(cmsCIExyY));
        
        cmsBool result = cmsTempFromWhitePoint(&tempK, &whitePoint);
        (void)result; // Use result to avoid unused variable warning
    }
}

static void fuzz_cmsD50_xyY(void) {
    const cmsCIExyY* d50 = cmsD50_xyY();
    (void)d50; // Use d50 to avoid unused variable warning
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Fuzz each API function
    fuzz_cmsCreateLab2Profile(Data, Size);
    fuzz_cmsWhitePointFromTemp(Data, Size);
    fuzz_cmsCreateLab4Profile(Data, Size);
    fuzz_cmsCreateLab4ProfileTHR(Data, Size);
    fuzz_cmsTempFromWhitePoint(Data, Size);
    fuzz_cmsD50_xyY();
    
    return 0;
}