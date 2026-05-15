// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetSupportedIntents at cmscnvrt.c:1192:27 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    if (size > 0) {
        return cmsOpenProfileFromMem(data, size);
    }
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;

    // Create test profiles
    cmsHPROFILE profile1 = create_test_profile(Data, Size);
    cmsHPROFILE profile2 = cmsCreate_sRGBProfile();
    
    if (!profile1 || !profile2) {
        if (profile1) cmsCloseProfile(profile1);
        if (profile2) cmsCloseProfile(profile2);
        cmsDeleteContext(context);
        return 0;
    }

    // Test cmsGetHeaderRenderingIntent
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(profile1);
    (void)intent;

    // Test cmsGetSupportedIntents
    cmsUInt32Number count = cmsGetSupportedIntents(0, NULL, NULL);
    if (count > 0) {
        cmsUInt32Number *codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
        char **descriptions = (char**)malloc(count * sizeof(char*));
        
        if (codes && descriptions) {
            cmsGetSupportedIntents(count, codes, descriptions);
            
            // Clean up descriptions
            for (cmsUInt32Number i = 0; i < count; i++) {
                if (descriptions[i]) {
                    free(descriptions[i]);
                }
            }
        }
        
        free(codes);
        free(descriptions);
    }

    // Test cmsGetPostScriptCRD
    write_dummy_file(Data, Size);
    cmsUInt32Number required_size = cmsGetPostScriptCRD(context, profile1, 
                                                       intent, 0, NULL, 0);
    if (required_size > 0 && required_size < 1024*1024) { // Limit to 1MB
        void *buffer = malloc(required_size);
        if (buffer) {
            cmsGetPostScriptCRD(context, profile1, intent, 0, buffer, required_size);
            free(buffer);
        }
    }

    // Test cmsCreateTransform with various parameters
    cmsUInt32Number inputFormat = (Size > 0) ? Data[0] % 10 : 0;
    cmsUInt32Number outputFormat = (Size > 1) ? Data[1] % 10 : 0;
    cmsUInt32Number transformIntent = (Size > 2) ? Data[2] % 4 : 0;
    cmsUInt32Number flags = (Size > 3) ? Data[3] : 0;
    
    cmsHTRANSFORM transform = cmsCreateTransform(profile1, inputFormat,
                                                profile2, outputFormat,
                                                transformIntent, flags);
    if (transform) {
        cmsDeleteTransform(transform);
    }

    // Test cmsGetSupportedIntentsTHR
    count = cmsGetSupportedIntentsTHR(context, 0, NULL, NULL);
    if (count > 0) {
        cmsUInt32Number *codes = (cmsUInt32Number*)malloc(count * sizeof(cmsUInt32Number));
        char **descriptions = (char**)malloc(count * sizeof(char*));
        
        if (codes && descriptions) {
            cmsGetSupportedIntentsTHR(context, count, codes, descriptions);
            
            // Clean up descriptions
            for (cmsUInt32Number i = 0; i < count; i++) {
                if (descriptions[i]) {
                    free(descriptions[i]);
                }
            }
        }
        
        free(codes);
        free(descriptions);
    }

    // Test cmsSetHeaderRenderingIntent
    cmsUInt32Number new_intent = (Size > 4) ? Data[4] % 4 : 0;
    cmsSetHeaderRenderingIntent(profile1, new_intent);

    // Cleanup
    cmsCloseProfile(profile1);
    cmsCloseProfile(profile2);
    cmsDeleteContext(context);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}