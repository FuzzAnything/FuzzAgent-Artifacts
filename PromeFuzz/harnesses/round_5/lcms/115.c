// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    // Try to open as memory profile first
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile) return hProfile;
    
    // If that fails, write to file and try to open from file
    write_dummy_file(data, size);
    hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    return hProfile;
}

static cmsToneCurve* create_tone_curve_from_data(const uint8_t *data, size_t size) {
    if (size < 2) return NULL;
    
    // Create a simple gamma curve using data to determine parameters
    cmsFloat64Number gamma = 2.2;
    if (size >= 8) {
        // Use first 8 bytes to create a double value for gamma
        uint64_t val = 0;
        memcpy(&val, data, size > 8 ? 8 : size);
        gamma = 0.1 + ((double)(val % 1000) / 100.0); // Gamma between 0.1 and 10.0
    }
    
    cmsFloat64Number params[10] = {0};
    params[0] = gamma;
    
    return cmsBuildParametricToneCurve(0, 1, params);
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create profile from input data
    cmsHPROFILE hProfile = create_profile_from_data(Data, Size);
    
    // Create tone curve from input data
    cmsToneCurve* toneCurve = create_tone_curve_from_data(Data, Size);
    
    // Extract parameters from input data
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    cmsFloat64Number precision = 0.01;
    cmsFloat64Number threshold = 0.01;
    
    if (Size >= 4) {
        intent = Data[0] % 4; // 0-3 for valid intents
        flags = Data[1];
        
        // Create floating point values from bytes
        uint16_t prec_val = 0;
        uint16_t thresh_val = 0;
        if (Size >= 6) {
            memcpy(&prec_val, Data + 2, 2);
            memcpy(&thresh_val, Data + 4, 2);
        } else if (Size >= 4) {
            prec_val = Data[2];
            thresh_val = Data[3];
        }
        precision = 0.001 + ((double)prec_val / 65535.0) * 0.1;
        threshold = 0.001 + ((double)thresh_val / 65535.0) * 0.1;
    }
    
    // Test cmsDetectDestinationBlackPoint
    if (hProfile) {
        cmsCIEXYZ blackPoint1;
        cmsDetectDestinationBlackPoint(&blackPoint1, hProfile, intent, flags);
    }
    
    // Test cmsDetectBlackPoint
    if (hProfile) {
        cmsCIEXYZ blackPoint2;
        cmsDetectBlackPoint(&blackPoint2, hProfile, intent, flags);
    }
    
    // Test cmsIsMatrixShaper
    if (hProfile) {
        cmsIsMatrixShaper(hProfile);
    }
    
    // Test cmsEstimateGamma
    if (toneCurve) {
        cmsEstimateGamma(toneCurve, precision);
    }
    
    // Test cmsDetectRGBProfileGamma
    if (hProfile) {
        cmsDetectRGBProfileGamma(hProfile, threshold);
    }
    
    // Test cmsDetectTAC
    if (hProfile) {
        cmsDetectTAC(hProfile);
    }
    
    // Cleanup
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}