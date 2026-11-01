// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
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

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Create a dummy profile for testing save operations
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        cleanup_context(ctx);
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    
    // Write dummy data to a file for testing open operations
    FILE *fp = fopen(dummy_file, "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        fp = NULL;
    }

    // Test cmsOpenProfileFromFile
    cmsHPROFILE hProfileFromFile = cmsOpenProfileFromFile(dummy_file, "r");
    cleanup_profile(hProfileFromFile);

    // Test cmsOpenProfileFromFileTHR
    cmsHPROFILE hProfileFromFileTHR = cmsOpenProfileFromFileTHR(ctx, dummy_file, "r");
    cleanup_profile(hProfileFromFileTHR);

    // Test cmsOpenProfileFromStream
    fp = fopen(dummy_file, "rb");
    if (fp != NULL) {
        cmsHPROFILE hProfileFromStream = cmsOpenProfileFromStream(fp, "r");
        cleanup_profile(hProfileFromStream);
        fclose(fp);
        fp = NULL;
    }

    // Test cmsOpenProfileFromStreamTHR
    fp = fopen(dummy_file, "rb");
    if (fp != NULL) {
        cmsHPROFILE hProfileFromStreamTHR = cmsOpenProfileFromStreamTHR(ctx, fp, "r");
        cleanup_profile(hProfileFromStreamTHR);
        fclose(fp);
        fp = NULL;
    }

    // Test cmsfilelength
    fp = fopen(dummy_file, "rb");
    if (fp != NULL) {
        long fileLength = cmsfilelength(fp);
        (void)fileLength; // Use the value to avoid unused variable warning
        fclose(fp);
        fp = NULL;
    }

    // Test cmsSaveProfileToStream
    fp = fopen(dummy_file, "wb");
    if (fp != NULL) {
        cmsBool saveResult = cmsSaveProfileToStream(hProfile, fp);
        (void)saveResult; // Use the value to avoid unused variable warning
        fclose(fp);
        fp = NULL;
    }

    // Cleanup
    cleanup_profile(hProfile);
    cleanup_context(ctx);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}