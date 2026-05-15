// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to file for file-based functions
    write_dummy_file(Data, Size);
    
    // Create context for thread-safe operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        remove_dummy_file();
        return 0;
    }
    
    // 1. Test cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE profile1 = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "./dummy_file");
    if (profile1) {
        cmsCloseProfile(profile1);
    }
    
    // 2. Test cmsOpenProfileFromFileTHR with different access modes
    cmsHPROFILE profile2 = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "r");
    if (profile2) {
        cmsCloseProfile(profile2);
    }
    
    // Try with write mode (though likely to fail with read-only data)
    cmsHPROFILE profile2w = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "w");
    if (profile2w) {
        cmsCloseProfile(profile2w);
    }
    
    // 3. Test cmsOpenProfileFromIOhandlerTHR with file-based I/O handler
    // 6. First create file-based I/O handler
    cmsIOHANDLER* io_file = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
    if (io_file) {
        cmsHPROFILE profile3 = cmsOpenProfileFromIOhandlerTHR(ctx, io_file);
        if (profile3) {
            cmsCloseProfile(profile3);
        }
        // cmsOpenProfileFromIOhandlerTHR takes ownership of io_file
        // Don't call cmsCloseIOhandler here
    }
    
    // 4. Test cmsOpenIOhandlerFromMem with memory buffer
    // 3. Then use it with cmsOpenProfileFromIOhandlerTHR
    if (Size > 0) {
        cmsIOHANDLER* io_mem = cmsOpenIOhandlerFromMem(ctx, (void*)Data, Size, "r");
        if (io_mem) {
            cmsHPROFILE profile4 = cmsOpenProfileFromIOhandlerTHR(ctx, io_mem);
            if (profile4) {
                cmsCloseProfile(profile4);
            }
            // cmsOpenProfileFromIOhandlerTHR takes ownership of io_mem
            // Don't call cmsCloseIOhandler here
        }
    }
    
    // 5. Test cmsOpenProfileFromStreamTHR - note: the stream is owned by the profile
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsHPROFILE profile5 = cmsOpenProfileFromStreamTHR(ctx, stream, "r");
        if (profile5) {
            cmsCloseProfile(profile5);
        } else {
            // Only close the stream if profile creation fails
            fclose(stream);
        }
    }
    
    // Clean up
    cmsDeleteContext(ctx);
    remove_dummy_file();
    
    return 0;
}