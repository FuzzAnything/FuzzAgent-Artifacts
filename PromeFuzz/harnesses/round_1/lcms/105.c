// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_iohandler(cmsIOHANDLER* io) {
    if (io != NULL) {
        cmsCloseIOhandler(io);
    }
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to dummy file
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Test cmsfilelength
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file) {
        long int file_len = cmsfilelength(dummy_file);
        (void)file_len; // Use result to avoid unused variable warning
        fclose(dummy_file);
    }
    
    // Test cmsOpenIOhandlerFromFile
    cmsContext context = cmsCreateContext(NULL, NULL);
    cmsIOHANDLER* io1 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    cleanup_iohandler(io1);
    
    // Test cmsOpenProfileFromStream and cmsOpenProfileFromStreamTHR
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file) {
        cmsHPROFILE profile1 = cmsOpenProfileFromStream(dummy_file, "r");
        cleanup_profile(profile1);
        
        fseek(dummy_file, 0, SEEK_SET);
        cmsHPROFILE profile2 = cmsOpenProfileFromStreamTHR(context, dummy_file, "r");
        cleanup_profile(profile2);
        
        fclose(dummy_file);
    }
    
    // Test cmsOpenIOhandlerFromStream
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file) {
        cmsIOHANDLER* io2 = cmsOpenIOhandlerFromStream(context, dummy_file);
        // Note: We don't close the file here because cmsOpenIOhandlerFromStream takes ownership
        // We only close the I/O handler
        cleanup_iohandler(io2);
    } else {
        // If file opening failed, we still need to close the file pointer
        if (dummy_file) {
            fclose(dummy_file);
        }
    }
    
    // Test cmsSaveProfileToStream
    // Create a simple sRGB profile to test saving
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfile();
    if (srgb_profile) {
        dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            cmsBool save_result = cmsSaveProfileToStream(srgb_profile, dummy_file);
            (void)save_result; // Use result to avoid unused variable warning
            fclose(dummy_file);
        }
        cleanup_profile(srgb_profile);
    }
    
    // Cleanup context
    cmsDeleteContext(context);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}