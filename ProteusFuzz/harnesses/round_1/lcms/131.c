// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

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

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file for file-based API testing
    FILE *f = fopen("./dummy_file", "wb");
    if (f == NULL) return 0;
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Test cmsOpenProfileFromFile
    cmsHPROFILE hProfile1 = cmsOpenProfileFromFile("./dummy_file", "r");
    cleanup_profile(hProfile1);

    // Test cmsOpenProfileFromFileTHR with default context
    cmsHPROFILE hProfile2 = cmsOpenProfileFromFileTHR(NULL, "./dummy_file", "r");
    cleanup_profile(hProfile2);

    // Test cmsOpenProfileFromFileTHR with custom context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context != NULL) {
        cmsHPROFILE hProfile3 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
        cleanup_profile(hProfile3);
        
        // Test cmsOpenProfileFromStreamTHR with custom context
        FILE *f2 = fopen("./dummy_file", "rb");
        if (f2 != NULL) {
            cmsHPROFILE hProfile4 = cmsOpenProfileFromStreamTHR(context, f2, "r");
            // According to lcms2 documentation:
            // "If the function fails, the FILE* is closed by the function."
            // "If the function succeeds, the FILE* is closed when the profile is closed."
            // Therefore, we should NEVER call fclose on f2 if hProfile4 is not NULL.
            // If hProfile4 is NULL, the function has already closed f2, so we must NOT call fclose.
            // Wait, re-reading the docs: "If the function fails, the FILE* is closed by the function."
            // So if it returns NULL, the file is already closed. We must not close it again.
            // If it returns a valid profile, the library owns the file. We must not close it.
            // So in both cases, we do NOT call fclose on f2.
            
            if (hProfile4 != NULL) {
                cleanup_profile(hProfile4);
            }
            // f2 is managed by the library or already closed. Do not fclose here.
        }
        
        cleanup_context(context);
    }

    // Test cmsOpenProfileFromStream
    FILE *f3 = fopen("./dummy_file", "rb");
    if (f3 != NULL) {
        cmsHPROFILE hProfile5 = cmsOpenProfileFromStream(f3, "r");
        // Same logic as above: if success, library owns file. If fail, library closed it.
        if (hProfile5 != NULL) {
            cleanup_profile(hProfile5);
        }
        // Do not fclose f3 here.
    }

    // Test cmsfilelength
    FILE *f4 = fopen("./dummy_file", "rb");
    if (f4 != NULL) {
        long length = cmsfilelength(f4);
        (void)length; // Use the value to avoid unused warning
        fclose(f4);
    }

    // Test cmsSaveProfileToStream with a valid profile (if we can create one)
    // We attempt to open a profile first to use for saving
    FILE *f5 = fopen("./dummy_file", "rb");
    if (f5 != NULL) {
        cmsHPROFILE hProfile6 = cmsOpenProfileFromStream(f5, "r");
        if (hProfile6 != NULL) {
            FILE *f6 = fopen("./dummy_file_out", "wb");
            if (f6 != NULL) {
                cmsBool result = cmsSaveProfileToStream(hProfile6, f6);
                (void)result;
                fclose(f6);
            }
            cleanup_profile(hProfile6);
            // f5 is managed by the library, do not fclose.
        } else {
            // If cmsOpenProfileFromStream failed, it closed f5. Do not fclose.
        }
    }

    // Clean up output file if it exists
    remove("./dummy_file_out");

    return 0;
}