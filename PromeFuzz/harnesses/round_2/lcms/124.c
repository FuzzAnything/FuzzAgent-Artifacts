// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cmsCreateLab4Profile(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIExyY)) return;
    
    cmsCIExyY whitePoint;
    memcpy(&whitePoint, data, sizeof(cmsCIExyY));
    
    cmsHPROFILE profile = cmsCreateLab4Profile(&whitePoint);
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void fuzz_cmsCreateLab2Profile(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIExyY)) return;
    
    cmsCIExyY whitePoint;
    memcpy(&whitePoint, data, sizeof(cmsCIExyY));
    
    cmsHPROFILE profile = cmsCreateLab2Profile(&whitePoint);
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void fuzz_cmsTempFromWhitePoint(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIExyY)) return;
    
    cmsCIExyY whitePoint;
    memcpy(&whitePoint, data, sizeof(cmsCIExyY));
    
    cmsFloat64Number tempK;
    cmsTempFromWhitePoint(&tempK, &whitePoint);
}

static void fuzz_cmsD50_xyY(void) {
    const cmsCIExyY* d50 = cmsD50_xyY();
    (void)d50;
}

static void fuzz_cmsXYZ2xyY(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsCIEXYZ) + sizeof(cmsCIExyY)) return;
    
    cmsCIEXYZ source;
    cmsCIExyY dest;
    
    memcpy(&source, data, sizeof(cmsCIEXYZ));
    memcpy(&dest, data + sizeof(cmsCIEXYZ), sizeof(cmsCIExyY));
    
    cmsXYZ2xyY(&dest, &source);
}

static void fuzz_cmsWhitePointFromTemp(const uint8_t *data, size_t size) {
    if (size < sizeof(cmsFloat64Number)) return;
    
    cmsFloat64Number tempK;
    memcpy(&tempK, data, sizeof(cmsFloat64Number));
    
    cmsCIExyY whitePoint;
    cmsWhitePointFromTemp(&whitePoint, tempK);
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_cmsD50_xyY();
    fuzz_cmsCreateLab4Profile(Data, Size);
    fuzz_cmsCreateLab2Profile(Data, Size);
    fuzz_cmsTempFromWhitePoint(Data, Size);
    fuzz_cmsXYZ2xyY(Data, Size);
    fuzz_cmsWhitePointFromTemp(Data, Size);
    
    return 0;
}