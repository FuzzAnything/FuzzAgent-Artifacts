// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
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

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_iohandler(cmsIOHANDLER* handler) {
    if (handler != NULL) {
        cmsCloseIOhandler(handler);
    }
}

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f != NULL) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t* Data, size_t Size) {
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Use first byte to determine which function to test
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0] % 6;
        Data++;
        Size--;
    }

    switch (selector) {
        case 0: {
            // cmsOpenIOhandlerFromFile
            write_dummy_file(Data, Size);
            const char* access_mode = (Size % 2 == 0) ? "r" : "w";
            cmsIOHANDLER* handler = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", access_mode);
            cleanup_iohandler(handler);
            break;
        }
        case 1: {
            // cmsOpenProfileFromFileTHR
            write_dummy_file(Data, Size);
            const char* access_mode = (Size % 2 == 0) ? "r" : "w";
            cmsHPROFILE profile = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", access_mode);
            cleanup_profile(profile);
            break;
        }
        case 2: {
            // cmsCreateDeviceLinkFromCubeFileTHR
            write_dummy_file(Data, Size);
            cmsHPROFILE profile = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "./dummy_file");
            cleanup_profile(profile);
            break;
        }
        case 3: {
            // cmsOpenProfileFromStreamTHR
            write_dummy_file(Data, Size);
            FILE* stream = fopen("./dummy_file", "rb");
            if (stream != NULL) {
                const char* access_mode = (Size % 2 == 0) ? "r" : "w";
                cmsHPROFILE profile = cmsOpenProfileFromStreamTHR(ctx, stream, access_mode);
                // Don't close the stream here - cmsCloseProfile will handle it
                cleanup_profile(profile);
                // Only close the stream if profile creation failed
                if (profile == NULL) {
                    fclose(stream);
                }
            }
            break;
        }
        case 4: {
            // cmsOpenIOhandlerFromMem
            if (Size > 0) {
                void* buffer = malloc(Size);
                if (buffer != NULL) {
                    memcpy(buffer, Data, Size);
                    cmsIOHANDLER* handler = cmsOpenIOhandlerFromMem(ctx, buffer, Size, "r");
                    cleanup_iohandler(handler);
                    free(buffer);
                }
            } else {
                // Test with NULL buffer
                cmsIOHANDLER* handler = cmsOpenIOhandlerFromMem(ctx, NULL, 0, "r");
                cleanup_iohandler(handler);
            }
            break;
        }
        case 5: {
            // cmsCreateDeviceLinkFromCubeFile
            write_dummy_file(Data, Size);
            cmsHPROFILE profile = cmsCreateDeviceLinkFromCubeFile("./dummy_file");
            cleanup_profile(profile);
            break;
        }
    }

    // Clean up context
    cmsDeleteContext(ctx);
    
    // Remove dummy file if it exists
    remove("./dummy_file");
    
    return 0;
}