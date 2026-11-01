// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void dummy_cleanup(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    // Minimum size required to hold all structures we need to copy
    size_t required_size = sizeof(cmsCIEXYZ) + sizeof(cmsCIELab) + sizeof(cmsCIExyY);
    
    if (Size < required_size) {
        return 0;
    }

    cmsCIEXYZ xyz_input = {0};
    cmsCIEXYZ xyz_output = {0};
    cmsCIEXYZ black_point = {0};
    cmsCIELab lab_input = {0};
    cmsCIELab lab_output = {0};
    cmsCIExyY xyY_input = {0};
    cmsCIEXYZ xyY_output = {0};
    const cmsCIEXYZ *white_point = NULL;
    cmsBool result;

    // Extract data from input buffer safely
    memcpy(&xyz_input, Data, sizeof(cmsCIEXYZ));
    memcpy(&lab_input, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIELab));
    memcpy(&xyY_input, Data + sizeof(cmsCIEXYZ) + sizeof(cmsCIELab), sizeof(cmsCIExyY));

    // Test cmsD50_XYZ
    white_point = cmsD50_XYZ();

    // Test cmsLab2XYZ
    cmsLab2XYZ(white_point, &xyz_output, &lab_input);

    // Test cmsXYZ2Lab
    cmsXYZ2Lab(white_point, &lab_output, &xyz_input);

    // Test cmsxyY2XYZ
    // Avoid division by zero if y is 0
    if (xyY_input.y != 0.0) {
        cmsxyY2XYZ(&xyY_output, &xyY_input);
    }

    // Test cmsAdaptToIlluminant
    cmsCIEXYZ source_white = {0.95047, 1.00000, 1.08883}; // D50
    cmsCIEXYZ target_illuminant = {0.95047, 1.00000, 1.08883}; // D50
    result = cmsAdaptToIlluminant(&xyz_output, &source_white, &target_illuminant, &xyz_input);
    (void)result; // Suppress unused variable warning

    // Test cmsDetectBlackPoint
    // Create a dummy profile for testing
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile != NULL) {
        cmsUInt32Number intent = 0; // Perceptual intent
        cmsUInt32Number flags = 0;
        result = cmsDetectBlackPoint(&black_point, hProfile, intent, flags);
        (void)result; // Suppress unused variable warning
        dummy_cleanup(hProfile);
    }

    return 0;
}