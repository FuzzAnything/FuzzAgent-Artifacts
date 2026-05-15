// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_iohandler(cmsIOHANDLER* ioHandler) {
    if (ioHandler != NULL) {
        // Note: cmsCloseIOhandler is not in the provided API list,
        // but typically I/O handlers need to be closed.
        // We'll rely on the library's internal cleanup for now.
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to a dummy file for file-based operations
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Create a context for context-aware functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Test cmsOpenProfileFromStreamTHR with read mode
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file != NULL) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, dummy_file, "r");
        // Do not close dummy_file here - library takes ownership
        cleanup_profile(hProfile);
    }
    
    // Test cmsOpenProfileFromStreamTHR with write mode
    dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file != NULL) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(context, dummy_file, "w");
        // Do not close dummy_file here - library takes ownership
        cleanup_profile(hProfile);
    }
    
    // Test cmsOpenIOhandlerFromFile with read mode
    cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    cleanup_iohandler(ioHandler);
    
    // Test cmsOpenIOhandlerFromFile with write mode
    ioHandler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "w");
    cleanup_iohandler(ioHandler);
    
    // Test cmsfilelength
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file != NULL) {
        long file_len = cmsfilelength(dummy_file);
        (void)file_len; // Use result to avoid unused variable warning
        fclose(dummy_file);
    }
    
    // Test cmsSaveProfileToStream
    // First create a minimal profile to save
    // Use cmsCreate_sRGBProfile instead which doesn't require complex structure initialization
    cmsHPROFILE testProfile = cmsCreate_sRGBProfile();
    
    if (testProfile != NULL) {
        dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file != NULL) {
            cmsBool save_result = cmsSaveProfileToStream(testProfile, dummy_file);
            (void)save_result; // Use result to avoid unused variable warning
            fclose(dummy_file);
        }
        cleanup_profile(testProfile);
    }
    
    // Test cmsOpenProfileFromStream with read mode
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file != NULL) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStream(dummy_file, "r");
        fclose(dummy_file); // This one doesn't take ownership
        cleanup_profile(hProfile);
    }
    
    // Test cmsOpenProfileFromStream with write mode
    dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file != NULL) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStream(dummy_file, "w");
        fclose(dummy_file); // This one doesn't take ownership
        cleanup_profile(hProfile);
    }
    
    // Test cmsOpenIOhandlerFromStream
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file != NULL) {
        ioHandler = cmsOpenIOhandlerFromStream(context, dummy_file);
        fclose(dummy_file); // This one doesn't take ownership
        cleanup_iohandler(ioHandler);
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}