// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DUMMY_FILE_PATH "./dummy_file"

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen(DUMMY_FILE_PATH, "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove(DUMMY_FILE_PATH);
}

static cmsHPROFILE create_dummy_profile_from_file(void) {
    return cmsOpenProfileFromFile(DUMMY_FILE_PATH, "r");
}

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create dummy file for profile loading
    create_dummy_file(Data, Size);
    
    // Test cmsGetAlarmCodes and cmsSetAlarmCodes
    cmsUInt16Number alarm_codes[cmsMAXCHANNELS];
    cmsUInt16Number new_alarm_codes[cmsMAXCHANNELS];
    
    // Initialize alarm codes with fuzz data
    for (int i = 0; i < cmsMAXCHANNELS && i * 2 + 1 < Size; i++) {
        new_alarm_codes[i] = (Data[i * 2] << 8) | Data[i * 2 + 1];
    }
    
    cmsSetAlarmCodes(new_alarm_codes);
    cmsGetAlarmCodes(alarm_codes);
    
    // Test context-based functions
    cmsContext ctx = create_test_context();
    if (ctx) {
        // Test cmsGetAlarmCodesTHR and cmsSetAlarmCodesTHR
        cmsSetAlarmCodesTHR(ctx, new_alarm_codes);
        cmsGetAlarmCodesTHR(ctx, alarm_codes);
        
        // Create dummy profiles for transform functions
        cmsHPROFILE dummy_profile = create_dummy_profile_from_file();
        
        if (dummy_profile) {
            // Extract parameters from fuzz data
            cmsUInt32Number input_format = (Size > 8) ? *((cmsUInt32Number*)Data + 1) : 0;
            cmsUInt32Number output_format = (Size > 12) ? *((cmsUInt32Number*)Data + 2) : 0;
            cmsUInt32Number intent = (Size > 16) ? *((cmsUInt32Number*)Data + 3) : 0;
            cmsUInt32Number proofing_intent = (Size > 20) ? *((cmsUInt32Number*)Data + 4) : 0;
            cmsUInt32Number flags = (Size > 24) ? *((cmsUInt32Number*)Data + 5) : 0;
            
            // Test cmsCreateTransformTHR
            cmsHTRANSFORM transform = cmsCreateTransformTHR(
                ctx,
                dummy_profile,
                input_format,
                dummy_profile,
                output_format,
                intent,
                flags
            );
            
            if (transform) {
                cmsDeleteTransform(transform);
            }
            
            // Test cmsCreateProofingTransformTHR
            transform = cmsCreateProofingTransformTHR(
                ctx,
                dummy_profile,
                input_format,
                dummy_profile,
                output_format,
                dummy_profile,
                intent,
                proofing_intent,
                flags
            );
            
            if (transform) {
                cmsDeleteTransform(transform);
            }
            
            cmsCloseProfile(dummy_profile);
        }
        
        cmsDeleteContext(ctx);
    }
    
    // Clean up
    cleanup_dummy_file();
    
    return 0;
}