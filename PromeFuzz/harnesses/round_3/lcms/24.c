// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
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

static cmsToneCurve* create_test_tone_curve(cmsContext context_id) {
    // Create a simple gamma tone curve for testing
    cmsFloat64Number gamma = 2.2;
    return cmsBuildGamma(context_id, gamma);
}

static void populate_xyY_from_data(cmsCIExyY* xyY, const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(cmsFloat64Number) * 3 <= size) {
        // Copy three doubles for x, y, Y
        memcpy(&xyY->x, data + offset, sizeof(cmsFloat64Number));
        memcpy(&xyY->y, data + offset + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
        memcpy(&xyY->Y, data + offset + sizeof(cmsFloat64Number) * 2, sizeof(cmsFloat64Number));
        
        // Ensure values are within valid ranges
        if (xyY->x < 0.0) xyY->x = 0.0;
        if (xyY->x > 1.0) xyY->x = 1.0;
        if (xyY->y < 0.0) xyY->y = 0.0;
        if (xyY->y > 1.0) xyY->y = 1.0;
        if (xyY->Y < 0.0) xyY->Y = 0.0;
        if (xyY->Y > 1.0) xyY->Y = 1.0;
    } else {
        // Use default D50 values if not enough data
        const cmsCIExyY* d50 = cmsD50_xyY();
        xyY->x = d50->x;
        xyY->y = d50->y;
        xyY->Y = d50->Y;
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a context for thread-safe functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Test 1: cmsCreateLab2Profile
    {
        cmsCIExyY whitePoint;
        populate_xyY_from_data(&whitePoint, Data, Size, 0);
        
        cmsHPROFILE labProfile = cmsCreateLab2Profile(&whitePoint);
        if (labProfile) {
            cmsCloseProfile(labProfile);
        }
    }
    
    // Test 2: cmsCreateRGBProfileTHR
    {
        cmsCIExyY whitePoint;
        cmsCIExyYTRIPLE primaries;
        
        // Use different offsets for white point and primaries
        populate_xyY_from_data(&whitePoint, Data, Size, 0);
        
        // Populate primaries
        if (Size >= sizeof(cmsFloat64Number) * 9) {
            memcpy(&primaries.Red.x, Data, sizeof(cmsFloat64Number));
            memcpy(&primaries.Red.y, Data + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
            memcpy(&primaries.Red.Y, Data + sizeof(cmsFloat64Number) * 2, sizeof(cmsFloat64Number));
            
            memcpy(&primaries.Green.x, Data + sizeof(cmsFloat64Number) * 3, sizeof(cmsFloat64Number));
            memcpy(&primaries.Green.y, Data + sizeof(cmsFloat64Number) * 4, sizeof(cmsFloat64Number));
            memcpy(&primaries.Green.Y, Data + sizeof(cmsFloat64Number) * 5, sizeof(cmsFloat64Number));
            
            memcpy(&primaries.Blue.x, Data + sizeof(cmsFloat64Number) * 6, sizeof(cmsFloat64Number));
            memcpy(&primaries.Blue.y, Data + sizeof(cmsFloat64Number) * 7, sizeof(cmsFloat64Number));
            memcpy(&primaries.Blue.Y, Data + sizeof(cmsFloat64Number) * 8, sizeof(cmsFloat64Number));
        } else {
            // Use default primaries if not enough data
            const cmsCIExyY* d50 = cmsD50_xyY();
            primaries.Red = *d50;
            primaries.Green = *d50;
            primaries.Blue = *d50;
        }
        
        // Create tone curves
        cmsToneCurve* transferFunction[3] = {NULL, NULL, NULL};
        for (int i = 0; i < 3; i++) {
            transferFunction[i] = create_test_tone_curve(context);
        }
        
        cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(context, &whitePoint, &primaries, transferFunction);
        if (rgbProfile) {
            cmsCloseProfile(rgbProfile);
        }
        
        // Free tone curves
        for (int i = 0; i < 3; i++) {
            if (transferFunction[i]) {
                cmsFreeToneCurve(transferFunction[i]);
            }
        }
    }
    
    // Test 3: cmsD50_xyY (simple call)
    {
        const cmsCIExyY* d50 = cmsD50_xyY();
        (void)d50; // Use to avoid unused variable warning
    }
    
    // Test 4: cmsCreateLab2ProfileTHR
    {
        cmsCIExyY whitePoint;
        populate_xyY_from_data(&whitePoint, Data, Size, Size > 50 ? 50 : 0);
        
        cmsHPROFILE lab2Profile = cmsCreateLab2ProfileTHR(context, &whitePoint);
        if (lab2Profile) {
            cmsCloseProfile(lab2Profile);
        }
    }
    
    // Test 5: cmsxyY2XYZ
    {
        cmsCIExyY source;
        cmsCIEXYZ dest;
        
        populate_xyY_from_data(&source, Data, Size, Size > 100 ? 100 : 0);
        cmsxyY2XYZ(&dest, &source);
    }
    
    // Test 6: cmsCreateLab4ProfileTHR
    {
        cmsCIExyY whitePoint;
        populate_xyY_from_data(&whitePoint, Data, Size, Size > 150 ? 150 : 0);
        
        cmsHPROFILE lab4Profile = cmsCreateLab4ProfileTHR(context, &whitePoint);
        if (lab4Profile) {
            cmsCloseProfile(lab4Profile);
        }
    }
    
    // Cleanup
    cmsDeleteContext(context);
    
    return 0;
}