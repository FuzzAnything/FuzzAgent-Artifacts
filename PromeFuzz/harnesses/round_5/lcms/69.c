// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void test_with_white_point(const cmsCIExyY* wp) {
    cmsHPROFILE hProfile;
    
    hProfile = cmsCreateLab4Profile(wp);
    if (hProfile) cmsCloseProfile(hProfile);
    
    hProfile = cmsCreateLab2Profile(wp);
    if (hProfile) cmsCloseProfile(hProfile);
    
    cmsFloat64Number tempK;
    cmsTempFromWhitePoint(&tempK, wp);
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx) {
        hProfile = cmsCreateLab4ProfileTHR(ctx, wp);
        if (hProfile) cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIExyY)) {
        return 0;
    }
    
    cmsCIExyY whitePoint;
    memcpy(&whitePoint, Data, sizeof(cmsCIExyY));
    
    test_with_white_point(&whitePoint);
    
    const cmsCIExyY* d50 = cmsD50_xyY();
    test_with_white_point(d50);
    
    if (Size >= sizeof(cmsCIExyY) + sizeof(cmsFloat64Number)) {
        cmsFloat64Number tempK;
        memcpy(&tempK, Data + sizeof(cmsCIExyY), sizeof(cmsFloat64Number));
        
        cmsCIExyY wpFromTemp;
        cmsWhitePointFromTemp(&wpFromTemp, tempK);
        
        cmsFloat64Number tempK2;
        cmsTempFromWhitePoint(&tempK2, &wpFromTemp);
    }
    
    return 0;
}