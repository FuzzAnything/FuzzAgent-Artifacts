// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 11) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsCIEXYZ BlackPoint;
    cmsCIEXYZ xyz_out;
    cmsCIELab Lab;
    cmsCIExyY xyY;
    cmsUInt16Number encoded_xyz[3];
    cmsUInt16Number float_xyz_input[3];
    const cmsCIEXYZ *d50;
    cmsBool result;
    cmsUInt32Number intent;
    cmsUInt32Number flags;

    // Initialize variables with fuzzed data safely
    intent = Data[0];
    flags = Data[1];
    
    // Initialize Lab values
    Lab.L = (Data[2] * 100.0) / 255.0;
    Lab.a = ((int8_t)Data[3]) * 100.0 / 128.0;
    Lab.b = ((int8_t)Data[4]) * 100.0 / 128.0;

    // Initialize xyY values
    xyY.x = (Data[5] * 1.0) / 255.0;
    xyY.y = (Data[6] * 1.0) / 255.0;
    xyY.Y = (Data[7] * 100.0) / 255.0;

    // Initialize encoded XYZ
    float_xyz_input[0] = Data[8];
    float_xyz_input[1] = Data[9];
    float_xyz_input[2] = Data[10];

    // Test cmsD50_XYZ
    d50 = cmsD50_XYZ();
    if (d50) {
        // Use d50 for cmsLab2XYZ
        cmsLab2XYZ(d50, &xyz_out, &Lab);
    }

    // Test cmsLab2XYZ with NULL white point (uses D50)
    cmsLab2XYZ(NULL, &xyz_out, &Lab);

    // Test cmsxyY2XYZ
    if (xyY.y != 0.0) {
        cmsxyY2XYZ(&xyz_out, &xyY);
    }

    // Test cmsXYZEncoded2Float
    cmsXYZEncoded2Float(&xyz_out, float_xyz_input);

    // Test cmsFloat2XYZEncoded
    cmsFloat2XYZEncoded(encoded_xyz, &xyz_out);

    // Create a dummy profile for cmsDetectDestinationBlackPoint
    // We'll try to load a profile from a dummy file
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        hProfile = cmsOpenProfileFromFile(dummy_file, "r");
    }

    if (hProfile) {
        // Test cmsDetectDestinationBlackPoint
        result = cmsDetectDestinationBlackPoint(&BlackPoint, hProfile, intent, flags);
        (void)result; // Suppress unused variable warning
        cleanup_profile(hProfile);
    }

    return 0;
}