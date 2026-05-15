// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROFILES 4
#define MAX_DATA_SIZE 4096

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile using standard primaries
    cmsToneCurve* gamma = cmsBuildGamma(ctx, 2.2);
    if (!gamma) return NULL;
    
    cmsToneCurve* curves[3] = {gamma, gamma, gamma};
    
    // Use standard sRGB primaries
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 0.212656},   // Red
        {0.3000, 0.6000, 0.715158},   // Green
        {0.1500, 0.0600, 0.072186}    // Blue
    };
    
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(ctx,
                                                 cmsD50_xyY(),
                                                 &primaries,
                                                 curves);
    
    cmsFreeToneCurve(gamma);
    return profile;
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Limit input size to prevent excessive memory usage
    if (Size > MAX_DATA_SIZE) Size = MAX_DATA_SIZE;
    
    // Create context for functions that require it
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy profiles
    cmsHPROFILE dummy_profiles[MAX_PROFILES];
    for (int i = 0; i < MAX_PROFILES; i++) {
        dummy_profiles[i] = create_dummy_profile(ctx);
        if (!dummy_profiles[i]) {
            for (int j = 0; j < i; j++) {
                cleanup_profile(dummy_profiles[j]);
            }
            cleanup_context(ctx);
            return 0;
        }
    }
    
    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    Data++;
    Size--;
    
    // Test cmsCreateExtendedTransform
    if (selector & 0x01) {
        cmsUInt32Number nProfiles = (Size > 0) ? (Data[0] % MAX_PROFILES) + 1 : 1;
        cmsBool BPC[MAX_PROFILES];
        cmsUInt32Number Intents[MAX_PROFILES];
        cmsFloat64Number AdaptationStates[MAX_PROFILES];
        
        for (cmsUInt32Number i = 0; i < nProfiles; i++) {
            BPC[i] = (Size > i + 1) ? (Data[i + 1] & 0x01) : 0;
            Intents[i] = (Size > i + 5) ? (Data[i + 5] % 4) : 0;
            AdaptationStates[i] = 1.0;
        }
        
        cmsUInt32Number InputFormat = (Size > 10) ? Data[10] : TYPE_RGB_8;
        cmsUInt32Number OutputFormat = (Size > 11) ? Data[11] : TYPE_RGB_8;
        cmsUInt32Number dwFlags = (Size > 12) ? *((cmsUInt32Number*)(Data + 12)) : 0;
        
        cmsHTRANSFORM transform = cmsCreateExtendedTransform(ctx,
                                                             nProfiles,
                                                             dummy_profiles,
                                                             BPC,
                                                             Intents,
                                                             AdaptationStates,
                                                             NULL, // hGamutProfile
                                                             0,    // nGamutPCSposition
                                                             InputFormat,
                                                             OutputFormat,
                                                             dwFlags);
        cleanup_transform(transform);
    }
    
    // Test cmsFormatterForColorspaceOfProfile
    if (selector & 0x02) {
        cmsUInt32Number nBytes = (Size > 0) ? ((Data[0] % 2) + 1) : 1;
        cmsBool lIsFloat = (Size > 1) ? (Data[1] & 0x01) : 0;
        
        cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(dummy_profiles[0],
                                                                       nBytes,
                                                                       lIsFloat);
        (void)formatter; // Use result to avoid unused variable warning
    }
    
    // Test cmsCreateProofingTransformTHR
    if (selector & 0x04) {
        cmsUInt32Number InputFormat = (Size > 0) ? Data[0] : TYPE_RGB_8;
        cmsUInt32Number OutputFormat = (Size > 1) ? Data[1] : TYPE_RGB_8;
        cmsUInt32Number Intent = (Size > 2) ? (Data[2] % 4) : 0;
        cmsUInt32Number ProofingIntent = (Size > 3) ? (Data[3] % 4) : 0;
        cmsUInt32Number dwFlags = (Size > 4) ? *((cmsUInt32Number*)(Data + 4)) : 0;
        
        cmsHTRANSFORM transform = cmsCreateProofingTransformTHR(ctx,
                                                                dummy_profiles[0],
                                                                InputFormat,
                                                                dummy_profiles[1],
                                                                OutputFormat,
                                                                dummy_profiles[2],
                                                                Intent,
                                                                ProofingIntent,
                                                                dwFlags);
        cleanup_transform(transform);
    }
    
    // Test cmsFormatterForPCSOfProfile
    if (selector & 0x08) {
        cmsUInt32Number nBytes = (Size > 0) ? ((Data[0] % 2) + 1) : 1;
        cmsBool lIsFloat = (Size > 1) ? (Data[1] & 0x01) : 0;
        
        cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(dummy_profiles[0],
                                                                nBytes,
                                                                lIsFloat);
        (void)formatter; // Use result to avoid unused variable warning
    }
    
    // Test cmsCreateMultiprofileTransform
    if (selector & 0x10) {
        cmsUInt32Number nProfiles = (Size > 0) ? (Data[0] % MAX_PROFILES) + 1 : 1;
        cmsUInt32Number InputFormat = (Size > 1) ? Data[1] : TYPE_RGB_8;
        cmsUInt32Number OutputFormat = (Size > 2) ? Data[2] : TYPE_RGB_8;
        cmsUInt32Number Intent = (Size > 3) ? (Data[3] % 4) : 0;
        cmsUInt32Number dwFlags = (Size > 4) ? *((cmsUInt32Number*)(Data + 4)) : 0;
        
        cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(dummy_profiles,
                                                                 nProfiles,
                                                                 InputFormat,
                                                                 OutputFormat,
                                                                 Intent,
                                                                 dwFlags);
        cleanup_transform(transform);
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    if (selector & 0x20) {
        cmsUInt32Number nProfiles = (Size > 0) ? (Data[0] % MAX_PROFILES) + 1 : 1;
        cmsUInt32Number InputFormat = (Size > 1) ? Data[1] : TYPE_RGB_8;
        cmsUInt32Number OutputFormat = (Size > 2) ? Data[2] : TYPE_RGB_8;
        cmsUInt32Number Intent = (Size > 3) ? (Data[3] % 4) : 0;
        cmsUInt32Number dwFlags = (Size > 4) ? *((cmsUInt32Number*)(Data + 4)) : 0;
        
        cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(ctx,
                                                                    dummy_profiles,
                                                                    nProfiles,
                                                                    InputFormat,
                                                                    OutputFormat,
                                                                    Intent,
                                                                    dwFlags);
        cleanup_transform(transform);
    }
    
    // Cleanup
    for (int i = 0; i < MAX_PROFILES; i++) {
        cleanup_profile(dummy_profiles[i]);
    }
    cleanup_context(ctx);
    
    return 0;
}