// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
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

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file for file-based APIs
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    cmsContext ctx = cmsCreateContext(NULL, NULL);

    cmsHPROFILE profile = NULL;
    FILE *stream = NULL;

    // Test 1: cmsOpenProfileFromFile
    profile = cmsOpenProfileFromFile(dummy_file, "r");
    cleanup_profile(profile);
    profile = NULL;

    // Test 2: cmsCreateDeviceLinkFromCubeFile
    profile = cmsCreateDeviceLinkFromCubeFile(dummy_file);
    cleanup_profile(profile);
    profile = NULL;

    // Test 3: cmsOpenProfileFromFileTHR
    if (ctx != NULL) {
        profile = cmsOpenProfileFromFileTHR(ctx, dummy_file, "r");
        cleanup_profile(profile);
        profile = NULL;

        profile = cmsOpenProfileFromFileTHR(ctx, dummy_file, "w");
        cleanup_profile(profile);
        profile = NULL;
    }

    // Test 4: cmsOpenProfileFromStreamTHR
    if (ctx != NULL) {
        stream = fopen(dummy_file, "rb");
        if (stream) {
            profile = cmsOpenProfileFromStreamTHR(ctx, stream, "r");
            // cmsOpenProfileFromStreamTHR may close the stream on failure or success depending on implementation
            // We should not fclose here if the function took ownership, but the docs say it closes placeholder on failure.
            // To be safe and avoid double free, we check if profile is NULL (failure) to assume stream might be closed?
            // Actually, standard behavior for these APIs is often that if it returns NULL, it might have closed the stream.
            // However, to be strictly safe against double-free in this specific ASAN error context, 
            // we will assume the library might have closed the stream if it failed or succeeded in a way that consumes it.
            // But the error trace shows fclose was called on a stream that was already freed.
            // The safest approach for fuzzing when ownership is ambiguous is to NOT fclose if the API might have done it.
            // But the API docs say "closing the placeholder profile automatically" on failure.
            // Let's assume if profile is valid, we must close profile, but stream ownership is tricky.
            // The ASAN error indicates fclose was called on a stream that was already freed.
            // This likely happens if cmsOpenProfileFromStreamTHR closes the stream internally.
            // We will skip fclose for streams passed to these APIs to prevent double-free.
            if (profile) {
                cleanup_profile(profile);
                profile = NULL;
            }
            // Do not fclose(stream) here as the library might have closed it.
            // If the library didn't close it, it's a leak, but double-free is a crash.
            // However, to be correct, we need to know if it closed it.
            // Let's try a different approach: only fclose if the function returned NULL? No, that's not guaranteed either.
            // Given the ASAN error, the library definitely closed the stream.
            // We will remove fclose calls for streams passed to cmsOpenProfileFromStream* functions.
            stream = NULL; // Mark as handled
        }

        stream = fopen(dummy_file, "wb");
        if (stream) {
            profile = cmsOpenProfileFromStreamTHR(ctx, stream, "w");
            if (profile) {
                cleanup_profile(profile);
                profile = NULL;
            }
            // Do not fclose(stream) here either
            stream = NULL;
        }
    }

    // Test 5: cmsOpenProfileFromStream
    stream = fopen(dummy_file, "rb");
    if (stream) {
        profile = cmsOpenProfileFromStream(stream, "r");
        if (profile) {
            cleanup_profile(profile);
            profile = NULL;
        }
        // Do not fclose(stream)
        stream = NULL;
    }

    stream = fopen(dummy_file, "wb");
    if (stream) {
        profile = cmsOpenProfileFromStream(stream, "w");
        if (profile) {
            cleanup_profile(profile);
            profile = NULL;
        }
        // Do not fclose(stream)
        stream = NULL;
    }

    // Test 6: cmsfilelength
    // This function does not close the stream, so we can safely fclose here.
    stream = fopen(dummy_file, "rb");
    if (stream) {
        long len = cmsfilelength(stream);
        (void)len; 
        fclose(stream);
        stream = NULL;
    }

    // Cleanup
    cleanup_context(ctx);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}