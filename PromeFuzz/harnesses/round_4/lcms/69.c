// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    // Create a context for all operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Use first byte to decide which function to test
    if (Size > 0) {
        uint8_t selector = Data[0] % 6;
        const uint8_t* remaining_data = Data + 1;
        size_t remaining_size = Size - 1;

        switch (selector) {
            case 0: { // cmsCreateLab4ProfileTHR
                cmsCIExyY white_point;
                if (remaining_size >= sizeof(cmsCIExyY)) {
                    memcpy(&white_point, remaining_data, sizeof(cmsCIExyY));
                    cmsHPROFILE profile = cmsCreateLab4ProfileTHR(ctx, &white_point);
                    if (profile) {
                        cmsCloseProfile(profile);
                    }
                } else {
                    cmsHPROFILE profile = cmsCreateLab4ProfileTHR(ctx, NULL);
                    if (profile) {
                        cmsCloseProfile(profile);
                    }
                }
                break;
            }
            
            case 1: { // cmsCreateXYZProfileTHR
                cmsHPROFILE profile = cmsCreateXYZProfileTHR(ctx);
                if (profile) {
                    cmsCloseProfile(profile);
                }
                break;
            }
            
            case 2: { // cmsCreateLab2ProfileTHR
                cmsCIExyY white_point;
                if (remaining_size >= sizeof(cmsCIExyY)) {
                    memcpy(&white_point, remaining_data, sizeof(cmsCIExyY));
                    cmsHPROFILE profile = cmsCreateLab2ProfileTHR(ctx, &white_point);
                    if (profile) {
                        cmsCloseProfile(profile);
                    }
                } else {
                    cmsHPROFILE profile = cmsCreateLab2ProfileTHR(ctx, NULL);
                    if (profile) {
                        cmsCloseProfile(profile);
                    }
                }
                break;
            }
            
            case 3: { // cmsGetProfileContextID
                // First create a profile to get its context ID
                cmsHPROFILE profile = cmsCreateXYZProfileTHR(ctx);
                if (profile) {
                    cmsContext profile_ctx = cmsGetProfileContextID(profile);
                    // Context ID should match our created context
                    (void)profile_ctx; // Use void to avoid unused variable warning
                    cmsCloseProfile(profile);
                }
                break;
            }
            
            case 4: { // cmsOpenProfileFromFileTHR
                // Write fuzzer data to file
                FILE* fp = fopen("./dummy_file", "wb");
                if (fp) {
                    fwrite(Data, 1, Size, fp);
                    fclose(fp);
                    
                    // Try to open profile with read access
                    cmsHPROFILE profile = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "r");
                    if (profile) {
                        cmsCloseProfile(profile);
                    }
                    
                    // Clean up the dummy file
                    remove("./dummy_file");
                }
                break;
            }
            
            case 5: { // cmsCreateContext
                // Create a second context with the fuzzer data as user data
                void* user_data = NULL;
                if (remaining_size > 0) {
                    user_data = malloc(remaining_size);
                    if (user_data) {
                        memcpy(user_data, remaining_data, remaining_size);
                    }
                }
                
                cmsContext ctx2 = cmsCreateContext(NULL, user_data);
                if (ctx2) {
                    cmsDeleteContext(ctx2);
                }
                
                if (user_data) {
                    free(user_data);
                }
                break;
            }
        }
    }

    // Clean up the main context
    cmsDeleteContext(ctx);
    
    return 0;
}