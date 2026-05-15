// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DUMMY_FILE "./dummy_file"

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

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t *data, size_t size) {
    FILE *fp = fopen(DUMMY_FILE, "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile(DUMMY_FILE, "r");
    remove(DUMMY_FILE);
    return profile;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size < 64) return 0;
    
    // Initialize context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Prepare alarm codes
    cmsUInt16Number alarmCodes[cmsMAXCHANNELS];
    cmsUInt16Number newAlarm[cmsMAXCHANNELS];
    for (int i = 0; i < cmsMAXCHANNELS; i++) {
        alarmCodes[i] = (Data[i * 2] << 8) | Data[i * 2 + 1];
    }
    
    // Test per-context alarm codes
    cmsSetAlarmCodesTHR(context, alarmCodes);
    cmsGetAlarmCodesTHR(context, newAlarm);
    
    // Test global alarm codes
    cmsSetAlarmCodes(alarmCodes);
    cmsGetAlarmCodes(newAlarm);
    
    // Create dummy profiles for transform testing
    size_t profile_size = Size / 4;
    cmsHPROFILE input_profile = create_dummy_profile_from_data(Data, profile_size);
    cmsHPROFILE output_profile = create_dummy_profile_from_data(Data + profile_size, profile_size);
    cmsHPROFILE proofing_profile = create_dummy_profile_from_data(Data + 2 * profile_size, profile_size);
    
    if (input_profile && output_profile && proofing_profile) {
        // Extract parameters from fuzzer data
        size_t offset = 3 * profile_size;
        if (Size - offset >= 20) {
            cmsUInt32Number input_format = *(cmsUInt32Number*)(Data + offset);
            cmsUInt32Number output_format = *(cmsUInt32Number*)(Data + offset + 4);
            cmsUInt32Number intent = *(cmsUInt32Number*)(Data + offset + 8);
            cmsUInt32Number proofing_intent = *(cmsUInt32Number*)(Data + offset + 12);
            cmsUInt32Number flags = *(cmsUInt32Number*)(Data + offset + 16);
            
            // Test proofing transform
            cmsHTRANSFORM proof_transform = cmsCreateProofingTransformTHR(
                context,
                input_profile,
                input_format,
                output_profile,
                output_format,
                proofing_profile,
                intent,
                proofing_intent,
                flags
            );
            cleanup_transform(proof_transform);
            
            // Test regular transform
            cmsHTRANSFORM regular_transform = cmsCreateTransformTHR(
                context,
                input_profile,
                input_format,
                output_profile,
                output_format,
                intent,
                flags
            );
            cleanup_transform(regular_transform);
            
            // Test with NULL output (device link)
            cmsHTRANSFORM device_link = cmsCreateTransformTHR(
                context,
                input_profile,
                input_format,
                NULL,
                output_format,
                intent,
                flags
            );
            cleanup_transform(device_link);
        }
    }
    
    // Cleanup
    cleanup_profile(input_profile);
    cleanup_profile(output_profile);
    cleanup_profile(proofing_profile);
    cmsDeleteContext(context);
    
    return 0;
}