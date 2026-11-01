// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsfilelength at cmserr.c:52:20 in lcms2.h
// cmsCreateDeviceLinkFromCubeFile at cmscgats.c:3285:23 in lcms2.h
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

int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *dummy_file = "./dummy_file";
    
    // Write input data to dummy file
    FILE *fp_write = fopen(dummy_file, "wb");
    if (fp_write == NULL) return 0;
    fwrite(Data, 1, Size, fp_write);
    fclose(fp_write);

    cmsContext context = NULL;
    cmsHPROFILE profile = NULL;
    FILE *fp_read = NULL;

    // Test cmsOpenProfileFromFile
    profile = cmsOpenProfileFromFile(dummy_file, "r");
    cleanup_profile(profile);
    profile = NULL;

    // Test cmsOpenProfileFromFileTHR
    context = cmsCreateContext(NULL, NULL);
    if (context != NULL) {
        profile = cmsOpenProfileFromFileTHR(context, dummy_file, "r");
        cleanup_profile(profile);
        profile = NULL;
        
        profile = cmsOpenProfileFromFileTHR(context, dummy_file, "w");
        cleanup_profile(profile);
        profile = NULL;
    }

    // Test cmsOpenProfileFromStream
    fp_read = fopen(dummy_file, "r");
    if (fp_read != NULL) {
        profile = cmsOpenProfileFromStream(fp_read, "r");
        cleanup_profile(profile);
        profile = NULL;
        fclose(fp_read);
        fp_read = NULL;
    }

    // Test cmsOpenProfileFromStreamTHR
    if (context == NULL) {
        context = cmsCreateContext(NULL, NULL);
    }
    if (context != NULL) {
        fp_read = fopen(dummy_file, "r");
        if (fp_read != NULL) {
            profile = cmsOpenProfileFromStreamTHR(context, fp_read, "r");
            cleanup_profile(profile);
            profile = NULL;
            fclose(fp_read);
            fp_read = NULL;
        }

        fp_read = fopen(dummy_file, "w");
        if (fp_read != NULL) {
            profile = cmsOpenProfileFromStreamTHR(context, fp_read, "w");
            cleanup_profile(profile);
            profile = NULL;
            fclose(fp_read);
            fp_read = NULL;
        }
    }

    // Test cmsfilelength
    fp_read = fopen(dummy_file, "r");
    if (fp_read != NULL) {
        long length = cmsfilelength(fp_read);
        (void)length; // Suppress unused variable warning
        fclose(fp_read);
        fp_read = NULL;
    }

    // Test cmsCreateDeviceLinkFromCubeFile
    profile = cmsCreateDeviceLinkFromCubeFile(dummy_file);
    cleanup_profile(profile);
    profile = NULL;

    // Cleanup
    cleanup_context(context);
    remove(dummy_file);

    return 0;
}