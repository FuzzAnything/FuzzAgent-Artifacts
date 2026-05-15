// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
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

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_iohandler(cmsIOHANDLER* io) {
    if (io) {
        cmsCloseIOhandler(io);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Create a context for thread-safe operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Write fuzzed data to dummy file for file-based functions
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }

    // 1. Test cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE profile1 = cmsCreateDeviceLinkFromCubeFileTHR(context, "./dummy_file");
    cleanup_profile(profile1);

    // 2. Test cmsOpenProfileFromFileTHR with read mode
    cmsHPROFILE profile2 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
    cleanup_profile(profile2);

    // 3. Test cmsOpenIOhandlerFromMem with read mode
    void* buffer_copy = NULL;
    if (Size > 0) {
        buffer_copy = malloc(Size);
        if (buffer_copy) {
            memcpy(buffer_copy, Data, Size);
            cmsIOHANDLER* io1 = cmsOpenIOhandlerFromMem(context, buffer_copy, Size, "r");
            cleanup_iohandler(io1);
            free(buffer_copy);
        }
    } else {
        // Test with NULL buffer
        cmsIOHANDLER* io1 = cmsOpenIOhandlerFromMem(context, NULL, 0, "r");
        cleanup_iohandler(io1);
    }

    // 4. Test cmsOpenProfileFromStreamTHR with read mode
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsHPROFILE profile3 = cmsOpenProfileFromStreamTHR(context, stream, "r");
        cleanup_profile(profile3);
        fclose(stream);
    }

    // 5. Test cmsOpenIOhandlerFromFile with read mode
    cmsIOHANDLER* io2 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    cleanup_iohandler(io2);

    // 6. Test cmsOpenProfileFromIOhandlerTHR
    // Create an IO handler first, then open profile from it
    if (Size > 0) {
        buffer_copy = malloc(Size);
        if (buffer_copy) {
            memcpy(buffer_copy, Data, Size);
            cmsIOHANDLER* io3 = cmsOpenIOhandlerFromMem(context, buffer_copy, Size, "r");
            if (io3) {
                cmsHPROFILE profile4 = cmsOpenProfileFromIOhandlerTHR(context, io3);
                cleanup_profile(profile4);
                cleanup_iohandler(io3);
            }
            free(buffer_copy);
        }
    }

    // Clean up context
    cmsDeleteContext(context);

    // Remove dummy file to avoid accumulation
    remove("./dummy_file");

    return 0;
}