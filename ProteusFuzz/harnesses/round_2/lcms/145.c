// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void safe_cleanup(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    // Minimum size required for the most complex operation (cmsxyY2XYZ needs 3 doubles + overhead)
    // We will check bounds for each specific operation individually.
    if (Size < 2) {
        return 0;
    }

    // Initialize basic structures
    cmsCIEXYZ xyz_out = {0.0, 0.0, 0.0};
    cmsCIEXYZ xyz_in = {0.0, 0.0, 0.0};
    cmsCIExyY xyY = {0.0, 0.0, 0.0};
    cmsCIELab Lab = {0.0, 0.0, 0.0};
    cmsUInt16Number encoded_xyz[3] = {0, 0, 0};
    cmsUInt16Number decoded_xyz[3] = {0, 0, 0};
    cmsHPROFILE hProfile = NULL;
    cmsBool result;
    const cmsCIEXYZ *d50 = NULL;

    // Parse input data safely
    uint8_t *ptr = (uint8_t *)Data;
    uint32_t intent = (ptr[0] % 4); // 0 to 3
    uint32_t flags = (ptr[1] % 256);

    // Test cmsD50_XYZ
    d50 = cmsD50_XYZ();
    if (d50 != NULL) {
        xyz_in = *d50;
    }

    // Test cmsLab2XYZ
    // Requires 2 doubles (16 bytes) starting at offset 2
    if (Size >= 2 + 2 * sizeof(double)) {
        memcpy(&Lab.L, ptr + 2, sizeof(double));
        memcpy(&Lab.a, ptr + 2 + sizeof(double), sizeof(double));
        // b is not parsed, remains 0.0
        cmsLab2XYZ(d50, &xyz_out, &Lab);
    }

    // Test cmsXYZEncoded2Float
    // Requires 3 * sizeof(cmsUInt16Number) = 6 bytes starting at offset 16
    if (Size >= 16 + 6) {
        memcpy(encoded_xyz, ptr + 16, 6);
        cmsXYZEncoded2Float(&xyz_out, encoded_xyz);
    }

    // Test cmsFloat2XYZEncoded
    // Requires sizeof(cmsCIEXYZ) = 3 * sizeof(double) = 24 bytes starting at offset 20
    if (Size >= 20 + sizeof(cmsCIEXYZ)) {
        memcpy(&xyz_in, ptr + 20, sizeof(cmsCIEXYZ));
        cmsFloat2XYZEncoded(decoded_xyz, &xyz_in);
    }

    // Test cmsxyY2XYZ
    // Requires 3 doubles (24 bytes) starting at offset 26
    if (Size >= 26 + 3 * sizeof(double)) {
        memcpy(&xyY.x, ptr + 26, sizeof(double));
        memcpy(&xyY.y, ptr + 26 + sizeof(double), sizeof(double));
        memcpy(&xyY.Y, ptr + 26 + 2 * sizeof(double), sizeof(double));
        
        // Prevent division by zero in cmsxyY2XYZ
        if (xyY.y == 0.0) {
            xyY.y = 0.0001;
        }
        cmsxyY2XYZ(&xyz_out, &xyY);
    }

    // Test cmsDetectDestinationBlackPoint
    // We need enough data to write to a file and attempt to open it.
    // Let's use the whole buffer if it's large enough.
    if (Size >= 50) {
        FILE *f = fopen("./dummy_file", "wb");
        if (f != NULL) {
            fwrite(ptr, 1, Size, f);
            fclose(f);

            hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
            
            if (hProfile != NULL) {
                result = cmsDetectDestinationBlackPoint(&xyz_out, hProfile, intent, flags);
                safe_cleanup(hProfile);
            }
        }
    }

    return 0;
}