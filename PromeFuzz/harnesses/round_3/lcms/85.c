// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "lcms2.h"

static void fuzz_cmsDetectRGBProfileGamma(cmsHPROFILE hProfile) {
    if (hProfile == NULL) return;
    
    // Test with different threshold values
    cmsDetectRGBProfileGamma(hProfile, 0.001);
    cmsDetectRGBProfileGamma(hProfile, 0.01);
    cmsDetectRGBProfileGamma(hProfile, 0.1);
    cmsDetectRGBProfileGamma(hProfile, 1.0);
}

static void fuzz_cmsIsMatrixShaper(cmsHPROFILE hProfile) {
    if (hProfile == NULL) return;
    
    cmsIsMatrixShaper(hProfile);
}

static void fuzz_cmsDetectTAC(cmsHPROFILE hProfile) {
    if (hProfile == NULL) return;
    
    cmsDetectTAC(hProfile);
}

static void fuzz_cmsDetectBlackPoint(cmsHPROFILE hProfile, cmsUInt32Number intent, cmsUInt32Number flags) {
    if (hProfile == NULL) return;
    
    cmsCIEXYZ blackPoint;
    cmsDetectBlackPoint(&blackPoint, hProfile, intent, flags);
}

static void fuzz_cmsEstimateGamma(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create a simple tone curve from fuzz data
    cmsUInt32Number nEntries = 256;
    if (size < nEntries * 2) return;
    
    cmsUInt16Number *table = (cmsUInt16Number *)malloc(nEntries * sizeof(cmsUInt16Number));
    if (table == NULL) return;
    
    // Fill table with fuzz data
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        if (i * 2 + 1 < size) {
            table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
        } else {
            table[i] = 0;
        }
    }
    
    cmsToneCurve *t = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    if (t != NULL) {
        // Test with different precision values
        cmsEstimateGamma(t, 0.001);
        cmsEstimateGamma(t, 0.01);
        cmsEstimateGamma(t, 0.1);
        cmsEstimateGamma(t, 1.0);
        cmsFreeToneCurve(t);
    }
    
    free(table);
}

static void fuzz_cmsDetectDestinationBlackPoint(cmsHPROFILE hProfile, cmsUInt32Number intent, cmsUInt32Number flags) {
    if (hProfile == NULL) return;
    
    cmsCIEXYZ blackPoint;
    cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intent, flags);
}

int LLVMFuzzerTestOneInput_85(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Create a profile from fuzz data
    cmsHPROFILE hProfile = NULL;
    
    // First try to open as a profile from memory
    if (size > 0) {
        hProfile = cmsOpenProfileFromMem(data, size);
    }
    
    // If that fails, create a dummy sRGB profile
    if (hProfile == NULL) {
        hProfile = cmsCreate_sRGBProfile();
    }
    
    if (hProfile == NULL) return 0;
    
    // Extract intent and flags from fuzz data if available
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    
    if (size >= sizeof(cmsUInt32Number)) {
        memcpy(&intent, data, sizeof(cmsUInt32Number));
    }
    if (size >= 2 * sizeof(cmsUInt32Number)) {
        memcpy(&flags, data + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
    }
    
    // Limit intent to valid values (0-3 are standard rendering intents)
    intent = intent % 4;
    
    // Call all target functions
    fuzz_cmsDetectRGBProfileGamma(hProfile);
    fuzz_cmsIsMatrixShaper(hProfile);
    fuzz_cmsDetectTAC(hProfile);
    fuzz_cmsDetectBlackPoint(hProfile, intent, flags);
    fuzz_cmsEstimateGamma(data, size);
    fuzz_cmsDetectDestinationBlackPoint(hProfile, intent, flags);
    
    // Cleanup
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}