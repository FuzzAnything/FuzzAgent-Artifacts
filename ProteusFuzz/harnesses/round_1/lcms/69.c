// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a context for thread-safe operations
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    // Write fuzz data to a dummy file
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != NULL) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Test 1: cmsIT8LoadFromFile
    cmsHANDLE it8Handle = cmsIT8LoadFromFile(ContextID, dummy_file);
    // IT8 handles are managed by the context or specific API not exposed here.
    // We rely on context deletion for cleanup.

    // Test 2: cmsSignalError
    cmsSignalError(ContextID, 0, "Test error message: %s", "fuzz");

    // Test 3: cmsOpenIOhandlerFromFile and cmsOpenProfileFromIOhandler2THR
    // The profile opened from IOhandler takes ownership of the IOhandler.
    // Therefore, we must NOT manually close the IOhandler if the profile is created.
    // CRITICAL FIX: If cmsOpenProfileFromIOhandler2THR fails (returns NULL),
    // the library internally calls cmsCloseProfile on the placeholder, which
    // in turn calls cmsCloseIOhandler to free the IOhandler.
    // Thus, we must NEVER manually close the IOhandler after this call,
    // regardless of success or failure.
    cmsIOHANDLER* iohandler = cmsOpenIOhandlerFromFile(ContextID, dummy_file, "r");
    cmsHPROFILE profileFromIO = NULL;
    
    if (iohandler != NULL) {
        profileFromIO = cmsOpenProfileFromIOhandler2THR(ContextID, iohandler, 0);
        // Ownership transfer logic:
        // 1. If profileFromIO is valid, the profile owns the iohandler.
        // 2. If profileFromIO is NULL, the library's error path in cmsOpenProfileFromIOhandler2THR
        //    already called cmsCloseProfile, which freed the iohandler.
        // In both cases, the iohandler is managed/freed by the library.
        // Do not call cmsCloseIOhandler here.
    }
    
    cleanup_profile(profileFromIO);

    // Test 4: cmsOpenProfileFromFile
    cmsHPROFILE profileFromFile = cmsOpenProfileFromFile(dummy_file, "r");
    cleanup_profile(profileFromFile);

    // Test 5: cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE deviceLinkProfile = cmsCreateDeviceLinkFromCubeFileTHR(ContextID, dummy_file);
    cleanup_profile(deviceLinkProfile);

    // Cleanup context (this also cleans up resources associated with the context)
    cmsDeleteContext(ContextID);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}