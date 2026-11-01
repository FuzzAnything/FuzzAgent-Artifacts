// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_handle(cmsHANDLE handle) {
    if (handle) {
        cmsIT8Free(handle);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = NULL;
    cmsHPROFILE profile = NULL;
    cmsHANDLE it8_handle = NULL;
    cmsIOHANDLER* io = NULL;

    // Create context (with NULL plugin for default)
    ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Use input data to decide which API to fuzz
    uint32_t choice = 0;
    if (Size >= 4) {
        choice = *(uint32_t*)Data;
    }

    switch (choice % 5) {
        case 0:
            // Fuzz cmsCreateDeviceLinkFromCubeFileTHR
            // Create a dummy file
            FILE* f = fopen("./dummy_file", "wb");
            if (f) {
                if (Size > 4) fwrite(Data + 4, 1, Size - 4, f);
                fclose(f);
            }
            profile = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "./dummy_file");
            cleanup_profile(profile);
            break;

        case 1:
            // Fuzz cmsIT8LoadFromFile
            FILE* f2 = fopen("./dummy_file", "wb");
            if (f2) {
                if (Size > 4) fwrite(Data + 4, 1, Size - 4, f2);
                fclose(f2);
            }
            it8_handle = cmsIT8LoadFromFile(ctx, "./dummy_file");
            cleanup_handle(it8_handle);
            break;

        case 2:
            // Fuzz cmsOpenProfileFromIOhandler2THR
            FILE* f3 = fopen("./dummy_file", "wb");
            if (f3) {
                if (Size > 4) fwrite(Data + 4, 1, Size - 4, f3);
                fclose(f3);
            }
            io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
            if (io) {
                profile = cmsOpenProfileFromIOhandler2THR(ctx, io, 0);
                // If profile is created, it owns the IO handler. 
                // cmsCloseProfile will close the IO handler.
                // If profile is NULL, the library has already closed/freed the IO handler 
                // in the error path of cmsOpenProfileFromIOhandler2THR.
                // Therefore, we must NOT call cmsCloseIOhandler manually here.
                if (profile) {
                    cleanup_profile(profile);
                }
                // Removed: else { cmsCloseIOhandler(io); } to prevent double-free
            }
            break;

        case 3:
            // Fuzz cmsCreateXYZProfileTHR
            profile = cmsCreateXYZProfileTHR(ctx);
            cleanup_profile(profile);
            break;

        case 4:
            // Fuzz cmsOpenIOhandlerFromFile with different modes
            FILE* f4 = fopen("./dummy_file", "wb");
            if (f4) {
                if (Size > 4) fwrite(Data + 4, 1, Size - 4, f4);
                fclose(f4);
            }
            // Try reading
            io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
            if (io) {
                cmsCloseIOhandler(io);
            }
            // Try writing (creates empty file)
            io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "w");
            if (io) {
                cmsCloseIOhandler(io);
            }
            break;
    }

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}