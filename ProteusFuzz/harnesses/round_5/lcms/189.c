// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void custom_log_handler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *ErrorText, va_list args) {
    (void)ContextID;
    (void)ErrorCode;
    (void)ErrorText;
    (void)args;
}

static void custom_log_handler_simple(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *ErrorText) {
    (void)ContextID;
    (void)ErrorCode;
    (void)ErrorText;
}

int LLVMFuzzerTestOneInput_189(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Test cmsSetLogErrorHandlerTHR
    cmsSetLogErrorHandlerTHR(ContextID, custom_log_handler_simple);

    // Test cmsSignalError
    cmsSignalError(ContextID, 0, "Test error message: %d %s", 42, "fuzz");

    // Test cmsOpenIOhandlerFromFile
    cmsIOHANDLER *ioh = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "r");
    if (ioh != NULL) {
        cmsCloseIOhandler(ioh);
    }

    // Test cmsOpenIOhandlerFromFile with write mode
    ioh = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "w");
    if (ioh != NULL) {
        cmsCloseIOhandler(ioh);
    }

    // Test cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE profile1 = cmsCreateDeviceLinkFromCubeFileTHR(ContextID, "./dummy_file");
    if (profile1 != NULL) {
        cmsCloseProfile(profile1);
    }

    // Test cmsOpenProfileFromStreamTHR
    // Note: cmsOpenProfileFromStreamTHR may take ownership of the stream or close it internally.
    // We open the file, pass it to the function, and do NOT close it ourselves to avoid double-free.
    FILE *stream = fopen("./dummy_file", "r");
    if (stream != NULL) {
        cmsHPROFILE profile2 = cmsOpenProfileFromStreamTHR(ContextID, stream, "r");
        if (profile2 != NULL) {
            cmsCloseProfile(profile2);
        }
        // Do not fclose(stream) here as the library might have closed it or taken ownership.
        // If the library does not close it, the OS will clean it up on process exit.
    }

    // Test cmsOpenProfileFromStreamTHR with write mode
    stream = fopen("./dummy_file", "w");
    if (stream != NULL) {
        cmsHPROFILE profile3 = cmsOpenProfileFromStreamTHR(ContextID, stream, "w");
        if (profile3 != NULL) {
            cmsCloseProfile(profile3);
        }
        // Do not fclose(stream) here.
    }

    // Test cmsSetLogErrorHandler (global)
    cmsSetLogErrorHandler(custom_log_handler_simple);

    // Cleanup
    cmsDeleteContext(ContextID);

    return 0;
}