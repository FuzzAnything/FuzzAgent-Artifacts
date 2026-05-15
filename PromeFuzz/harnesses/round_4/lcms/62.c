// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile as a dummy
    return cmsCreate_sRGBProfileTHR(ctx);
}

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Create dummy profiles
    cmsHPROFILE dummy_profiles[4];
    for (int i = 0; i < 4; i++) {
        dummy_profiles[i] = create_dummy_profile(ctx);
        if (!dummy_profiles[i]) {
            for (int j = 0; j < i; j++) {
                cmsCloseProfile(dummy_profiles[j]);
            }
            cmsDeleteContext(ctx);
            return 0;
        }
    }

    // Extract parameters from fuzzed input
    size_t offset = 0;
    
    // For _cmsDefaultICCintents
    cmsUInt32Number nProfiles = (Size > offset) ? (Data[offset++] % 4) + 1 : 1;
    if (nProfiles > 4) nProfiles = 4;
    
    cmsUInt32Number intents[4];
    cmsBool BPC[4];
    cmsFloat64Number AdaptationStates[4];
    
    for (cmsUInt32Number i = 0; i < nProfiles; i++) {
        if (offset < Size) {
            intents[i] = Data[offset++] % 4; // 4 standard intents
        } else {
            intents[i] = 0;
        }
        
        if (offset < Size) {
            BPC[i] = Data[offset++] % 2;
        } else {
            BPC[i] = 0;
        }
        
        AdaptationStates[i] = 1.0; // Default value
    }
    
    cmsUInt32Number dwFlags = 0;
    if (offset + 4 <= Size) {
        dwFlags = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
    }

    // Test _cmsDefaultICCintents
    cmsPipeline* pipeline = _cmsDefaultICCintents(ctx, nProfiles, intents, 
                                                  dummy_profiles, BPC, 
                                                  AdaptationStates, dwFlags);
    cleanup_pipeline(pipeline);

    // Test cmsGetSupportedIntents
    cmsUInt32Number count = cmsGetSupportedIntents(0, NULL, NULL);
    if (count > 0) {
        cmsUInt32Number* codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
        char** descriptions = (char**)malloc(count * sizeof(char*));
        if (codes && descriptions) {
            cmsUInt32Number actual_count = cmsGetSupportedIntents(count, codes, descriptions);
            // According to lcms documentation, descriptions are static strings
            // and should NOT be freed individually
            free(descriptions);
            free(codes);
        }
    }

    // Test cmsGetSupportedIntentsTHR
    count = cmsGetSupportedIntentsTHR(ctx, 0, NULL, NULL);
    if (count > 0) {
        cmsUInt32Number* codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
        char** descriptions = (char**)malloc(count * sizeof(char*));
        if (codes && descriptions) {
            cmsUInt32Number actual_count = cmsGetSupportedIntentsTHR(ctx, count, codes, descriptions);
            // According to lcms documentation, descriptions are static strings
            // and should NOT be freed individually
            free(descriptions);
            free(codes);
        }
    }

    // Test cmsIsIntentSupported
    for (int i = 0; i < 4; i++) {
        cmsUInt32Number intent = (Size > offset) ? Data[offset++] % 4 : 0;
        cmsUInt32Number direction = (Size > offset) ? Data[offset++] % 2 : 0;
        cmsIsIntentSupported(dummy_profiles[i], intent, direction);
    }

    // Test cmsCreateTransform
    cmsUInt32Number inputFormat = (Size > offset) ? Data[offset++] : 0;
    cmsUInt32Number outputFormat = (Size > offset) ? Data[offset++] : 0;
    cmsUInt32Number intent = (Size > offset) ? Data[offset++] % 4 : 0;
    dwFlags = 0;
    if (offset + 4 <= Size) {
        dwFlags = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
    }
    
    cmsHTRANSFORM transform = cmsCreateTransform(dummy_profiles[0], inputFormat,
                                                dummy_profiles[1], outputFormat,
                                                intent, dwFlags);
    cleanup_transform(transform);

    // Test cmsCreateProofingTransform
    inputFormat = (Size > offset) ? Data[offset++] : 0;
    outputFormat = (Size > offset) ? Data[offset++] : 0;
    intent = (Size > offset) ? Data[offset++] % 4 : 0;
    cmsUInt32Number proofingIntent = (Size > offset) ? Data[offset++] % 4 : 0;
    dwFlags = 0;
    if (offset + 4 <= Size) {
        dwFlags = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
    }
    
    transform = cmsCreateProofingTransform(dummy_profiles[0], inputFormat,
                                          dummy_profiles[1], outputFormat,
                                          dummy_profiles[2], intent,
                                          proofingIntent, dwFlags);
    cleanup_transform(transform);

    // Cleanup
    for (int i = 0; i < 4; i++) {
        cmsCloseProfile(dummy_profiles[i]);
    }
    cmsDeleteContext(ctx);

    return 0;
}