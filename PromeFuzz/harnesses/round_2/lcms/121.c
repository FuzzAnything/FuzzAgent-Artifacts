// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
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

static cmsToneCurve* create_test_tone_curve(void) {
    // Create a simple gamma curve for testing
    cmsFloat64Number gamma = 2.2;
    return cmsBuildGamma(NULL, gamma);
}

static void parse_input(const uint8_t *Data, size_t Size, 
                       cmsCIExyY* whitePoint, cmsFloat64Number* tempK,
                       cmsFloat64Number* gamma) {
    // Initialize with default values
    const cmsCIExyY* d50 = cmsD50_xyY();
    *whitePoint = *d50;
    *tempK = 6500.0;  // Default temperature
    *gamma = 2.2;     // Default gamma
    
    // Parse input data if available
    if (Size >= sizeof(cmsCIExyY)) {
        memcpy(whitePoint, Data, sizeof(cmsCIExyY));
    }
    
    if (Size >= sizeof(cmsCIExyY) + sizeof(cmsFloat64Number)) {
        memcpy(tempK, Data + sizeof(cmsCIExyY), sizeof(cmsFloat64Number));
    }
    
    if (Size >= sizeof(cmsCIExyY) + 2 * sizeof(cmsFloat64Number)) {
        memcpy(gamma, Data + sizeof(cmsCIExyY) + sizeof(cmsFloat64Number), 
               sizeof(cmsFloat64Number));
    }
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    cmsCIExyY whitePoint;
    cmsFloat64Number tempK, gamma;
    
    // Parse input data
    parse_input(Data, Size, &whitePoint, &tempK, &gamma);
    
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
    
    // Test cmsTempFromWhitePoint
    cmsFloat64Number computedTemp;
    cmsTempFromWhitePoint(&computedTemp, &whitePoint);
    
    // Test cmsD50_xyY (no parameters needed)
    const cmsCIExyY* d50 = cmsD50_xyY();
    (void)d50;  // Use to avoid unused variable warning
    
    // Test cmsCreateGrayProfile
    cmsToneCurve* toneCurve = create_test_tone_curve();
    if (toneCurve) {
        cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint, toneCurve);
        if (grayProfile) {
            cmsCloseProfile(grayProfile);
        }
        cmsFreeToneCurve(toneCurve);
    }
    
    // Test cmsWhitePointFromTemp
    cmsCIExyY wpFromTemp;
    cmsWhitePointFromTemp(&wpFromTemp, tempK);
    
    // Test with D50 white point
    cmsHPROFILE lab4ProfileD50 = cmsCreateLab4Profile(d50);
    if (lab4ProfileD50) {
        cmsCloseProfile(lab4ProfileD50);
    }
    
    cmsHPROFILE lab2ProfileD50 = cmsCreateLab2Profile(d50);
    if (lab2ProfileD50) {
        cmsCloseProfile(lab2ProfileD50);
    }
    
    // Test temperature conversion with D50
    cmsFloat64Number tempFromD50;
    cmsTempFromWhitePoint(&tempFromD50, d50);
    
    // Test with extreme temperature values
    cmsCIExyY extremeWp;
    cmsWhitePointFromTemp(&extremeWp, 4000.0);   // Lower bound
    cmsWhitePointFromTemp(&extremeWp, 25000.0);  // Upper bound
    cmsWhitePointFromTemp(&extremeWp, 10000.0);  // Middle value
    
    // Test with invalid temperature (outside range)
    cmsWhitePointFromTemp(&extremeWp, 3000.0);   // Below range
    cmsWhitePointFromTemp(&extremeWp, 30000.0);  // Above range
    
    // Test with modified white point values
    cmsCIExyY modifiedWp = whitePoint;
    modifiedWp.x = modifiedWp.x > 0.8 ? 0.3 : 0.8;
    modifiedWp.y = modifiedWp.y > 0.8 ? 0.3 : 0.8;
    modifiedWp.Y = modifiedWp.Y > 0.8 ? 0.3 : 0.8;
    
    cmsHPROFILE modifiedProfile = cmsCreateLab4Profile(&modifiedWp);
    if (modifiedProfile) {
        cmsCloseProfile(modifiedProfile);
    }
    
    // Test with zero values (edge case)
    cmsCIExyY zeroWp = {0.0, 0.0, 0.0};
    cmsHPROFILE zeroProfile = cmsCreateLab4Profile(&zeroWp);
    if (zeroProfile) {
        cmsCloseProfile(zeroProfile);
    }
    
    // Test with very small values
    cmsCIExyY smallWp = {0.0001, 0.0001, 0.0001};
    cmsHPROFILE smallProfile = cmsCreateLab4Profile(&smallWp);
    if (smallProfile) {
        cmsCloseProfile(smallProfile);
    }
    
    return 0;
}