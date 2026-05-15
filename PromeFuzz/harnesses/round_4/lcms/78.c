// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    return cmsOpenProfileFromMem(data, size);
}

static cmsHTRANSFORM create_dummy_transform(void) {
    cmsHPROFILE src = cmsCreate_sRGBProfile();
    cmsHPROFILE dst = cmsCreate_sRGBProfile();
    cmsHTRANSFORM transform = cmsCreateTransform(src, TYPE_RGB_8, dst, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    cmsCloseProfile(src);
    cmsCloseProfile(dst);
    return transform;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create some profiles from fuzz data
    cmsHPROFILE profile1 = create_profile_from_data(Data, Size);
    cmsHPROFILE profile2 = create_profile_from_data(Data + Size/2, Size/2);
    
    // Create a dummy transform for testing
    cmsHTRANSFORM dummy_transform = create_dummy_transform();
    
    // Test cmsTransform2DeviceLink
    if (dummy_transform) {
        cmsHPROFILE devicelink = cmsTransform2DeviceLink(dummy_transform, 4.0, 0);
        if (devicelink) cmsCloseProfile(devicelink);
    }
    
    // Test cmsCreateProofingTransformTHR
    if (profile1 && profile2) {
        cmsHTRANSFORM proofing_transform = cmsCreateProofingTransformTHR(
            NULL,
            profile1,
            TYPE_RGB_8,
            profile2,
            TYPE_RGB_8,
            profile1,
            INTENT_PERCEPTUAL,
            INTENT_RELATIVE_COLORIMETRIC,
            cmsFLAGS_SOFTPROOFING
        );
        if (proofing_transform) cmsDeleteTransform(proofing_transform);
    }
    
    // Test cmsDetectTAC
    if (profile1) {
        double tac = cmsDetectTAC(profile1);
        (void)tac; // Use result to avoid optimization
    }
    
    // Test cmsCreateXYZProfile
    cmsHPROFILE xyz_profile = cmsCreateXYZProfile();
    if (xyz_profile) cmsCloseProfile(xyz_profile);
    
    // Test cmsCreateTransform
    if (profile1 && profile2) {
        cmsHTRANSFORM transform = cmsCreateTransform(
            profile1,
            TYPE_RGB_8,
            profile2,
            TYPE_RGB_8,
            INTENT_PERCEPTUAL,
            0
        );
        if (transform) cmsDeleteTransform(transform);
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    if (profile1 && profile2) {
        cmsHPROFILE profiles[2] = {profile1, profile2};
        cmsHTRANSFORM multi_transform = cmsCreateMultiprofileTransformTHR(
            NULL,
            profiles,
            2,
            TYPE_RGB_8,
            TYPE_RGB_8,
            INTENT_PERCEPTUAL,
            0
        );
        if (multi_transform) cmsDeleteTransform(multi_transform);
    }
    
    // Cleanup
    if (dummy_transform) cmsDeleteTransform(dummy_transform);
    if (profile1) cmsCloseProfile(profile1);
    if (profile2) cmsCloseProfile(profile2);
    
    return 0;
}