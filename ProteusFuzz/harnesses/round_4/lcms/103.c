// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_profiles(cmsContext ctx, cmsHPROFILE p1, cmsHPROFILE p2, cmsHPROFILE p3, cmsHPROFILE p4, cmsHPROFILE p5) {
    if (p1) cmsCloseProfile(p1);
    if (p2) cmsCloseProfile(p2);
    if (p3) cmsCloseProfile(p3);
    if (p4) cmsCloseProfile(p4);
    if (p5) cmsCloseProfile(p5);
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a thread-safe context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHPROFILE hXYZ = NULL;
    cmsHPROFILE hNULL = NULL;
    cmsHPROFILE hOkLab = NULL;
    cmsHPROFILE hsRGB = NULL;
    cmsHPROFILE hDummy = NULL;

    // 1. cmsCreateXYZProfileTHR
    hXYZ = cmsCreateXYZProfileTHR(ctx);

    // 2. cmsGetProfileContextID (test with valid and NULL profiles)
    cmsContext retrievedCtx = NULL;
    if (hXYZ) {
        retrievedCtx = cmsGetProfileContextID(hXYZ);
        if (retrievedCtx != ctx) {
            // Unexpected, but continue
        }
    }
    retrievedCtx = cmsGetProfileContextID(NULL); // Should return NULL

    // 3. cmsCreateNULLProfileTHR
    hNULL = cmsCreateNULLProfileTHR(ctx);

    // 4. cmsCreate_OkLabProfile
    hOkLab = cmsCreate_OkLabProfile(ctx);

    // 5. cmsCreate_sRGBProfileTHR
    hsRGB = cmsCreate_sRGBProfileTHR(ctx);

    // Create a dummy profile to test cmsGetProfileContextID and cleanup
    if (Size > 0) {
        // Attempt to create a profile from data if possible, or just use existing ones
        // Since we don't have a direct "create from buffer" in the target list, 
        // we rely on the generated profiles for context testing.
        // We can try to create a profile from a file if data looks like a file path, 
        // but the instructions say use "./dummy_file" if a file is required.
        // Here we just ensure we have at least one profile to test context retrieval.
        if (!hDummy) hDummy = hXYZ; 
    }

    // Test cmsGetProfileContextID on other profiles
    if (hNULL) cmsGetProfileContextID(hNULL);
    if (hOkLab) cmsGetProfileContextID(hOkLab);
    if (hsRGB) cmsGetProfileContextID(hsRGB);

    // 6. cmsUnregisterPluginsTHR
    // This clears thread-local plugin overrides.
    cmsUnregisterPluginsTHR(ctx);

    // Cleanup
    cleanup_profiles(ctx, hXYZ, hNULL, hOkLab, hsRGB, hDummy);
    
    // Destroy context
    cmsDeleteContext(ctx);

    return 0;
}