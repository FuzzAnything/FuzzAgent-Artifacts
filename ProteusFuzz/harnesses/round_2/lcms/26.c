// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const unsigned char *)data, size);
    return hProfile;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hProfiles[255];
    cmsCIEXYZ BlackPoint = {0, 0, 0};
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number direction = 0;
    cmsUInt32Number inputFormat = 0;
    cmsUInt32Number outputFormat = 0;
    cmsHTRANSFORM hTransform = NULL;

    // Initialize intent and flags from input data to explore different states
    if (Size >= 1) {
        intent = Data[0] % 4; // Valid intents are 0-3
    }
    if (Size >= 2) {
        flags = Data[1];
    }
    if (Size >= 3) {
        direction = Data[2];
    }
    if (Size >= 4) {
        inputFormat = Data[3];
    }
    if (Size >= 5) {
        outputFormat = Data[4];
    }

    // Create a profile from the input data
    hProfile = create_profile_from_data(Data, Size);

    if (hProfile != NULL) {
        // Test cmsGetHeaderRenderingIntent
        cmsUInt32Number currentIntent = cmsGetHeaderRenderingIntent(hProfile);
        (void)currentIntent;

        // Test cmsSetHeaderRenderingIntent
        cmsSetHeaderRenderingIntent(hProfile, intent);

        // Test cmsIsIntentSupported
        cmsBool isSupported = cmsIsIntentSupported(hProfile, intent, direction);
        (void)isSupported;

        // Test cmsDetectBlackPoint
        cmsBool detected = cmsDetectBlackPoint(&BlackPoint, hProfile, intent, flags);
        (void)detected;

        // Test cmsDetectDestinationBlackPoint
        cmsBool destDetected = cmsDetectDestinationBlackPoint(&BlackPoint, hProfile, intent, flags);
        (void)destDetected;

        // Test cmsCreateMultiprofileTransform with a single profile
        hProfiles[0] = hProfile;
        hTransform = cmsCreateMultiprofileTransform(hProfiles, 1, inputFormat, outputFormat, intent, flags);
        if (hTransform != NULL) {
            cmsDeleteTransform(hTransform);
            hTransform = NULL;
        }

        // Test cmsCreateMultiprofileTransform with two profiles (if possible)
        if (Size >= 100) {
            cmsHPROFILE hProfile2 = create_profile_from_data(Data + 100, Size - 100);
            if (hProfile2 != NULL) {
                hProfiles[0] = hProfile;
                hProfiles[1] = hProfile2;
                hTransform = cmsCreateMultiprofileTransform(hProfiles, 2, inputFormat, outputFormat, intent, flags);
                if (hTransform != NULL) {
                    cmsDeleteTransform(hTransform);
                    hTransform = NULL;
                }
                cmsCloseProfile(hProfile2);
            }
        }

        // Cleanup
        cmsCloseProfile(hProfile);
    }

    return 0;
}