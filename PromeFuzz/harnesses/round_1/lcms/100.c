// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 2 * sizeof(cmsFloat64Number)) {
        return NULL;
    }
    
    cmsFloat64Number gamma;
    memcpy(&gamma, data, sizeof(cmsFloat64Number));
    
    cmsFloat64Number offset;
    memcpy(&offset, data + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    
    cmsFloat64Number params[3] = {gamma, 0.0, offset};
    return cmsBuildParametricToneCurve(NULL, 1, params);
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_tone_curve(cmsToneCurve* curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size < 4 * sizeof(cmsFloat64Number)) {
        return 0;
    }
    
    cmsCIExyY white_point;
    cmsFloat64Number temp_k;
    cmsFloat64Number result_temp;
    
    memcpy(&white_point.x, Data, sizeof(cmsFloat64Number));
    memcpy(&white_point.y, Data + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    memcpy(&white_point.Y, Data + 2 * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    memcpy(&temp_k, Data + 3 * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    
    const cmsCIExyY* d50_white_point = cmsD50_xyY();
    
    cmsBool wp_result = cmsWhitePointFromTemp(&white_point, temp_k);
    
    cmsBool temp_result = cmsTempFromWhitePoint(&result_temp, &white_point);
    
    cmsToneCurve* tone_curve = create_tone_curve_from_data(Data, Size);
    
    cmsHPROFILE gray_profile = cmsCreateGrayProfile(&white_point, tone_curve);
    cleanup_profile(gray_profile);
    
    cmsHPROFILE lab2_profile = cmsCreateLab2Profile(&white_point);
    cleanup_profile(lab2_profile);
    
    cmsHPROFILE lab4_profile = cmsCreateLab4Profile(&white_point);
    cleanup_profile(lab4_profile);
    
    cleanup_tone_curve(tone_curve);
    
    return 0;
}