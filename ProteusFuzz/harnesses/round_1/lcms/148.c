// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *dummy_file = "./dummy_file";
    const char *dummy_file2 = "./dummy_file2";

    // Write input data to a dummy file for file-based API tests
    write_dummy_file(dummy_file, Data, Size);
    write_dummy_file(dummy_file2, Data, Size);

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        ctx = cmsCreateContext(NULL, NULL); // Fallback if first fails
    }

    cmsHPROFILE profile = NULL;
    FILE *stream = NULL;

    // Test 1: cmsOpenProfileFromFile
    profile = cmsOpenProfileFromFile(dummy_file, "r");
    if (profile) cmsCloseProfile(profile);
    profile = NULL;

    // Test 2: cmsOpenProfileFromFileTHR
    profile = cmsOpenProfileFromFileTHR(ctx, dummy_file, "r");
    if (profile) cmsCloseProfile(profile);
    profile = NULL;

    // Test 3: cmsOpenProfileFromFile with write mode (may fail if file is not valid ICC)
    profile = cmsOpenProfileFromFile(dummy_file, "w");
    if (profile) cmsCloseProfile(profile);
    profile = NULL;

    // Test 4: cmsOpenProfileFromFileTHR with write mode
    profile = cmsOpenProfileFromFileTHR(ctx, dummy_file, "w");
    if (profile) cmsCloseProfile(profile);
    profile = NULL;

    // Test 5: cmsOpenProfileFromStream
    stream = fopen(dummy_file, "r");
    if (stream) {
        profile = cmsOpenProfileFromStream(stream, "r");
        if (profile) cmsCloseProfile(profile);
        profile = NULL;
        fclose(stream);
        stream = NULL;
    }

    // Test 6: cmsOpenProfileFromStreamTHR
    stream = fopen(dummy_file, "r");
    if (stream) {
        profile = cmsOpenProfileFromStreamTHR(ctx, stream, "r");
        if (profile) cmsCloseProfile(profile);
        profile = NULL;
        fclose(stream);
        stream = NULL;
    }

    // Test 7: cmsfilelength
    stream = fopen(dummy_file, "r");
    if (stream) {
        long len = cmsfilelength(stream);
        (void)len; // Use the return value to avoid unused warning
        fclose(stream);
        stream = NULL;
    }

    // Test 8: cmsCreateDeviceLinkFromCubeFile
    profile = cmsCreateDeviceLinkFromCubeFile(dummy_file2);
    if (profile) cmsCloseProfile(profile);
    profile = NULL;

    // Cleanup
    if (ctx) cmsDeleteContext(ctx);

    return 0;
}