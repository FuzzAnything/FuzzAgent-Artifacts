// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform != NULL) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_context(cmsContext context) {
    if (context != NULL) {
        cmsDeleteContext(context);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Create a context for thread-safe operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Create profile from memory
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, Data, (cmsUInt32Number)Size);
    if (profile == NULL) {
        // If we can't create a profile from the input, try with a dummy file
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp != NULL) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            // Try to open from file
            profile = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
            remove("./dummy_file");
        }
    }
    
    if (profile == NULL) {
        // If still no profile, create a default one for testing
        profile = cmsCreate_sRGBProfileTHR(context);
    }
    
    if (profile == NULL) {
        cleanup_context(context);
        return 0;
    }
    
    // Prepare arrays for extended transform
    cmsUInt32Number nProfiles = 2;
    cmsHPROFILE hProfiles[2] = {profile, profile};
    cmsBool BPC[2] = {FALSE, FALSE};
    cmsUInt32Number Intents[2] = {INTENT_PERCEPTUAL, INTENT_PERCEPTUAL};
    cmsFloat64Number AdaptationStates[2] = {1.0, 1.0};
    
    // Test cmsCreateExtendedTransform
    cmsHTRANSFORM transform1 = cmsCreateExtendedTransform(
        context,
        nProfiles,
        hProfiles,
        BPC,
        Intents,
        AdaptationStates,
        NULL,  // hGamutProfile
        0,     // nGamutPCSposition
        TYPE_RGB_8,   // InputFormat
        TYPE_RGB_8,   // OutputFormat
        0      // dwFlags
    );
    cleanup_transform(transform1);
    
    // Test cmsCreateTransform
    cmsHTRANSFORM transform2 = cmsCreateTransform(
        profile,      // Input
        TYPE_RGB_8,   // InputFormat
        profile,      // Output
        TYPE_RGB_8,   // OutputFormat
        INTENT_PERCEPTUAL,  // Intent
        0             // dwFlags
    );
    cleanup_transform(transform2);
    
    // Test cmsCreateMultiprofileTransform
    cmsHTRANSFORM transform3 = cmsCreateMultiprofileTransform(
        hProfiles,
        nProfiles,
        TYPE_RGB_8,   // InputFormat
        TYPE_RGB_8,   // OutputFormat
        INTENT_PERCEPTUAL,  // Intent
        0             // dwFlags
    );
    cleanup_transform(transform3);
    
    // Test cmsCreateMultiprofileTransformTHR
    cmsHTRANSFORM transform4 = cmsCreateMultiprofileTransformTHR(
        context,
        hProfiles,
        nProfiles,
        TYPE_RGB_8,   // InputFormat
        TYPE_RGB_8,   // OutputFormat
        INTENT_PERCEPTUAL,  // Intent
        0             // dwFlags
    );
    cleanup_transform(transform4);
    
    // Test cmsCreateTransformTHR
    cmsHTRANSFORM transform5 = cmsCreateTransformTHR(
        context,
        profile,      // Input
        TYPE_RGB_8,   // InputFormat
        profile,      // Output
        TYPE_RGB_8,   // OutputFormat
        INTENT_PERCEPTUAL,  // Intent
        0             // dwFlags
    );
    cleanup_transform(transform5);
    
    // Test with NULL output (device link)
    cmsHTRANSFORM transform6 = cmsCreateTransformTHR(
        context,
        profile,      // Input
        TYPE_RGB_8,   // InputFormat
        NULL,         // Output (device link)
        TYPE_RGB_8,   // OutputFormat
        INTENT_PERCEPTUAL,  // Intent
        0             // dwFlags
    );
    cleanup_transform(transform6);
    
    // Test with different formats - only use standard lcms types
    cmsUInt32Number formats[] = {
        TYPE_RGB_8, TYPE_RGB_16, TYPE_RGB_FLT,
        TYPE_CMYK_8, TYPE_CMYK_16, TYPE_CMYK_FLT,
        TYPE_Lab_8, TYPE_Lab_16, TYPE_Lab_FLT
    };
    int num_formats = sizeof(formats) / sizeof(formats[0]);
    
    for (int i = 0; i < 3 && i < num_formats; i++) {
        for (int j = 0; j < 3 && j < num_formats; j++) {
            cmsHTRANSFORM transform7 = cmsCreateTransformTHR(
                context,
                profile,
                formats[i],
                profile,
                formats[j],
                INTENT_PERCEPTUAL,
                0
            );
            cleanup_transform(transform7);
        }
    }
    
    // Test with different intents
    cmsUInt32Number intents[] = {
        INTENT_PERCEPTUAL,
        INTENT_RELATIVE_COLORIMETRIC,
        INTENT_SATURATION,
        INTENT_ABSOLUTE_COLORIMETRIC
    };
    int num_intents = sizeof(intents) / sizeof(intents[0]);
    
    for (int i = 0; i < num_intents; i++) {
        cmsHTRANSFORM transform8 = cmsCreateTransformTHR(
            context,
            profile,
            TYPE_RGB_8,
            profile,
            TYPE_RGB_8,
            intents[i],
            0
        );
        cleanup_transform(transform8);
    }
    
    // Test with different flags
    cmsUInt32Number flags[] = {
        0,
        cmsFLAGS_NOCACHE,
        cmsFLAGS_NOOPTIMIZE,
        cmsFLAGS_NULLTRANSFORM,
        cmsFLAGS_GAMUTCHECK,
        cmsFLAGS_SOFTPROOFING
    };
    int num_flags = sizeof(flags) / sizeof(flags[0]);
    
    for (int i = 0; i < num_flags; i++) {
        cmsHTRANSFORM transform9 = cmsCreateTransformTHR(
            context,
            profile,
            TYPE_RGB_8,
            profile,
            TYPE_RGB_8,
            INTENT_PERCEPTUAL,
            flags[i]
        );
        cleanup_transform(transform9);
    }
    
    // Cleanup
    cleanup_profile(profile);
    cleanup_context(context);
    
    return 0;
}