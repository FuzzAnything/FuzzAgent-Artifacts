// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a temporary file to hold the fuzzed data
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f == NULL) {
        return 0;
    }
    size_t written = fwrite(Data, 1, Size, f);
    fclose(f);

    // Reopen file for reading operations
    f = fopen(dummy_file, "rb");
    if (f == NULL) {
        return 0;
    }

    // Test cmsfilelength
    long file_len = cmsfilelength(f);
    // Reset file position for subsequent reads
    if (file_len != -1) {
        fseek(f, 0, SEEK_SET);
    }

    // Test cmsOpenProfileFromStream
    cmsHPROFILE profile1 = cmsOpenProfileFromStream(f, "r");
    cleanup_profile(profile1);

    // Reset file position
    fseek(f, 0, SEEK_SET);

    // Test cmsOpenProfileFromStreamTHR with default context (NULL)
    cmsHPROFILE profile2 = cmsOpenProfileFromStreamTHR(NULL, f, "r");
    cleanup_profile(profile2);

    // Reset file position
    fseek(f, 0, SEEK_SET);

    // Create a custom context for threaded tests
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context != NULL) {
        // Test cmsOpenProfileFromFileTHR
        cmsHPROFILE profile3 = cmsOpenProfileFromFileTHR(context, dummy_file, "r");
        cleanup_profile(profile3);

        // Reset file position
        fseek(f, 0, SEEK_SET);

        // Test cmsOpenProfileFromStreamTHR with custom context
        cmsHPROFILE profile4 = cmsOpenProfileFromStreamTHR(context, f, "r");
        cleanup_profile(profile4);

        cleanup_context(context);
    }

    // Test cmsOpenProfileFromFile
    cmsHPROFILE profile5 = cmsOpenProfileFromFile(dummy_file, "r");
    cleanup_profile(profile5);

    // Test cmsCreateDeviceLinkFromCubeFile
    cmsHPROFILE profile6 = cmsCreateDeviceLinkFromCubeFile(dummy_file);
    cleanup_profile(profile6);

    // Cleanup
    fclose(f);
    remove(dummy_file);

    return 0;
}