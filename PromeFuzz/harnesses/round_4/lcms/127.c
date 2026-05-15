// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    write_dummy_file(data, size);
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    remove_dummy_file();
    return profile;
}

static cmsToneCurve* create_tone_curve_from_data(const uint8_t *data, size_t size) {
    if (size < 2) return NULL;
    
    size_t num_entries = size / 2;
    if (num_entries > 4096) num_entries = 4096;
    
    cmsUInt16Number *table = (cmsUInt16Number*)malloc(num_entries * sizeof(cmsUInt16Number));
    if (!table) return NULL;
    
    for (size_t i = 0; i < num_entries; i++) {
        table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    cmsToneCurve *curve = cmsBuildTabulatedToneCurve16(NULL, num_entries, table);
    free(table);
    return curve;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create profile from input data
    cmsHPROFILE profile = create_profile_from_data(Data, Size);
    
    if (profile) {
        // Test cmsIsMatrixShaper
        cmsIsMatrixShaper(profile);
        
        // Test cmsDetectDestinationBlackPoint with different intents
        cmsCIEXYZ blackPoint1;
        for (cmsUInt32Number intent = 0; intent < 4; intent++) {
            cmsDetectDestinationBlackPoint(&blackPoint1, profile, intent, 0);
        }
        
        // Test cmsDetectTAC
        cmsDetectTAC(profile);
        
        // Test cmsDetectBlackPoint with different intents
        cmsCIEXYZ blackPoint2;
        for (cmsUInt32Number intent = 0; intent < 4; intent++) {
            cmsDetectBlackPoint(&blackPoint2, profile, intent, 0);
        }
        
        // Test cmsDetectRGBProfileGamma with different thresholds
        for (double threshold = 0.1; threshold <= 1.0; threshold += 0.3) {
            cmsDetectRGBProfileGamma(profile, threshold);
        }
        
        cmsCloseProfile(profile);
    }
    
    // Test cmsEstimateGamma with tone curve
    cmsToneCurve *toneCurve = create_tone_curve_from_data(Data, Size);
    if (toneCurve) {
        for (double precision = 0.001; precision <= 0.1; precision *= 10) {
            cmsEstimateGamma(toneCurve, precision);
        }
        cmsFreeToneCurve(toneCurve);
    }
    
    return 0;
}