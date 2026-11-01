// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_transform(cmsHTRANSFORM xform) {
    if (xform) {
        cmsDeleteTransform(xform);
    }
}

static void cleanup_list(cmsNAMEDCOLORLIST* list) {
    if (list) {
        cmsFreeNamedColorList(list);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsNAMEDCOLORLIST* list = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsNAMEDCOLORLIST* list_from_xform = NULL;
    cmsHPROFILE hProfile = NULL;

    // Initialize buffers for cmsNamedColorInfo
    char name_buf[256] = {0};
    char prefix_buf[256] = {0};
    char suffix_buf[256] = {0};
    cmsUInt16Number pcs_buf[4] = {0};
    cmsUInt16Number colorant_buf[16] = {0};

    // Test cmsDupNamedColorList with NULL
    cmsNAMEDCOLORLIST* dup_null = cmsDupNamedColorList(NULL);
    if (dup_null) {
        cmsFreeNamedColorList(dup_null);
    }

    // Try to create a profile from dummy memory using cmsOpenProfileFromMem
    // We construct a minimal ICC profile header (128 bytes)
    unsigned char dummy_icc[128] = {0};
    // Set magic number for ICC profile at offset 36
    dummy_icc[36] = 'a'; dummy_icc[37] = 'c'; dummy_icc[38] = 's'; dummy_icc[39] = 'p';
    // Set profile size (4 bytes at offset 0) to 128
    dummy_icc[0] = 0; dummy_icc[1] = 0; dummy_icc[2] = 0; dummy_icc[3] = 128;

    hProfile = cmsOpenProfileFromMem(dummy_icc, 128);
    if (hProfile) {
        cmsUInt32Number inputType = TYPE_RGB_8;
        cmsUInt32Number outputType = TYPE_RGB_8;
        // Intent is cmsUInt32Number, use INTENT_PERCEPTUAL (0)
        xform = cmsCreateTransform(hProfile, inputType, hProfile, outputType, INTENT_PERCEPTUAL, 0);
        if (xform) {
            list_from_xform = cmsGetNamedColorList(xform);
            if (list_from_xform) {
                cmsNamedColorInfo(list_from_xform, 0, name_buf, prefix_buf, suffix_buf, pcs_buf, colorant_buf);
                cmsNamedColorInfo(list_from_xform, 1000, name_buf, prefix_buf, suffix_buf, pcs_buf, colorant_buf);
                cmsNamedColorInfo(list_from_xform, 0, NULL, NULL, NULL, NULL, NULL);
                
                // Test cmsNamedColorIndex
                cmsInt32Number idx = cmsNamedColorIndex(list_from_xform, "Test");
                cmsInt32Number idx_null = cmsNamedColorIndex(list_from_xform, NULL);
                (void)idx;
                (void)idx_null;
                
                // Test cmsDupNamedColorList
                cmsNAMEDCOLORLIST* dup_list = cmsDupNamedColorList(list_from_xform);
                if (dup_list) {
                    cmsFreeNamedColorList(dup_list);
                }
            }
        }
    }

    // Test functions with NULL inputs directly
    cmsNamedColorInfo(NULL, 0, name_buf, prefix_buf, suffix_buf, pcs_buf, colorant_buf);
    cmsNamedColorInfo(NULL, 0, NULL, NULL, NULL, NULL, NULL);
    
    cmsInt32Number idx_null_list = cmsNamedColorIndex(NULL, "Test");
    cmsInt32Number idx_null_name = cmsNamedColorIndex(NULL, NULL);
    (void)idx_null_list;
    (void)idx_null_name;
    
    cmsNAMEDCOLORLIST* dup_null_list = cmsDupNamedColorList(NULL);
    if (dup_null_list) cmsFreeNamedColorList(dup_null_list);
    
    cmsFreeNamedColorList(NULL);
    
    // Test cmsAppendNamedColor with NULL list
    cmsUInt16Number pcs[3] = {0};
    cmsUInt16Number colorant[16] = {0};
    cmsBool res = cmsAppendNamedColor(NULL, "Test", pcs, colorant);
    (void)res;

    // Cleanup
    cleanup_transform(xform);
    cleanup_profile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}