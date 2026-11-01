// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_context(cmsContext context) {
    if (context != NULL) {
        cmsDeleteContext(context);
    }
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for testing
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    cmsHPROFILE profile1 = NULL;
    cmsHPROFILE profile2 = NULL;
    cmsHPROFILE profile3 = NULL;
    cmsHPROFILE profile4 = NULL;
    cmsContext context = NULL;
    FILE *stream = NULL;

    // Test cmsCreateDeviceLinkFromCubeFile
    profile1 = cmsCreateDeviceLinkFromCubeFile(dummy_file);
    cleanup_profile(profile1);

    // Test cmsOpenProfileFromFile
    profile2 = cmsOpenProfileFromFile(dummy_file, "r");
    cleanup_profile(profile2);

    // Test cmsOpenProfileFromFileTHR
    context = cmsCreateContext(NULL, NULL);
    if (context != NULL) {
        profile3 = cmsOpenProfileFromFileTHR(context, dummy_file, "r");
        cleanup_profile(profile3);
    }

    // Test cmsOpenProfileFromStream
    stream = fopen(dummy_file, "rb");
    if (stream != NULL) {
        profile4 = cmsOpenProfileFromStream(stream, "r");
        cleanup_profile(profile4);
        fclose(stream);
        stream = NULL; // Reset pointer to avoid double free logic if reused
    }

    // Test cmsOpenProfileFromStreamTHR
    stream = fopen(dummy_file, "rb");
    if (stream != NULL && context != NULL) {
        profile4 = cmsOpenProfileFromStreamTHR(context, stream, "r");
        cleanup_profile(profile4);
        fclose(stream);
        stream = NULL; // Reset pointer
    }

    // Test cmsfilelength
    stream = fopen(dummy_file, "rb");
    if (stream != NULL) {
        long length = cmsfilelength(stream);
        (void)length; // Use the return value to avoid compiler warnings
        fclose(stream);
        stream = NULL; // Reset pointer
    }

    // Cleanup context
    cleanup_context(context);

    // Remove the dummy file
    remove(dummy_file);

    return 0;
}