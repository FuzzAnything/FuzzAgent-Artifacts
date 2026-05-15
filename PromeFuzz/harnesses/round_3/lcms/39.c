// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Create a dummy ICC profile from fuzzer input
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        // If we can't create a profile from input, use a default profile
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            return 0;
        }
    }

    // Test cmsGetHeaderRenderingIntent
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);

    // Test cmsSetHeaderRenderingIntent
    // Use the intent we just got, or derive from input size
    cmsUInt32Number newIntent = (Size > 0) ? (Data[0] % 4) : intent;
    cmsSetHeaderRenderingIntent(hProfile, newIntent);

    // Test cmsGetSupportedIntents
    cmsUInt32Number count = cmsGetSupportedIntents(0, NULL, NULL);
    if (count > 0) {
        cmsUInt32Number* codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
        char** descriptions = (char**)malloc(count * sizeof(char*));
        if (codes && descriptions) {
            cmsGetSupportedIntents(count, codes, descriptions);
            // The descriptions array contains pointers to static strings
            // owned by the library, so we should NOT free them individually
            free(descriptions);
            free(codes);
        }
    }

    // Test cmsGetSupportedIntentsTHR
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        count = cmsGetSupportedIntentsTHR(context, 0, NULL, NULL);
        if (count > 0) {
            cmsUInt32Number* codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
            char** descriptions = (char**)malloc(count * sizeof(char*));
            if (codes && descriptions) {
                cmsGetSupportedIntentsTHR(context, count, codes, descriptions);
                // The descriptions array contains pointers to static strings
                // owned by the library, so we should NOT free them individually
                free(descriptions);
                free(codes);
            }
        }
    }

    // Test cmsGetPostScriptCRD
    cmsUInt32Number bufferSize = 0;
    if (context) {
        // First get required buffer size
        bufferSize = cmsGetPostScriptCRD(context, hProfile, intent, 0, NULL, 0);
        if (bufferSize > 0 && bufferSize < 1024*1024) { // Limit to 1MB to avoid OOM
            void* buffer = malloc(bufferSize);
            if (buffer) {
                cmsGetPostScriptCRD(context, hProfile, intent, 0, buffer, bufferSize);
                free(buffer);
            }
        }
    }

    // Test cmsCreateTransform
    cmsHPROFILE outputProfile = cmsCreate_sRGBProfile();
    if (outputProfile) {
        cmsUInt32Number inputFormat = (Size > 1) ? (Data[1] % 8) : TYPE_RGB_8;
        cmsUInt32Number outputFormat = (Size > 2) ? (Data[2] % 8) : TYPE_RGB_8;
        cmsUInt32Number transformIntent = (Size > 3) ? (Data[3] % 4) : intent;
        cmsUInt32Number flags = (Size > 4) ? Data[4] : 0;
        
        cmsHTRANSFORM transform = cmsCreateTransform(hProfile, inputFormat, 
                                                     outputProfile, outputFormat, 
                                                     transformIntent, flags);
        if (transform) {
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(outputProfile);
    }

    // Cleanup
    if (context) {
        cmsDeleteContext(context);
    }
    cmsCloseProfile(hProfile);
    
    return 0;
}