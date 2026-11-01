// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void dummy_log_handler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *ErrorText) {
    (void)ContextID;
    (void)ErrorCode;
    (void)ErrorText;
}

static cmsLogErrorHandlerFunction get_log_handler(const uint8_t *Data, size_t Size) {
    if (Size > 0 && Data[0] % 2 == 0) {
        return dummy_log_handler;
    }
    return NULL;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context for thread-specific operations
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Fuzz cmsSetLogErrorHandler
    cmsLogErrorHandlerFunction Fn = get_log_handler(Data, Size);
    if (Fn != NULL) {
        cmsSetLogErrorHandler(Fn);
    }

    // Fuzz cmsSetLogErrorHandlerTHR
    cmsLogErrorHandlerFunction FnTHR = get_log_handler(Data, Size);
    if (FnTHR != NULL) {
        cmsSetLogErrorHandlerTHR(ContextID, FnTHR);
    } else {
        cmsSetLogErrorHandlerTHR(ContextID, NULL);
    }

    // Fuzz cmsOpenIOhandlerFromFile
    if (Size > 10) {
        char filename[256];
        char mode[10];
        size_t fname_len = (Data[0] % 10) + 1;
        size_t mode_len = (Data[1] % 2) + 1;
        if (fname_len < sizeof(filename) && mode_len < sizeof(mode)) {
            memcpy(filename, Data + 2, fname_len);
            filename[fname_len] = '\0';
            memcpy(mode, Data + 2 + fname_len, mode_len);
            mode[mode_len] = '\0';
            
            // Ensure mode is valid
            if (strcmp(mode, "r") == 0 || strcmp(mode, "w") == 0) {
                cmsIOHANDLER *iohandler = cmsOpenIOhandlerFromFile(ContextID, filename, mode);
                if (iohandler != NULL) {
                    cmsCloseIOhandler(iohandler);
                }
            }
        }
    }

    // Fuzz cmsOpenProfileFromMemTHR
    if (Size > 4) {
        cmsUInt32Number dwSize = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        if (Size >= dwSize + 4) {
            cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ContextID, Data + 4, dwSize);
            if (hProfile != NULL) {
                cmsCloseProfile(hProfile);
            }
        }
    }

    // Fuzz cmsSignalError
    if (Size > 3) {
        cmsUInt32Number ErrorCode = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        const char *ErrorText = "Fuzzed error message";
        cmsSignalError(ContextID, ErrorCode, ErrorText);
    }

    // Fuzz cmsOpenProfileFromStreamTHR
    FILE *stream = fopen("./dummy_file", "r");
    if (stream == NULL) {
        // Try to create the file if it doesn't exist
        stream = fopen("./dummy_file", "w");
        if (stream != NULL) {
            fwrite(Data, 1, Size, stream);
            fclose(stream);
            stream = fopen("./dummy_file", "r");
        }
    }

    if (stream != NULL) {
        cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ContextID, stream, "r");
        if (hProfile != NULL) {
            cmsCloseProfile(hProfile);
        }
        fclose(stream);
    }

    // Cleanup
    cmsDeleteContext(ContextID);

    return 0;
}