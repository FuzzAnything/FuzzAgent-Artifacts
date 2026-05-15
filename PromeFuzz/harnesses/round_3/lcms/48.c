// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to dummy file for file-based operations
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);

    // Create context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }

    // 1. Test cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE hProfile1 = cmsCreateDeviceLinkFromCubeFileTHR(context, "./dummy_file");
    cleanup_profile(hProfile1);

    // 2. Test cmsOpenProfileFromFileTHR
    cmsHPROFILE hProfile2 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
    cleanup_profile(hProfile2);

    // 3. Test cmsOpenIOhandlerFromMem
    // Ensure we have some data for memory operations
    if (Size > 0) {
        void* buffer = malloc(Size);
        if (buffer != NULL) {
            memcpy(buffer, Data, Size);
            cmsIOHANDLER* ioHandler1 = cmsOpenIOhandlerFromMem(context, buffer, Size, "r");
            if (ioHandler1 != NULL) {
                cmsCloseIOhandler(ioHandler1);
            }
            free(buffer);
        }
    }

    // 4. Test cmsOpenProfileFromStreamTHR
    // Note: cmsOpenProfileFromStreamTHR takes ownership of the FILE* and will close it
    FILE* stream = fopen("./dummy_file", "rb");
    if (stream != NULL) {
        cmsHPROFILE hProfile3 = cmsOpenProfileFromStreamTHR(context, stream, "r");
        cleanup_profile(hProfile3);
        // Do NOT call fclose(stream) here - the stream is managed by lcms
    }

    // 5. Test cmsOpenIOhandlerFromFile
    cmsIOHANDLER* ioHandler2 = cmsOpenIOhandlerFromFile(context, "./dummy_file", "r");
    if (ioHandler2 != NULL) {
        cmsCloseIOhandler(ioHandler2);
    }

    // 6. Test cmsCreateDeviceLinkFromCubeFile (non-THR version)
    cmsHPROFILE hProfile4 = cmsCreateDeviceLinkFromCubeFile("./dummy_file");
    cleanup_profile(hProfile4);

    // Cleanup context
    cmsDeleteContext(context);

    return 0;
}