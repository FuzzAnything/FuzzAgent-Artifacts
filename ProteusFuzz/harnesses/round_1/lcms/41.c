// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_contexts(cmsContext ctx1, cmsContext ctx2, cmsHPROFILE prof1, cmsHPROFILE prof2) {
    if (prof1 != NULL) {
        cmsCloseProfile(prof1);
    }
    if (prof2 != NULL) {
        cmsCloseProfile(prof2);
    }
    if (ctx1 != NULL) {
        cmsDeleteContext(ctx1);
    }
    if (ctx2 != NULL) {
        cmsDeleteContext(ctx2);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsContext ctx_dup = NULL;
    cmsHPROFILE prof = NULL;
    cmsHPROFILE prof_dup = NULL;
    cmsUInt16Number alarmCodes[cmsMAXCHANNELS];
    void *userData = NULL;
    void *userData_dup = NULL;

    // Initialize alarm codes
    memset(alarmCodes, 0, sizeof(alarmCodes));

    // Use input data to seed random choices and values
    uint8_t choice = Data[0] % 4;
    uint8_t choice2 = Data[1] % 4;

    // Create a context based on input
    if (Size >= 4) {
        // Try to create a context with custom user data if size allows
        // Ensure we don't read past the buffer end
        size_t available_data = Size - 2;
        size_t user_data_size = (available_data > 100) ? 100 : available_data;
        
        if (user_data_size > 0) {
            userData = malloc(user_data_size);
            if (userData) {
                memcpy(userData, Data + 2, user_data_size);
                ctx = cmsDupContext(NULL, userData);
            }
        }
    }

    if (ctx == NULL) {
        ctx = cmsCreateContext(NULL, NULL);
    }

    if (ctx == NULL) {
        free(userData);
        return 0;
    }

    // Fuzz cmsCreate_sRGBProfileTHR
    if (ctx != NULL) {
        prof = cmsCreate_sRGBProfileTHR(ctx);
    }

    // Fuzz cmsSetAlarmCodesTHR
    if (ctx != NULL) {
        // Fill alarm codes with fuzz data safely
        size_t codes_size = sizeof(alarmCodes);
        size_t available_for_codes = (Size > 4) ? (Size - 4) : 0;
        size_t copy_len = (available_for_codes < codes_size) ? available_for_codes : codes_size;
        
        if (copy_len > 0) {
            memcpy(alarmCodes, Data + 4, copy_len);
        }
        cmsSetAlarmCodesTHR(ctx, alarmCodes);
    }

    // Fuzz cmsGetAlarmCodesTHR
    if (ctx != NULL) {
        cmsUInt16Number retrievedCodes[cmsMAXCHANNELS];
        memset(retrievedCodes, 0, sizeof(retrievedCodes));
        cmsGetAlarmCodesTHR(ctx, retrievedCodes);
    }

    // Fuzz cmsGetContextUserData
    if (ctx != NULL) {
        void *retrievedUserData = cmsGetContextUserData(ctx);
        (void)retrievedUserData; // Use to avoid unused variable warning
    }

    // Fuzz cmsDupContext
    if (ctx != NULL) {
        if (Size >= 10) {
            size_t available_for_dup = Size - 10;
            size_t new_user_data_size = (available_for_dup > 200) ? 200 : available_for_dup;
            
            if (new_user_data_size > 0) {
                userData_dup = malloc(new_user_data_size);
                if (userData_dup) {
                    memcpy(userData_dup, Data + 10, new_user_data_size);
                    ctx_dup = cmsDupContext(ctx, userData_dup);
                }
            } else {
                ctx_dup = cmsDupContext(ctx, NULL);
            }
        } else {
            ctx_dup = cmsDupContext(ctx, NULL);
        }
    }

    if (ctx_dup != NULL) {
        prof_dup = cmsCreate_sRGBProfileTHR(ctx_dup);
        
        // Set and get alarm codes on duplicated context
        cmsSetAlarmCodesTHR(ctx_dup, alarmCodes);
        cmsUInt16Number dupCodes[cmsMAXCHANNELS];
        memset(dupCodes, 0, sizeof(dupCodes));
        cmsGetAlarmCodesTHR(ctx_dup, dupCodes);
        
        // Get user data from duplicated context
        void *dupUserData = cmsGetContextUserData(ctx_dup);
        (void)dupUserData;
    }

    // Cleanup
    cleanup_contexts(ctx, ctx_dup, prof, prof_dup);
    free(userData);
    free(userData_dup);

    return 0;
}