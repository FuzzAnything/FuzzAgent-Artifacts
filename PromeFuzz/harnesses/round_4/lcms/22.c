// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

static void init_dummy_it8(cmsHANDLE *hIT8) {
    *hIT8 = cmsIT8Alloc(NULL);
    if (*hIT8) {
        cmsIT8SetSheetType(*hIT8, "CGATS.17");
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsHANDLE hIT8 = NULL;
    init_dummy_it8(&hIT8);
    if (!hIT8) return 0;

    size_t data_len = Size;
    char *data_copy = (char *)malloc(data_len + 1);
    if (!data_copy) {
        cmsIT8Free(hIT8);
        return 0;
    }
    memcpy(data_copy, Data, data_len);
    data_copy[data_len] = '\0';

    size_t str_len = data_len / 6;
    if (str_len < 1) str_len = 1;

    char *str1 = data_copy;
    char *str2 = (str_len < data_len) ? str1 + str_len : str1;
    char *str3 = (2*str_len < data_len) ? str2 + str_len : str2;
    char *str4 = (3*str_len < data_len) ? str3 + str_len : str3;
    char *str5 = (4*str_len < data_len) ? str4 + str_len : str4;
    char *str6 = (5*str_len < data_len) ? str5 + str_len : str5;

    if (str1[0] != '\0') {
        cmsIT8SetComment(hIT8, str1);
    }

    cmsIT8SetTableByLabel(hIT8, str2, str3, str4);

    cmsIT8SetSheetType(hIT8, str5);

    cmsIT8SetIndexColumn(hIT8, str6);

    int row = (int)(Data[0] % 10);
    int col = (int)(Data[Size > 1 ? 1 : 0] % 10);
    const char *retrieved = cmsIT8GetDataRowCol(hIT8, row, col);
    (void)retrieved;

    int patch_idx = cmsIT8GetPatchByName(hIT8, str1);
    (void)patch_idx;

    free(data_copy);
    cmsIT8Free(hIT8);
    return 0;
}