// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_handle(cmsHANDLE hIT8, cmsContext ctx) {
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (!hIT8) {
        cmsDeleteContext(ctx);
        return 0;
    }

    const char *sample_str = (const char *)Data;
    size_t str_len = Size;
    
    char *safe_str = malloc(str_len + 1);
    if (!safe_str) {
        cleanup_handle(hIT8, ctx);
        return 0;
    }
    memcpy(safe_str, Data, str_len);
    safe_str[str_len] = '\0';

    cmsBool ret1 = cmsIT8SetIndexColumn(hIT8, safe_str);
    (void)ret1;

    int n = (Size > 0) ? (Data[0] % 100) : 0;
    cmsBool ret2 = cmsIT8SetDataFormat(hIT8, n, safe_str);
    (void)ret2;

    cmsUInt32Number hex_val = (Size > 1) ? ((Data[0] << 8) | Data[1]) : 0;
    cmsBool ret3 = cmsIT8SetPropertyHex(hIT8, "TestProp", hex_val);
    (void)ret3;

    cmsUInt32Number table_idx = (Size > 2) ? (Data[2] % 10) : 0;
    cmsInt32Number ret4 = cmsIT8SetTable(hIT8, table_idx);
    (void)ret4;

    int row = (Size > 3) ? (Data[3] % 50) : 0;
    int col = (Size > 4) ? (Data[4] % 50) : 0;
    cmsBool ret5 = cmsIT8SetDataRowCol(hIT8, row, col, safe_str);
    (void)ret5;

    cmsUInt32Number bytes_needed = 0;
    cmsBool ret6 = cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
    if (ret6 && bytes_needed > 0) {
        void *mem_buf = malloc(bytes_needed);
        if (mem_buf) {
            cmsBool ret7 = cmsIT8SaveToMem(hIT8, mem_buf, &bytes_needed);
            (void)ret7;
            free(mem_buf);
        }
    }
    (void)ret6;

    free(safe_str);
    cleanup_handle(hIT8, ctx);

    return 0;
}