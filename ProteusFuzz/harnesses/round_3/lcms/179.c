// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsContext CreateContext(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    return ctx;
}

static void CleanupProfile(cmsHPROFILE hProfile, cmsContext ctx) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_179(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = CreateContext(Data, Size);
    if (!ctx) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsCIExyY whitePoint = { 0.3127f, 0.3290f, 1.0f }; // D65 default
    cmsCIExyYTRIPLE primaries = { 
        {0.64f, 0.33f, 0.0f}, // Red
        {0.30f, 0.60f, 0.0f}, // Green
        {0.15f, 0.06f, 0.0f}  // Blue
    };
    cmsToneCurve *curves[3] = { NULL, NULL, NULL };

    // Helper to create a simple tone curve if needed
    // Use cmsBuildGamma with a gamma of 1.0 (identity) or cmsBuildToneCurve with a table size
    // cmsBuildToneCurve is the correct function name in lcms2, but let's ensure we use it correctly.
    // The error suggests implicit declaration, which means the header might not be included or 
    // the function name is slightly different in the specific version. 
    // However, cmsBuildToneCurve is standard. Let's try cmsBuildGamma as a fallback or ensure correct usage.
    // Actually, cmsBuildToneCurve takes (cmsContext, int nEntries).
    // Let's try to create a simple identity curve using cmsBuildGamma(1.0) if available, 
    // or just use cmsBuildToneCurve correctly.
    
    cmsToneCurve *curve = NULL;
    if (Size > 0) {
        // Try to build a tone curve. If the function is missing in the specific header version,
        // we might need to use cmsBuildGamma. But cmsBuildToneCurve is standard in lcms2.
        // The error "incompatible integer to pointer conversion" suggests the compiler thinks
        // cmsBuildToneCurve returns an int, which happens if it's not declared.
        // This implies lcms2.h might not be found or is incomplete in the fuzzer environment.
        // Assuming lcms2.h is correct, let's just call it. If it fails to compile, 
        // we might need to use a different approach like cmsBuildGamma.
        // Let's use cmsBuildGamma(1.0) as a safer alternative if cmsBuildToneCurve is problematic.
        // But the prompt says include lcms2.h. Let's assume the function exists.
        // To avoid the implicit declaration error, we must ensure the function is declared.
        // If the environment is broken, we might need to define a dummy or use a different function.
        // Let's try cmsBuildGamma which is definitely in lcms2.
        curve = cmsBuildGamma(ctx, 1.0);
    }

    if (curve) {
        curves[0] = curve;
        curves[1] = curve; 
        curves[2] = curve;
    }

    // Fuzzing Strategy: Randomly select which API to call based on input data
    uint8_t mode = Data[0] % 6;

    switch (mode) {
        case 0: {
            // cmsCreateGrayProfileTHR
            if (Size > 2) {
                whitePoint.x = (float)(Data[1] & 0xFF) / 255.0f;
                whitePoint.y = (float)(Data[2] & 0xFF) / 255.0f;
            }
            hProfile = cmsCreateGrayProfileTHR(ctx, &whitePoint, curve);
            break;
        }
        case 1: {
            // cmsCreate_OkLabProfile
            hProfile = cmsCreate_OkLabProfile(ctx);
            break;
        }
        case 2: {
            // cmsCreateNULLProfileTHR
            hProfile = cmsCreateNULLProfileTHR(ctx);
            break;
        }
        case 3: {
            // cmsCreateLab2ProfileTHR
            if (Size > 2) {
                whitePoint.x = (float)(Data[1] & 0xFF) / 255.0f;
                whitePoint.y = (float)(Data[2] & 0xFF) / 255.0f;
            }
            hProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
            break;
        }
        case 4: {
            // cmsCreateRGBProfileTHR
            if (Size > 2) {
                whitePoint.x = (float)(Data[1] & 0xFF) / 255.0f;
                whitePoint.y = (float)(Data[2] & 0xFF) / 255.0f;
            }
            if (Size > 5) {
                primaries.Red.x = (float)(Data[3] & 0xFF) / 255.0f;
                primaries.Red.y = (float)(Data[4] & 0xFF) / 255.0f;
            }
            hProfile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, curves);
            break;
        }
        case 5: {
            // cmsCreateLab4ProfileTHR
            if (Size > 2) {
                whitePoint.x = (float)(Data[1] & 0xFF) / 255.0f;
                whitePoint.y = (float)(Data[2] & 0xFF) / 255.0f;
            }
            hProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
            break;
        }
    }

    // Cleanup
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }

    // Cleanup tone curves created
    if (curve) {
        cmsFreeToneCurve(curve);
    }

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}