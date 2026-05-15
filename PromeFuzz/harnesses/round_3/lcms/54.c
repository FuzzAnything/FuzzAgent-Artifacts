// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static void fuzz_cmsCIExyY_from_data(cmsCIExyY* wp, const uint8_t* data, size_t size) {
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(wp, data, sizeof(cmsCIExyY));
    } else {
        wp->x = 0.3127;
        wp->y = 0.3290;
        wp->Y = 1.0;
    }
}

static cmsFloat64Number fuzz_temp_from_data(const uint8_t* data, size_t size) {
    cmsFloat64Number temp = 6500.0;
    if (size >= sizeof(cmsFloat64Number)) {
        memcpy(&temp, data, sizeof(cmsFloat64Number));
        if (temp < 0) temp = -temp;
        if (temp < 4000.0) temp = 4000.0;
        if (temp > 25000.0) temp = 25000.0;
    }
    return temp;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsCIExyY whitePoint;
    cmsCIEXYZ xyz;
    cmsFloat64Number tempK;
    cmsBool boolResult;
    cmsHPROFILE profile;

    fuzz_cmsCIExyY_from_data(&whitePoint, Data, Size);

    size_t offset = sizeof(cmsCIExyY) % Size;
    tempK = fuzz_temp_from_data(Data + offset, Size - offset);

    const cmsCIExyY* d50 = cmsD50_xyY();

    profile = cmsCreateLab2Profile(&whitePoint);
    if (profile) cmsCloseProfile(profile);

    profile = cmsCreateLab2Profile(NULL);
    if (profile) cmsCloseProfile(profile);

    boolResult = cmsTempFromWhitePoint(&tempK, &whitePoint);

    boolResult = cmsWhitePointFromTemp(&whitePoint, tempK);

    if (Size >= sizeof(cmsCIExyY) + sizeof(cmsFloat64Number)) {
        cmsCIExyY source;
        memcpy(&source, Data, sizeof(cmsCIExyY));
        cmsxyY2XYZ(&xyz, &source);
    }

    profile = cmsCreateLab4Profile(&whitePoint);
    if (profile) cmsCloseProfile(profile);

    profile = cmsCreateLab4Profile(d50);
    if (profile) cmsCloseProfile(profile);

    return 0;
}