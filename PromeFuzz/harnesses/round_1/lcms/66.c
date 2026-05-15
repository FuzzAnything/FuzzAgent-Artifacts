// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
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

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    // Create a context for thread-safe operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }
    
    // Write fuzzer data to dummy file for file-based operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Test 1: cmsOpenIOhandlerFromFile
    cmsIOHANDLER* io1 = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
    if (io1 != NULL) {
        // Test 2: cmsOpenProfileFromIOhandlerTHR with the created I/O handler
        cmsHPROFILE profile1 = cmsOpenProfileFromIOhandlerTHR(ctx, io1);
        cleanup_profile(profile1);
        // Use cmsCloseIOhandler if available, otherwise just let it leak
        // since we can't access the internal structure
    }
    
    // Test 3: cmsOpenProfileFromStreamTHR
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream != NULL) {
        cmsHPROFILE profile2 = cmsOpenProfileFromStreamTHR(ctx, stream, "r");
        cleanup_profile(profile2);
        fclose(stream);
    }
    
    // Test 4: cmsOpenProfileFromFileTHR
    cmsHPROFILE profile3 = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "r");
    cleanup_profile(profile3);
    
    // Test 5: cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE profile4 = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "./dummy_file");
    cleanup_profile(profile4);
    
    // Test 6: cmsOpenIOhandlerFromMem
    // Ensure we have a valid buffer (non-NULL with size > 0)
    void* buffer = NULL;
    if (Size > 0) {
        buffer = malloc(Size);
        if (buffer != NULL) {
            memcpy(buffer, Data, Size);
            cmsIOHANDLER* io2 = cmsOpenIOhandlerFromMem(ctx, buffer, Size, "r");
            if (io2 != NULL) {
                // Try to open profile from memory I/O handler
                cmsHPROFILE profile5 = cmsOpenProfileFromIOhandlerTHR(ctx, io2);
                cleanup_profile(profile5);
                // Use cmsCloseIOhandler if available, otherwise just let it leak
            }
            free(buffer);
        }
    } else {
        // Test with NULL buffer and zero size
        cmsIOHANDLER* io3 = cmsOpenIOhandlerFromMem(ctx, NULL, 0, "r");
        // Use cmsCloseIOhandler if available, otherwise just let it leak
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}