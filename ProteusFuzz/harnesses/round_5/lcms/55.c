// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

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

static void write_dummy_file(const char* filename, const uint8_t* data, size_t size) {
    FILE* f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Write dummy file for testing file-based APIs
    write_dummy_file("./dummy_file", Data, Size);

    // Test 1: cmsOpenProfileFromFile
    cmsHPROFILE profile1 = cmsOpenProfileFromFile("./dummy_file", "r");
    cleanup_profile(profile1);

    // Test 2: cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE profile2 = cmsCreateDeviceLinkFromCubeFileTHR(ctx, "./dummy_file");
    cleanup_profile(profile2);

    // Test 3: cmsSignalError
    // Using 0 as a generic error code since specific enums might vary or be undefined in some builds
    cmsSignalError(ctx, 0, "Test error message: %d", 42);

    // Test 4: cmsOpenIOhandlerFromFile and cmsOpenProfileFromIOhandler2THR
    // cmsOpenProfileFromIOhandler2THR takes ownership of the iohandler.
    // If it returns a valid profile, the iohandler is closed internally when the profile is closed.
    // If it returns NULL, the iohandler was likely already freed during the internal error cleanup
    // of the temporary profile created by the function. Therefore, we must NOT close it manually.
    cmsIOHANDLER* iohandler = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
    cmsHPROFILE profile3 = NULL;
    
    if (iohandler) {
        profile3 = cmsOpenProfileFromIOhandler2THR(ctx, iohandler, FALSE);
        // Do not call cmsCloseIOhandler here. Ownership has been transferred to the library.
        // If profile3 is NULL, the library has already handled the cleanup of iohandler.
        // If profile3 is valid, cleanup_profile will handle the iohandler.
    }
    
    // Clean up profile3 if it was created successfully.
    // This will also close the associated iohandler internally.
    cleanup_profile(profile3);

    // Test 5: cmsIT8LoadFromFile
    cmsHANDLE it8handle = cmsIT8LoadFromFile(ctx, "./dummy_file");
    cleanup_handle(it8handle);

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}