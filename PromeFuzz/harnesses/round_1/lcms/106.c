// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file for file-based operations
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Create context for context-aware functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        remove("./dummy_file");
        return 0;
    }
    
    // Test cmsOpenIOhandlerFromFile with different access modes
    cmsIOHANDLER* handler1 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    if (handler1 != NULL) {
        cmsCloseIOhandler(handler1);
    }
    
    cmsIOHANDLER* handler2 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "w");
    if (handler2 != NULL) {
        cmsCloseIOhandler(handler2);
    }
    
    // Test cmsOpenProfileFromFile
    cmsHPROFILE profile1 = cmsOpenProfileFromFile("./dummy_file", "r");
    cleanup_profile(profile1);
    
    // Test cmsOpenProfileFromFileTHR
    cmsHPROFILE profile2 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
    cleanup_profile(profile2);
    
    // Test cmsOpenProfileFromStream
    FILE* stream1 = fopen("./dummy_file", "rb");
    if (stream1 != NULL) {
        cmsHPROFILE profile3 = cmsOpenProfileFromStream(stream1, "r");
        // Note: cmsOpenProfileFromStream takes ownership of the FILE* stream
        // Do not call fclose(stream1) here as it's managed by the profile
        cleanup_profile(profile3);
    }
    
    // Test cmsOpenProfileFromStreamTHR
    FILE* stream2 = fopen("./dummy_file", "rb");
    if (stream2 != NULL) {
        cmsHPROFILE profile4 = cmsOpenProfileFromStreamTHR(context, stream2, "r");
        // Note: cmsOpenProfileFromStreamTHR also takes ownership of the FILE* stream
        // Do not call fclose(stream2) here as it's managed by the profile
        cleanup_profile(profile4);
    }
    
    // Test cmsOpenIOhandlerFromMem
    // Create a copy of the input data to avoid const issues
    void* buffer = malloc(Size);
    if (buffer != NULL) {
        memcpy(buffer, Data, Size);
        cmsIOHANDLER* handler3 = cmsOpenIOhandlerFromMem(context, buffer, Size, "r");
        if (handler3 != NULL) {
            cmsCloseIOhandler(handler3);
        }
        free(buffer);
    }
    
    // Cleanup
    cmsDeleteContext(context);
    remove("./dummy_file");
    
    return 0;
}