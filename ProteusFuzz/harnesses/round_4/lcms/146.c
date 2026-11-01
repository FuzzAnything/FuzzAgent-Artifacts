// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void fuzz_named_color_list(cmsNAMEDCOLORLIST* list, const uint8_t* data, size_t size) {
    if (!list) return;

    // Fuzz cmsAppendNamedColor
    size_t idx = 0;
    while (idx + 4 < size) {
        cmsUInt16Number pcs[3] = {0, 0, 0};
        cmsUInt16Number colorant[cmsMAXCHANNELS] = {0};
        char name[33] = {0};

        // Extract name length
        uint8_t name_len = data[idx];
        if (name_len > 32) name_len = 32;
        idx++;

        if (idx + name_len <= size) {
            memcpy(name, data + idx, name_len);
            idx += name_len;
        }

        // Extract PCS values if available
        if (idx + 6 <= size) {
            pcs[0] = data[idx++];
            pcs[1] = data[idx++];
            pcs[2] = data[idx++];
            pcs[0] |= (data[idx++] << 8);
            pcs[1] |= (data[idx++] << 8);
            pcs[2] |= (data[idx++] << 8);
        }

        // Extract colorant values if available
        if (idx + 2 <= size) {
            colorant[0] = data[idx++];
            colorant[0] |= (data[idx++] << 8);
        }

        cmsAppendNamedColor(list, name, pcs, colorant);
    }

    // Fuzz cmsNamedColorIndex
    if (size > 0) {
        char search_name[33] = {0};
        uint8_t search_len = data[size - 1];
        if (search_len > 32) search_len = 32;
        if (size > search_len) {
            memcpy(search_name, data + (size - search_len - 1), search_len);
        }
        cmsNamedColorIndex(list, search_name);
    }

    // Fuzz cmsDupNamedColorList
    cmsNAMEDCOLORLIST* dup_list = cmsDupNamedColorList(list);
    if (dup_list) {
        cmsFreeNamedColorList(dup_list);
    }

    // Fuzz cmsNamedColorInfo
    // Since we cannot access internal fields directly, we try indices based on input data
    // We assume the list might have grown due to cmsAppendNamedColor calls
    if (size > 0) {
        char name_buf[33] = {0};
        char prefix_buf[33] = {0};
        char suffix_buf[33] = {0};
        cmsUInt16Number pcs_buf[3] = {0};
        cmsUInt16Number colorant_buf[cmsMAXCHANNELS] = {0};

        // Try a few indices based on input data to trigger different paths
        cmsUInt32Number test_idx = data[0];
        cmsNamedColorInfo(list, test_idx, name_buf, prefix_buf, suffix_buf, pcs_buf, colorant_buf);
        
        // Try index 0
        cmsNamedColorInfo(list, 0, name_buf, prefix_buf, suffix_buf, pcs_buf, colorant_buf);
    }
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Use cmsAllocNamedColorList to create a new named color list.
    // Signature: cmsNAMEDCOLORLIST* cmsAllocNamedColorList(cmsContext ContextID, cmsUInt32Number n, cmsUInt32Number ColorantCount, const char* Prefix, const char* Suffix);
    cmsNAMEDCOLORLIST* list = cmsAllocNamedColorList(ctx, 0, 0, "", "");
    if (!list) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzz the named color list directly
    fuzz_named_color_list(list, Data, Size);

    cmsFreeNamedColorList(list);
    cmsDeleteContext(ctx);

    return 0;
}