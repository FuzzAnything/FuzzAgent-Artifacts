// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    // Ensure minimum size for operations
    if (Size < 1) return 0;

    // Create a dummy file with fuzzing data
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) return 0;
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);

    // Test cmsfilelength
    FILE *file1 = fopen("./dummy_file", "rb");
    if (file1) {
        long int file_len = cmsfilelength(file1);
        (void)file_len; // Use result to avoid unused variable warning
        fclose(file1);
    }

    // Test cmsOpenIOhandlerFromStream
    FILE *file2 = fopen("./dummy_file", "rb");
    if (file2) {
        cmsContext context = cmsCreateContext(NULL, NULL);
        cmsIOHANDLER *handler = cmsOpenIOhandlerFromStream(context, file2);
        if (handler) {
            cmsCloseIOhandler(handler);
            // handler closed the stream, don't call fclose
        } else {
            // Handler failed, stream is NOT closed by library - need to close it
            fclose(file2);
        }
        cmsDeleteContext(context);
    }

    // Test cmsOpenIOhandlerFromFile
    const char *access_mode = (Size % 2 == 0) ? "r" : "w";
    cmsContext context2 = cmsCreateContext(NULL, NULL);
    cmsIOHANDLER *handler2 = cmsOpenIOhandlerFromFile(context2, "./dummy_file", access_mode);
    if (handler2) {
        cmsCloseIOhandler(handler2);
    }
    cmsDeleteContext(context2);

    // Test cmsSaveProfileToStream
    FILE *file3 = fopen("./dummy_file", "wb");
    if (file3) {
        // Create a simple sRGB profile to save
        cmsHPROFILE profile = cmsCreate_sRGBProfile();
        if (profile) {
            cmsBool save_result = cmsSaveProfileToStream(profile, file3);
            (void)save_result; // Use result to avoid unused variable warning
            cmsCloseProfile(profile);
        }
        fclose(file3);
    }

    // Test cmsOpenProfileFromStream
    FILE *file4 = fopen("./dummy_file", "rb");
    if (file4) {
        cmsHPROFILE profile2 = cmsOpenProfileFromStream(file4, "r");
        if (profile2) {
            cmsCloseProfile(profile2);
            // profile closed the stream, don't call fclose
        } else {
            // Profile failed, stream is closed by library - don't call fclose
        }
    }

    // Test cmsOpenProfileFromStreamTHR
    FILE *file5 = fopen("./dummy_file", "rb");
    if (file5) {
        cmsContext context3 = cmsCreateContext(NULL, NULL);
        cmsHPROFILE profile3 = cmsOpenProfileFromStreamTHR(context3, file5, "r");
        if (profile3) {
            cmsCloseProfile(profile3);
            // profile closed the stream, don't call fclose
        } else {
            // Profile failed, stream is closed by library - don't call fclose
        }
        cmsDeleteContext(context3);
    }

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}

#ifdef __cplusplus
}
#endif