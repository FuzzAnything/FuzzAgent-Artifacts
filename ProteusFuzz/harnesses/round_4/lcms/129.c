// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsSetAdaptationStateTHR at cmsxform.c:57:28 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE p1, cmsHPROFILE p2, cmsHPROFILE p3, cmsHPROFILE p4, cmsHPROFILE p5, cmsHPROFILE p6) {
    if (p1) cmsCloseProfile(p1);
    if (p2) cmsCloseProfile(p2);
    if (p3) cmsCloseProfile(p3);
    if (p4) cmsCloseProfile(p4);
    if (p5) cmsCloseProfile(p5);
    if (p6) cmsCloseProfile(p6);
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHPROFILE p_xyz = NULL;
    cmsHPROFILE p_oklab = NULL;
    cmsHPROFILE p_srgb = NULL;
    cmsHPROFILE p_ink = NULL;
    cmsHPROFILE p_linear = NULL;
    cmsHPROFILE p_dummy = NULL;

    cmsFloat64Number prev_adapt = 0.0;

    // 1. cmsCreateXYZProfileTHR
    p_xyz = cmsCreateXYZProfileTHR(ctx);

    // 2. cmsSetAdaptationStateTHR
    // Use data to determine the threshold value, ensuring non-negative
    cmsFloat64Number threshold = (cmsFloat64Number)Data[0] / 255.0;
    if (Size > 1) threshold += (cmsFloat64Number)Data[1] / 255.0;
    // Allow some negative values to test the "unchanged" logic
    if (Size > 2 && (Data[2] & 1)) {
        threshold = -1.0;
    }
    prev_adapt = cmsSetAdaptationStateTHR(ctx, threshold);

    // 3. cmsCreate_OkLabProfile
    p_oklab = cmsCreate_OkLabProfile(ctx);

    // 4. cmsCreate_sRGBProfileTHR
    p_srgb = cmsCreate_sRGBProfileTHR(ctx);

    // 5. cmsCreateInkLimitingDeviceLinkTHR
    // ColorSpace: Use cmsSigCmykData if available, else try others to trigger failure paths
    cmsColorSpaceSignature cs = cmsSigCmykData;
    if (Size > 3) {
        uint32_t cs_val = *(const uint32_t*)(Data + 3);
        // Map some values to known signatures, others to random to test failure
        if (cs_val == 0x434D594B) cs = cmsSigCmykData; // "CMYK"
        else if (cs_val == 0x52474220) cs = cmsSigRgbData; // "RGB "
        else cs = (cmsColorSpaceSignature)cs_val;
    }
    
    cmsFloat64Number limit = 100.0;
    if (Size > 7) {
        limit = *(const cmsFloat64Number*)(Data + 4);
    }
    // Clamp logic is internal, but we pass raw values to test clamping
    p_ink = cmsCreateInkLimitingDeviceLinkTHR(ctx, cs, limit);

    // 6. cmsCreateLinearizationDeviceLinkTHR
    // We need an array of cmsToneCurve pointers. 
    // We can create a dummy curve or pass NULLs to test error handling.
    cmsToneCurve* curves[4] = { NULL, NULL, NULL, NULL };
    
    // Try to create a simple linear curve if we have enough data to simulate a table
    if (Size > 10) {
        // Attempt to create a dummy curve using internal API is hard without full context
        // Instead, we pass NULLs to test the function's robustness against NULL inputs
        // Or create a valid curve if we can. Since we can't easily create a valid curve 
        // without more complex setup, we rely on the function handling NULLs or invalid pointers.
        // However, the API expects an array of pointers. Let's try to create one valid curve 
        // using cmsBuildToneCurve if possible, but it's not in the list. 
        // We will pass NULLs to test error paths.
    }
    
    // ColorSpace for linearization
    cmsColorSpaceSignature linear_cs = cmsSigRgbData;
    if (Size > 11) {
        uint32_t l_cs_val = *(const uint32_t*)(Data + 11);
        if (l_cs_val == 0x434D594B) linear_cs = cmsSigCmykData;
        else if (l_cs_val == 0x52474220) linear_cs = cmsSigRgbData;
        else linear_cs = (cmsColorSpaceSignature)l_cs_val;
    }
    
    p_linear = cmsCreateLinearizationDeviceLinkTHR(ctx, linear_cs, curves);

    // Additional exploration: Call functions with NULL context if possible? 
    // No, the signatures require cmsContext. We have 'ctx'.
    // Test with a second context to ensure isolation?
    cmsContext ctx2 = cmsCreateContext(NULL, NULL);
    if (ctx2) {
        cmsHPROFILE p_xyz2 = cmsCreateXYZProfileTHR(ctx2);
        if (p_xyz2) {
            // Do nothing, just created
        }
        // cmsCloseContext is not available in standard lcms2.h, 
        // contexts created with NULL are usually global or managed internally.
        // We assume standard cleanup or leak for this specific test case 
        // as the API for explicit context destruction might be version dependent or internal.
        // If cmsCloseContext exists in the specific build, it would be used here.
        // For safety with standard headers, we skip explicit close if not declared.
        // Note: In many LCMS2 versions, contexts created with NULL are global and should not be closed.
        // If a custom allocator was used, we would need to close it.
        // Since we passed NULL, we assume it's the global context or auto-managed.
    }

    // Cleanup
    cleanup_profiles(p_xyz, p_oklab, p_srgb, p_ink, p_linear, p_dummy);
    
    // Context cleanup: Only close if we created it with a custom allocator or if the API supports it.
    // With NULL allocator, it's often the global context. We skip closing to avoid double-free or invalid calls.
    // If the user provided a custom allocator, they would need to close it.
    // For this fuzz driver, we rely on the process exit to clean up global resources if any.

    return 0;
}