// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Test cmsfilelength
    FILE *f1 = fopen("./dummy_file", "rb");
    if (f1) {
        long file_len = cmsfilelength(f1);
        fclose(f1);
    }
    
    // Test cmsOpenProfileFromStream with read mode
    // Note: cmsOpenProfileFromStream takes ownership of the FILE* and closes it internally
    FILE *f2 = fopen("./dummy_file", "rb");
    if (f2) {
        cmsHPROFILE profile1 = cmsOpenProfileFromStream(f2, "r");
        cleanup_profile(profile1);
        // Do NOT close f2 here - it's already closed by cmsCloseProfile
    }
    
    // Test cmsOpenProfileFromStreamTHR with read mode
    // Note: cmsOpenProfileFromStreamTHR also takes ownership of the FILE*
    FILE *f3 = fopen("./dummy_file", "rb");
    if (f3) {
        cmsHPROFILE profile2 = cmsOpenProfileFromStreamTHR(NULL, f3, "r");
        cleanup_profile(profile2);
        // Do NOT close f3 here - it's already closed by cmsCloseProfile
    }
    
    // Test cmsOpenIOhandlerFromStream
    FILE *f4 = fopen("./dummy_file", "rb");
    if (f4) {
        cmsIOHANDLER* handler1 = cmsOpenIOhandlerFromStream(NULL, f4);
        // The handler takes ownership of the stream, don't close f4
        // We can't directly free the handler, but it will be cleaned up when the program ends
    }
    
    // Test cmsOpenIOhandlerFromFile with read mode
    cmsIOHANDLER* handler2 = cmsOpenIOhandlerFromFile(NULL, "./dummy_file", "r");
    // No direct cleanup available for opaque handler
    
    // Test cmsSaveProfileToStream - need a profile first
    // Create a simple profile to test saving
    cmsToneCurve* transfer[3] = { NULL, NULL, NULL };
    cmsHPROFILE test_profile = cmsCreateRGBProfile(NULL, NULL, transfer);
    if (test_profile) {
        FILE *f5 = fopen("./dummy_file", "wb");
        if (f5) {
            // cmsSaveProfileToStream does NOT take ownership of the FILE*
            cmsBool save_result = cmsSaveProfileToStream(test_profile, f5);
            fclose(f5);  // We need to close this one ourselves
        }
        cleanup_profile(test_profile);
    }
    
    // Test cmsOpenProfileFromStream with write mode
    FILE *f6 = fopen("./dummy_file", "wb");
    if (f6) {
        cmsHPROFILE profile3 = cmsOpenProfileFromStream(f6, "w");
        cleanup_profile(profile3);
        // Do NOT close f6 here - it's already closed by cmsCloseProfile
    }
    
    // Test cmsOpenProfileFromStreamTHR with write mode
    FILE *f7 = fopen("./dummy_file", "wb");
    if (f7) {
        cmsHPROFILE profile4 = cmsOpenProfileFromStreamTHR(NULL, f7, "w");
        cleanup_profile(profile4);
        // Do NOT close f7 here - it's already closed by cmsCloseProfile
    }
    
    // Test cmsOpenIOhandlerFromFile with write mode
    cmsIOHANDLER* handler3 = cmsOpenIOhandlerFromFile(NULL, "./dummy_file", "w");
    // No direct cleanup available for opaque handler
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}