// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_context(cmsContext context) {
    if (context != NULL) {
        cmsDeleteContext(context);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with fuzzing data
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Test cmsOpenProfileFromFile
    cmsHPROFILE hProfile1 = cmsOpenProfileFromFile(dummy_file, "r");
    cleanup_profile(hProfile1);

    // Test cmsOpenProfileFromFileTHR with default context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        cmsHPROFILE hProfile2 = cmsOpenProfileFromFileTHR(context, dummy_file, "r");
        cleanup_profile(hProfile2);
        cleanup_context(context);
    }

    // Test cmsOpenProfileFromStream
    FILE *stream = fopen(dummy_file, "rb");
    if (stream) {
        cmsHPROFILE hProfile3 = cmsOpenProfileFromStream(stream, "r");
        cleanup_profile(hProfile3);
        fclose(stream);
    }

    // Test cmsOpenProfileFromStreamTHR with context
    context = cmsCreateContext(NULL, NULL);
    if (context) {
        stream = fopen(dummy_file, "rb");
        if (stream) {
            cmsHPROFILE hProfile4 = cmsOpenProfileFromStreamTHR(context, stream, "r");
            cleanup_profile(hProfile4);
            fclose(stream);
        }
        cleanup_context(context);
    }

    // Test cmsfilelength
    stream = fopen(dummy_file, "rb");
    if (stream) {
        long fileLen = cmsfilelength(stream);
        (void)fileLen; // Use the return value to avoid unused variable warning
        fclose(stream);
    }

    // Test cmsSaveProfileToStream with a valid profile
    // First, try to open a profile to save
    hProfile1 = cmsOpenProfileFromFile(dummy_file, "r");
    if (hProfile1) {
        FILE *outStream = fopen(dummy_file, "wb");
        if (outStream) {
            cmsBool saveResult = cmsSaveProfileToStream(hProfile1, outStream);
            (void)saveResult; // Use the return value to avoid unused variable warning
            fclose(outStream);
        }
        cleanup_profile(hProfile1);
    }

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}