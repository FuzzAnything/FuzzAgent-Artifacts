// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void cleanup_file() {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create context for thread-safe functions
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Write data to dummy file for file-based operations
    write_dummy_file(Data, Size);
    
    // Test cmsOpenIOhandlerFromFile
    cmsIOHANDLER* iohandler_file = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
    if (iohandler_file) {
        cmsCloseIOhandler(iohandler_file);
    }
    
    // Test cmsOpenProfileFromFileTHR
    cmsHPROFILE profile_file = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "r");
    if (profile_file) {
        cmsCloseProfile(profile_file);
    }
    
    // Test cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE cube_profile_thr = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "./dummy_file");
    if (cube_profile_thr) {
        cmsCloseProfile(cube_profile_thr);
    }
    
    // Test cmsOpenProfileFromStreamTHR
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsHPROFILE profile_stream = cmsOpenProfileFromStreamTHR(ctx, stream, "r");
        if (profile_stream) {
            cmsCloseProfile(profile_stream);
        }
        fclose(stream);
    }
    
    // Test cmsOpenIOhandlerFromMem
    // Ensure we have at least some data for memory operations
    size_t mem_size = Size > 0 ? Size : 1;
    void* buffer = malloc(mem_size);
    if (buffer) {
        memcpy(buffer, Data, mem_size);
        cmsIOHANDLER* iohandler_mem = cmsOpenIOhandlerFromMem(ctx, buffer, mem_size, "r");
        if (iohandler_mem) {
            cmsCloseIOhandler(iohandler_mem);
        }
        free(buffer);
    }
    
    // Test cmsCreateDeviceLinkFromCubeFile (non-thread-safe version)
    cmsHPROFILE cube_profile = cmsCreateDeviceLinkFromCubeFile("./dummy_file");
    if (cube_profile) {
        cmsCloseProfile(cube_profile);
    }
    
    // Cleanup
    cleanup_file();
    cmsDeleteContext(ctx);
    
    return 0;
}