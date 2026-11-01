// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_context_from_fuzz_data(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;

    // Attempt to use fuzz data to influence context behavior if possible
    // For now, we just ensure the context is valid.
    if (Size > 0) {
        // We can try to set a memory plugin or other settings, but standard
        // context creation is sufficient for basic fuzzing of these APIs.
        // The _cmsMalloc/_cmsFree functions rely on the context's internal plugin.
    }
    return ctx;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    void *buffer = NULL;
    void *malloced_ptr = NULL;
    cmsUInt32Number size_to_malloc = 0;
    cmsUInt32Number crd_size = 0;

    // Step 1: Prepare environment
    // Create a context
    ctx = create_context_from_fuzz_data(Data, Size);
    if (!ctx) {
        return 0;
    }

    // Create a dummy profile. We need a valid profile for cmsGetPostScriptCRD.
    // We can create a simple sRGB profile or use a minimal one.
    // Since we don't have a file, we create one in memory or use a standard one.
    // cmsCreateProfileFromNamedProfile is not always available or reliable for fuzzing.
    // Let's try to create a minimal profile using cmsCreateProfileFromIccBuffer if we had data.
    // Instead, let's create a simple profile using the API if possible, or just use NULL if the API handles it.
    // However, cmsGetPostScriptCRD expects a valid profile.
    // Let's create a simple sRGB profile.
    cmsHPROFILE hProfile_sRGB = cmsCreate_sRGBProfile();
    if (hProfile_sRGB) {
        hProfile = hProfile_sRGB;
    } else {
        // If we can't create a profile, we can't test cmsGetPostScriptCRD effectively.
        cmsDeleteContext(ctx);
        return 0;
    }

    // Derive intent and flags from fuzz data
    if (Size >= 2) {
        intent = Data[0] % 4; // 0 to 3
        flags = Data[1];
    }

    // Derive malloc size from fuzz data
    if (Size >= 4) {
        size_to_malloc = (Data[2] << 8) | Data[3];
        // Cap the size to prevent OOM or excessive allocation
        if (size_to_malloc > 1024 * 1024) {
            size_to_malloc = 1024 * 1024;
        }
    } else if (Size > 0) {
        size_to_malloc = Data[0];
    }

    // Step 2: Invoke target functions in order

    // 1. cmsGetPostScriptCRD (Query size)
    crd_size = cmsGetPostScriptCRD(ctx, hProfile, intent, flags, NULL, 0);

    // 2. _cmsMalloc
    if (size_to_malloc > 0) {
        malloced_ptr = _cmsMalloc(ctx, size_to_malloc);
        if (malloced_ptr) {
            // Fill with data to ensure memory is touched
            memset(malloced_ptr, 0xAB, size_to_malloc);
        }
    }

    // 3. cmsGetPostScriptCRD (Fill buffer)
    if (crd_size > 0) {
        buffer = malloc(crd_size);
        if (buffer) {
            cmsGetPostScriptCRD(ctx, hProfile, intent, flags, buffer, crd_size);
            free(buffer);
            buffer = NULL;
        }
    }

    // 4. _cmsFree
    if (malloced_ptr) {
        _cmsFree(ctx, malloced_ptr);
    }

    // Step 3: Cleanup
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    if (ctx) {
        cmsDeleteContext(ctx);
    }

    return 0;
}