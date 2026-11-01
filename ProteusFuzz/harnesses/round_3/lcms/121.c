// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Custom log error handler for testing
static void CustomLogErrorHandler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *ErrorText) {
    (void)ContextID;
    (void)ErrorCode;
    (void)ErrorText;
}

// Helper to create a dummy file with fuzzed content
static void create_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    // Fuzz cmsSetLogErrorHandlerTHR
    cmsSetLogErrorHandlerTHR(ContextID, CustomLogErrorHandler);
    cmsSetLogErrorHandlerTHR(ContextID, NULL); // Restore default

    // Fuzz cmsSetLogErrorHandler (global)
    cmsSetLogErrorHandler(CustomLogErrorHandler);
    // Note: Behavior undefined if Fn is NULL per spec, but we test it carefully.
    // We avoid calling it with NULL if it causes issues, but the spec says "undefined".
    // Let's skip the NULL call for global handler to be safe, or just call it once.
    // cmsSetLogErrorHandler(NULL); 

    // Fuzz cmsSignalError
    cmsSignalError(ContextID, 1, "Test error message %d", 42);
    cmsSignalError(ContextID, 0, "Another error");

    // Prepare dummy file for I/O tests
    create_dummy_file(Data, Size);

    // Fuzz cmsOpenIOhandlerFromFile with various modes
    cmsIOHANDLER *ioh = NULL;
    
    // Try "r" mode
    ioh = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "r");
    if (ioh) {
        cmsCloseIOhandler(ioh);
    }

    // Try "w" mode
    ioh = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "w");
    if (ioh) {
        cmsCloseIOhandler(ioh);
    }

    // Try invalid modes
    ioh = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "rw");
    if (ioh) cmsCloseIOhandler(ioh);
    ioh = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "invalid");
    if (ioh) cmsCloseIOhandler(ioh);
    ioh = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "");
    if (ioh) cmsCloseIOhandler(ioh);

    // Fuzz cmsOpenProfileFromStreamTHR
    // Note: cmsOpenProfileFromStreamTHR may close the stream internally in some error paths or modes,
    // or the library implementation might take ownership. To avoid double-free, we do not fclose
    // the stream after passing it to the library. The OS will clean up the file descriptor.
    
    FILE *stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ContextID, stream, "r");
        if (hProfile) {
            cmsCloseProfile(hProfile);
        }
        // Do NOT fclose(stream) here to prevent double-free if library closed it.
    }

    // Try write mode
    stream = fopen("./dummy_file", "wb");
    if (stream) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ContextID, stream, "w");
        if (hProfile) {
            cmsCloseProfile(hProfile);
        }
        // Do NOT fclose(stream) here.
    }

    // Fuzz cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE hDeviceLink = cmsCreateDeviceLinkFromCubeFileTHR(ContextID, "./dummy_file");
    if (hDeviceLink) {
        cmsCloseProfile(hDeviceLink);
    }

    // Cleanup context
    cmsDeleteContext(ContextID);

    return 0;
}