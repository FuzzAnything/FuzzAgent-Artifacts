// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzz input
    cmsUInt32Number n = (Size > 0) ? Data[0] : 0;
    cmsUInt32Number ColorantCount = (Size > 1) ? Data[1] : 0;
    
    // Limit values to reasonable ranges to avoid immediate allocation failures or overflows
    if (n > 1000) n = 1000;
    if (ColorantCount > cmsMAXCHANNELS) ColorantCount = cmsMAXCHANNELS;
    if (ColorantCount == 0) ColorantCount = 1;

    // Create a valid context if needed
    cmsContext ctx = NULL;
    // Use a deterministic condition based on input to decide context creation
    if (Data[2] != 0) {
        ctx = cmsCreateContext(NULL, NULL);
        if (!ctx) ctx = NULL;
    }

    // Generate prefix and suffix strings from input data
    char prefix[33] = {0};
    char suffix[33] = {0};
    
    // Ensure we have enough data before reading lengths
    if (Size > 3) {
        size_t prefix_len = Data[3];
        if (prefix_len > 32) prefix_len = 32;
        // Ensure we have enough data for the prefix content
        if (Size > 3 + prefix_len) {
            memcpy(prefix, Data + 4, prefix_len);
        }
    }

    if (Size > 4) {
        size_t suffix_len = Data[4];
        if (suffix_len > 32) suffix_len = 32;
        // Ensure we have enough data for the suffix content
        if (Size > 4 + suffix_len) {
            memcpy(suffix, Data + 5, suffix_len);
        }
    }

    // 1. Allocate Named Color List
    cmsNAMEDCOLORLIST *list = cmsAllocNamedColorList(ctx, n, ColorantCount, prefix, suffix);
    if (!list) {
        if (ctx) cmsDeleteContext(ctx);
        return 0;
    }

    // 2. Get Count
    cmsUInt32Number count = cmsNamedColorCount(list);
    (void)count; // Use to suppress unused variable warning

    // 3. Duplicate List
    cmsNAMEDCOLORLIST *dup_list = cmsDupNamedColorList(list);
    if (dup_list) {
        cmsNamedColorCount(dup_list);
        cmsFreeNamedColorList(dup_list);
    }

    // 4. Test cmsNamedColorInfo with various indices
    char name_buf[256] = {0};
    char prefix_buf[33] = {0};
    char suffix_buf[33] = {0};
    cmsUInt16Number pcs_buf[8] = {0};
    cmsUInt16Number colorant_buf[8] = {0};

    // Test valid index if list is not empty
    if (count > 0) {
        cmsNamedColorInfo(list, 0, name_buf, prefix_buf, suffix_buf, pcs_buf, colorant_buf);
        // Test out of bounds
        cmsNamedColorInfo(list, count + 1, name_buf, prefix_buf, suffix_buf, pcs_buf, colorant_buf);
    }
    
    // Test with NULL list
    cmsNamedColorInfo(NULL, 0, name_buf, prefix_buf, suffix_buf, pcs_buf, colorant_buf);

    // 5. Test cmsGetNamedColorList with a dummy transform (will likely return NULL, which is safe)
    cmsHTRANSFORM xform = NULL; // We cannot easily create a valid transform with named color without more complex setup
    cmsNAMEDCOLORLIST *from_xform = cmsGetNamedColorList(xform);
    if (from_xform) {
        cmsFreeNamedColorList(from_xform);
    }

    // Cleanup
    cmsFreeNamedColorList(list);
    if (ctx) cmsDeleteContext(ctx);

    // Write dummy file if data is substantial
    if (Size > 10) {
        write_dummy_file(Data, Size);
    }

    return 0;
}