// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Create a context for thread-safe operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Test 1: cmsOpenIOhandlerFromMem
    if (Size > 0) {
        cmsIOHANDLER* mem_handler = cmsOpenIOhandlerFromMem(ctx, (void*)Data, Size, "r");
        if (mem_handler != NULL) {
            cmsCloseIOhandler(mem_handler);
        }
        
        // Also test with write mode (though it may fail)
        mem_handler = cmsOpenIOhandlerFromMem(ctx, (void*)Data, Size, "w");
        if (mem_handler != NULL) {
            cmsCloseIOhandler(mem_handler);
        }
    }

    // Test 2 & 5: cmsOpenProfileFromStreamTHR and cmsOpenProfileFromStream
    // Write data to dummy file for stream operations
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy != NULL) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy);
        }
        fclose(dummy);
        
        // Test cmsOpenProfileFromStreamTHR in read mode
        dummy = fopen("./dummy_file", "rb");
        if (dummy != NULL) {
            cmsHPROFILE profile_thr = cmsOpenProfileFromStreamTHR(ctx, dummy, "r");
            if (profile_thr != NULL) {
                cmsCloseProfile(profile_thr);
                // The profile closing already closed the file, so don't fclose again
            } else {
                // When cmsOpenProfileFromStreamTHR returns NULL, the stream has already been closed
                // by the library in the error path. Do NOT call fclose.
            }
        }
        
        // Test cmsOpenProfileFromStream (legacy version without context)
        dummy = fopen("./dummy_file", "rb");
        if (dummy != NULL) {
            cmsHPROFILE profile_stream = cmsOpenProfileFromStream(dummy, "r");
            if (profile_stream != NULL) {
                cmsCloseProfile(profile_stream);
                // The profile closing already closed the file, so don't fclose again
            } else {
                // When cmsOpenProfileFromStream returns NULL, the stream has already been closed
                // by the library in the error path. Do NOT call fclose.
            }
        }
        
        // Test write mode for cmsOpenProfileFromStreamTHR
        dummy = fopen("./dummy_file", "wb");
        if (dummy != NULL) {
            cmsHPROFILE profile_thr = cmsOpenProfileFromStreamTHR(ctx, dummy, "w");
            if (profile_thr != NULL) {
                cmsCloseProfile(profile_thr);
                // The profile closing already closed the file, so don't fclose again
            } else {
                // When cmsOpenProfileFromStreamTHR returns NULL, the stream has already been closed
                // by the library in the error path. Do NOT call fclose.
            }
        }
    }

    // Test 3: cmsOpenIOhandlerFromStream
    dummy = fopen("./dummy_file", "rb");
    if (dummy != NULL) {
        cmsIOHANDLER* stream_handler = cmsOpenIOhandlerFromStream(ctx, dummy);
        if (stream_handler != NULL) {
            cmsCloseIOhandler(stream_handler);
            // cmsCloseIOhandler already closed the stream, so don't call fclose
        } else {
            // cmsOpenIOhandlerFromStream failed, stream is still open - close it
            fclose(dummy);
        }
    }

    // Test 4: cmsOpenIOhandlerFromFile
    if (Size > 0) {
        // Write data to file first
        dummy = fopen("./dummy_file", "wb");
        if (dummy != NULL) {
            fwrite(Data, 1, Size, dummy);
            fclose(dummy);
            
            // Test read mode
            cmsIOHANDLER* file_handler = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
            if (file_handler != NULL) {
                cmsCloseIOhandler(file_handler);
            }
            
            // Test write mode
            file_handler = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "w");
            if (file_handler != NULL) {
                cmsCloseIOhandler(file_handler);
            }
        }
    }

    // Test 6: cmsOpenProfileFromFileTHR
    if (Size > 0) {
        // Ensure file exists with data
        dummy = fopen("./dummy_file", "wb");
        if (dummy != NULL) {
            fwrite(Data, 1, Size, dummy);
            fclose(dummy);
            
            // Test read mode
            cmsHPROFILE file_profile = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "r");
            if (file_profile != NULL) {
                cmsCloseProfile(file_profile);
            }
            
            // Test write mode
            file_profile = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "w");
            if (file_profile != NULL) {
                cmsCloseProfile(file_profile);
            }
        }
    }

    // Clean up context
    cmsDeleteContext(ctx);
    
    // Remove dummy file if it exists
    remove("./dummy_file");
    
    return 0;
}