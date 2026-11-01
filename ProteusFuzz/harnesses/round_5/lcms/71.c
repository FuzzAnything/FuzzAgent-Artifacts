// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_context(cmsContext context) {
    if (context) {
        cmsDeleteContext(context);
    }
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;

    cmsHPROFILE profile = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsHTRANSFORM transform2 = NULL;
    cmsHTRANSFORM transform3 = NULL;
    cmsHTRANSFORM transform4 = NULL;

    // Try to open a profile from memory
    profile = cmsOpenProfileFromMemTHR(context, Data, Size);

    // Test cmsSignalError with a valid error code (cmsERROR_UNKNOWN is not standard, use 0 or a known code)
    // Using 0 as a generic error code for testing
    cmsSignalError(context, 0, "Test error message");

    // Test cmsCreateTransformTHR with single profile (Output = NULL)
    if (profile) {
        // Use explicit casts for types to avoid syntax errors
        transform = cmsCreateTransformTHR(context, profile, (cmsUInt32Number)0, NULL, (cmsUInt32Number)0, 0, 0);
    }

    // Test cmsCreateMultiprofileTransform
    if (profile && Size >= 2) {
        cmsHPROFILE profiles[2] = {profile, NULL};
        // Try with one valid profile
        transform2 = cmsCreateMultiprofileTransform(profiles, 1, (cmsUInt32Number)0, (cmsUInt32Number)0, 0, 0);
        
        // Try to create a second profile from remaining data if possible
        if (Size > 100) {
            cmsHPROFILE profile2 = cmsOpenProfileFromMemTHR(context, Data + 100, Size - 100);
            if (profile2) {
                profiles[1] = profile2;
                transform2 = cmsCreateMultiprofileTransform(profiles, 2, (cmsUInt32Number)0, (cmsUInt32Number)0, 0, 0);
                cleanup_profile(profile2);
            }
        }
    }

    // Test cmsCreateProofingTransformTHR
    if (profile && Size >= 200) {
        cmsHPROFILE proofProfile = cmsOpenProfileFromMemTHR(context, Data + 100, Size - 100);
        if (proofProfile) {
            transform3 = cmsCreateProofingTransformTHR(context, profile, (cmsUInt32Number)0, profile, (cmsUInt32Number)0, proofProfile, 0, 0, 0);
            cleanup_profile(proofProfile);
        }
    }

    // Test cmsCreateExtendedTransform
    if (profile) {
        cmsHPROFILE hProfiles[1] = {profile};
        cmsBool BPC[1] = {0}; // Use 0 instead of cmsFALSE
        cmsUInt32Number Intents[1] = {0};
        cmsFloat64Number AdaptationStates[1] = {0.0};
        transform4 = cmsCreateExtendedTransform(context, 1, hProfiles, BPC, Intents, AdaptationStates, NULL, 0, (cmsUInt32Number)0, (cmsUInt32Number)0, 0);
    }

    // Cleanup
    cleanup_transform(transform);
    cleanup_transform(transform2);
    cleanup_transform(transform3);
    cleanup_transform(transform4);
    cleanup_profile(profile);
    cleanup_context(context);

    return 0;
}