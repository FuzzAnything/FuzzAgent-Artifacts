// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_iohandler(cmsIOHANDLER* handler) {
    if (handler) {
        cmsCloseIOhandler(handler);
    }
}

int LLVMFuzzerTestOneInput_70(const uint8_t *data, size_t size) {
    // Write fuzzing data to dummy file for file-based APIs
    write_dummy_file(data, size);
    
    // Create context for thread-safe APIs
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    // Test cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE profile1 = cmsCreateDeviceLinkFromCubeFileTHR(context, "./dummy_file");
    cleanup_profile(profile1);
    
    // Test cmsOpenIOhandlerFromFile with read mode
    cmsIOHANDLER* handler1 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    cleanup_iohandler(handler1);
    
    // Test cmsOpenIOhandlerFromFile with write mode (empty file)
    cmsIOHANDLER* handler2 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "w");
    cleanup_iohandler(handler2);
    
    // Test cmsOpenProfileFromStreamTHR - DO NOT close the stream after passing it
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsHPROFILE profile2 = cmsOpenProfileFromStreamTHR(context, stream, "r");
        cleanup_profile(profile2);
        // Do NOT call fclose(stream) here - the library takes ownership
    }
    
    // Test cmsOpenIOhandlerFromMem with read mode
    if (size > 0) {
        void* buffer = malloc(size);
        if (buffer) {
            memcpy(buffer, data, size);
            cmsIOHANDLER* handler3 = cmsOpenIOhandlerFromMem(context, buffer, size, "r");
            cleanup_iohandler(handler3);
            free(buffer);
        }
    }
    
    // Test cmsOpenProfileFromFileTHR with read mode
    cmsHPROFILE profile3 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
    cleanup_profile(profile3);
    
    // Test cmsOpenProfileFromFileTHR with write mode (empty file)
    cmsHPROFILE profile4 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "w");
    cleanup_profile(profile4);
    
    // Test cmsCreateDeviceLinkFromCubeFile (non-thread-safe version)
    cmsHPROFILE profile5 = cmsCreateDeviceLinkFromCubeFile("./dummy_file");
    cleanup_profile(profile5);
    
    // Cleanup context
    cmsDeleteContext(context);
    
    return 0;
}