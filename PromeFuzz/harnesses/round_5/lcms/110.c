// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen(DUMMY_FILE, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove(DUMMY_FILE);
}

static cmsHPROFILE create_profile_from_input(const uint8_t *data, size_t size) {
    write_dummy_file(data, size);
    cmsHPROFILE profile = cmsOpenProfileFromFile(DUMMY_FILE, "r");
    remove_dummy_file();
    return profile;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *data, size_t size) {
    if (size < 4) return 0;

    // Initialize alarm codes from fuzzer data
    cmsUInt16Number alarmCodes[cmsMAXCHANNELS];
    size_t alarmBytes = sizeof(cmsUInt16Number) * cmsMAXCHANNELS;
    if (size >= alarmBytes) {
        memcpy(alarmCodes, data, alarmBytes);
    } else {
        memset(alarmCodes, 0, alarmBytes);
        memcpy(alarmCodes, data, size);
    }

    // Test global alarm code functions
    cmsSetAlarmCodes(alarmCodes);
    
    cmsUInt16Number retrievedAlarmCodes[cmsMAXCHANNELS];
    cmsGetAlarmCodes(retrievedAlarmCodes);

    // Test context-based alarm code functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        cmsSetAlarmCodesTHR(context, alarmCodes);
        cmsGetAlarmCodesTHR(context, retrievedAlarmCodes);
        
        // Create dummy profiles for transform testing
        cmsHPROFILE inputProfile = create_profile_from_input(data, size);
        cmsHPROFILE outputProfile = create_profile_from_input(data + (size/2), size/2);
        cmsHPROFILE proofingProfile = create_profile_from_input(data + (size/3), size/3);
        
        if (inputProfile && outputProfile && proofingProfile) {
            // Extract parameters from fuzzer data
            size_t offset = alarmBytes % size;
            cmsUInt32Number inputFormat = *(cmsUInt32Number*)(data + offset) % 0xFFFF;
            offset = (offset + 4) % size;
            cmsUInt32Number outputFormat = *(cmsUInt32Number*)(data + offset) % 0xFFFF;
            offset = (offset + 4) % size;
            cmsUInt32Number intent = *(cmsUInt32Number*)(data + offset) % 4;
            offset = (offset + 4) % size;
            cmsUInt32Number proofingIntent = *(cmsUInt32Number*)(data + offset) % 4;
            offset = (offset + 4) % size;
            cmsUInt32Number flags = *(cmsUInt32Number*)(data + offset);
            
            // Test proofing transform creation
            cmsHTRANSFORM proofingTransform = cmsCreateProofingTransformTHR(
                context,
                inputProfile,
                inputFormat,
                outputProfile,
                outputFormat,
                proofingProfile,
                intent,
                proofingIntent,
                flags
            );
            
            if (proofingTransform) {
                cmsDeleteTransform(proofingTransform);
            }
            
            // Test regular transform creation
            cmsHTRANSFORM regularTransform = cmsCreateTransformTHR(
                context,
                inputProfile,
                inputFormat,
                outputProfile,
                outputFormat,
                intent,
                flags
            );
            
            if (regularTransform) {
                cmsDeleteTransform(regularTransform);
            }
            
            // Test with NULL output profile (device link)
            cmsHTRANSFORM deviceLink = cmsCreateTransformTHR(
                context,
                inputProfile,
                inputFormat,
                NULL,
                outputFormat,
                intent,
                flags
            );
            
            if (deviceLink) {
                cmsDeleteTransform(deviceLink);
            }
        }
        
        // Cleanup profiles
        if (inputProfile) cmsCloseProfile(inputProfile);
        if (outputProfile) cmsCloseProfile(outputProfile);
        if (proofingProfile) cmsCloseProfile(proofingProfile);
        
        cmsDeleteContext(context);
    }
    
    return 0;
}