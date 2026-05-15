// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_dummy_tone_curve(void) {
    // Create a simple gamma curve
    return cmsBuildGamma(NULL, 2.2);
}

static void fill_xyY_from_data(cmsCIExyY* xyY, const uint8_t* data, size_t* offset, size_t size) {
    if (xyY == NULL) return;
    
    if (*offset + sizeof(double) * 3 <= size) {
        memcpy(&xyY->x, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyY->y, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyY->Y, data + *offset, sizeof(double));
        *offset += sizeof(double);
    } else {
        // Use default values if not enough data
        xyY->x = 0.3127;
        xyY->y = 0.3290;
        xyY->Y = 1.0;
    }
}

static void fill_xyY_triple_from_data(cmsCIExyYTRIPLE* triple, const uint8_t* data, size_t* offset, size_t size) {
    if (triple == NULL) return;
    
    fill_xyY_from_data(&triple->Red, data, offset, size);
    fill_xyY_from_data(&triple->Green, data, offset, size);
    fill_xyY_from_data(&triple->Blue, data, offset, size);
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    size_t offset = 0;
    
    // Create dummy white point from fuzzer data
    cmsCIExyY whitePoint;
    fill_xyY_from_data(&whitePoint, Data, &offset, Size);
    
    // Create dummy primaries from fuzzer data
    cmsCIExyYTRIPLE primaries;
    fill_xyY_triple_from_data(&primaries, Data, &offset, Size);
    
    // Create dummy tone curves
    cmsToneCurve* transferFunction[3] = {NULL, NULL, NULL};
    cmsToneCurve* grayTransferFunction = NULL;
    
    for (int i = 0; i < 3; i++) {
        transferFunction[i] = create_dummy_tone_curve();
    }
    grayTransferFunction = create_dummy_tone_curve();
    
    // Test cmsCreateLab4Profile
    cmsHPROFILE lab4Profile = cmsCreateLab4Profile(&whitePoint);
    if (lab4Profile) {
        cmsCloseProfile(lab4Profile);
    }
    
    // Test cmsCreateLab2Profile
    cmsHPROFILE lab2Profile = cmsCreateLab2Profile(&whitePoint);
    if (lab2Profile) {
        cmsCloseProfile(lab2Profile);
    }
    
    // Test cmsCreateRGBProfileTHR - use default context (NULL)
    cmsHPROFILE rgbProfileTHR = cmsCreateRGBProfileTHR(NULL, &whitePoint, &primaries, transferFunction);
    if (rgbProfileTHR) {
        cmsCloseProfile(rgbProfileTHR);
    }
    
    // Test cmsCreateGrayProfile
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint, grayTransferFunction);
    if (grayProfile) {
        cmsCloseProfile(grayProfile);
    }
    
    // Test cmsCreateRGBProfile
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunction);
    if (rgbProfile) {
        cmsCloseProfile(rgbProfile);
    }
    
    // Test cmsD50_xyY
    const cmsCIExyY* d50 = cmsD50_xyY();
    (void)d50; // Use to avoid unused variable warning
    
    // Clean up tone curves
    for (int i = 0; i < 3; i++) {
        if (transferFunction[i]) {
            cmsFreeToneCurve(transferFunction[i]);
        }
    }
    if (grayTransferFunction) {
        cmsFreeToneCurve(grayTransferFunction);
    }
    
    return 0;
}