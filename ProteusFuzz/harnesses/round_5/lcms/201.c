// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void write_dummy_it8_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal valid IT8 file header
    const char header[] = "IT8.7/1.0";
    fwrite(header, 1, strlen(header), f);
    
    // Pad to a reasonable size
    char padding[1024] = {0};
    fwrite(padding, 1, sizeof(padding), f);
    
    fclose(f);
}

static void cleanup_dummy_file(const char *filename) {
    remove(filename);
}

int LLVMFuzzerTestOneInput_201(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    const char *dummy_file = "./dummy_file";
    write_dummy_it8_file(dummy_file);

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        cleanup_dummy_file(dummy_file);
        return 0;
    }

    // Use cmsIT8Alloc to create a valid handle for testing functions that require a non-NULL handle.
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    
    if (!hIT8) {
        cmsDeleteContext(ctx);
        cleanup_dummy_file(dummy_file);
        return 0;
    }

    char sample_name[256] = {0};
    char prop_name[256] = {0};
    char color_name[256] = {0};
    char str1[256] = {0};
    char str2[256] = {0};

    // Safe copy for sample_name
    size_t copy_len = Size < 255 ? Size : 255;
    memcpy(sample_name, Data, copy_len);
    sample_name[copy_len] = '\0';
    
    // Safe copy for prop_name
    if (Size > 1) {
        copy_len = Size - 1 < 255 ? Size - 1 : 255;
        memcpy(prop_name, Data + 1, copy_len);
        prop_name[copy_len] = '\0';
    }

    // Safe copy for color_name
    if (Size > 2) {
        copy_len = Size - 2 < 255 ? Size - 2 : 255;
        memcpy(color_name, Data + 2, copy_len);
        color_name[copy_len] = '\0';
    }

    // Safe copy for str1
    if (Size > 3) {
        copy_len = Size - 3 < 255 ? Size - 3 : 255;
        memcpy(str1, Data + 3, copy_len);
        str1[copy_len] = '\0';
    }

    // Safe copy for str2
    if (Size > 4) {
        copy_len = Size - 4 < 255 ? Size - 4 : 255;
        memcpy(str2, Data + 4, copy_len);
        str2[copy_len] = '\0';
    }

    // 1. Fuzz cmsIT8FindDataFormat
    int format_id = cmsIT8FindDataFormat(hIT8, sample_name);
    (void)format_id;

    // 2. Fuzz cmsIT8SetIndexColumn
    cmsBool set_result = cmsIT8SetIndexColumn(hIT8, sample_name);
    (void)set_result;

    // 3. Fuzz cmsIT8GetDataRowCol
    int row = (int)(Data[0] % 100) - 50;
    int col = (int)(Data[1] % 100) - 50;
    const char *cell_data = cmsIT8GetDataRowCol(hIT8, row, col);
    if (cell_data) {
        (void)strlen(cell_data);
    }

    // 4. Fuzz cmsIT8SetPropertyHex
    // Ensure we have at least 4 bytes before accessing Data[3]
    cmsUInt32Number hex_val = (cmsUInt32Number)(Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3]);
    cmsBool prop_result = cmsIT8SetPropertyHex(hIT8, prop_name, hex_val);
    (void)prop_result;

    // 5. Fuzz cmsNamedColorIndex
    cmsNAMEDCOLORLIST *ncList = cmsAllocNamedColorList(ctx, 0, 0, "Prefix", "Suffix");
    if (ncList) {
        cmsInt32Number index = cmsNamedColorIndex(ncList, color_name);
        (void)index;
        cmsFreeNamedColorList(ncList);
    }

    // 6. Fuzz cmsstrcasecmp
    int cmp_result = cmsstrcasecmp(str1, str2);
    (void)cmp_result;

    // Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(ctx);
    cleanup_dummy_file(dummy_file);

    return 0;
}