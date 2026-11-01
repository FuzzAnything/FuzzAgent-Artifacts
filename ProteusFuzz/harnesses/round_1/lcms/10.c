// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void myErrorHandler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *ErrorText) {
    (void)ContextID;
    (void)ErrorCode;
    (void)ErrorText;
}

static void myErrorHandlerTHR(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *ErrorText) {
    (void)ContextID;
    (void)ErrorCode;
    (void)ErrorText;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsSetLogErrorHandler(myErrorHandler);
    cmsSetLogErrorHandlerTHR(ContextID, myErrorHandlerTHR);

    cmsSignalError(ContextID, 0, "Fuzz test error: %zu bytes", Size);

    cmsHPROFILE hProfileMem = cmsOpenProfileFromMemTHR(ContextID, Data, Size);
    if (hProfileMem != NULL) {
        cmsCloseProfile(hProfileMem);
    }

    cmsIOHANDLER* iohandler = cmsOpenIOhandlerFromFile(ContextID, "./dummy_file", "r");
    (void)iohandler;

    FILE* dummyFile = fopen("./dummy_file", "wb");
    if (dummyFile != NULL) {
        fwrite(Data, 1, Size, dummyFile);
        fclose(dummyFile);
    }

    dummyFile = fopen("./dummy_file", "rb");
    if (dummyFile != NULL) {
        cmsHPROFILE hProfileStream = cmsOpenProfileFromStreamTHR(ContextID, dummyFile, "r");
        if (hProfileStream != NULL) {
            cmsCloseProfile(hProfileStream);
        }
        fclose(dummyFile);
    }

    dummyFile = fopen("./dummy_file", "wb");
    if (dummyFile != NULL) {
        cmsHPROFILE hProfileWrite = cmsOpenProfileFromStreamTHR(ContextID, dummyFile, "w");
        if (hProfileWrite != NULL) {
            cmsCloseProfile(hProfileWrite);
        }
        fclose(dummyFile);
    }

    cmsDeleteContext(ContextID);

    return 0;
}