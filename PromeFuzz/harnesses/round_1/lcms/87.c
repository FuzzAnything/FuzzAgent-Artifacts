// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cmsCreate_sRGBProfile();
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    if (profile == NULL) {
        profile = cmsCreate_sRGBProfile();
    }
    return profile;
}

static cmsToneCurve* create_test_tone_curve(void) {
    cmsFloat64Number params[10] = {2.4, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    return cmsBuildParametricToneCurve(NULL, 4, params);
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsHPROFILE hProfile = create_profile_from_data(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }
    
    cmsCIEXYZ blackPoint;
    cmsUInt32Number intent;
    cmsUInt32Number flags;
    
    uint32_t seed = 0;
    memcpy(&seed, Data, sizeof(seed) > Size ? Size : sizeof(seed));
    
    intent = seed % 4;
    flags = (seed >> 2) % 4;
    
    cmsBool result_bool;
    cmsFloat64Number result_double;
    
    result_bool = cmsIsMatrixShaper(hProfile);
    
    result_bool = cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intent, flags);
    
    cmsFloat64Number threshold = 0.001 + ((seed % 1000) / 1000000.0);
    result_double = cmsDetectRGBProfileGamma(hProfile, threshold);
    
    result_bool = cmsDetectBlackPoint(&blackPoint, hProfile, intent, flags);
    
    result_double = cmsDetectTAC(hProfile);
    
    cmsToneCurve* toneCurve = create_test_tone_curve();
    if (toneCurve != NULL) {
        cmsFloat64Number precision = 0.0001 + ((seed % 1000) / 10000000.0);
        result_double = cmsEstimateGamma(toneCurve, precision);
        cmsFreeToneCurve(toneCurve);
    }
    
    cmsCloseProfile(hProfile);
    
    return 0;
}