// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_handler(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext Context = cmsCreateContext(NULL, NULL);
    if (Context == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(Context);
    if (hIT8 == NULL) {
        cmsDeleteContext(Context);
        return 0;
    }

    // Fuzz cmsIT8SetPropertyStr
    if (Size > 2) {
        char key[256];
        char val[256];
        size_t keyLen = (Size / 2) < 255 ? (Size / 2) : 255;
        size_t valLen = (Size - keyLen) < 255 ? (Size - keyLen) : 255;
        memcpy(key, Data, keyLen);
        key[keyLen] = '\0';
        memcpy(val, Data + keyLen, valLen);
        val[valLen] = '\0';
        cmsIT8SetPropertyStr(hIT8, key, val);
    }

    // Fuzz cmsIT8SetPropertyMulti
    if (Size > 4) {
        char key[256];
        char subKey[256];
        char buffer[256];
        size_t partLen = Size / 3;
        if (partLen > 255) partLen = 255;
        memcpy(key, Data, partLen);
        key[partLen] = '\0';
        memcpy(subKey, Data + partLen, partLen);
        subKey[partLen] = '\0';
        memcpy(buffer, Data + 2 * partLen, partLen);
        buffer[partLen] = '\0';
        cmsIT8SetPropertyMulti(hIT8, key, subKey, buffer);
    }

    // Fuzz cmsIT8SetDataFormat
    if (Size > 0) {
        int n = Data[0] % 100;
        char format[256];
        size_t fmtLen = (Size - 1) < 255 ? (Size - 1) : 255;
        memcpy(format, Data + 1, fmtLen);
        format[fmtLen] = '\0';
        cmsIT8SetDataFormat(hIT8, n, format);
    }

    // Fuzz cmsIT8SetTableByLabel
    if (Size > 3) {
        char set[256];
        char field[256];
        char expected[256];
        size_t partLen = Size / 3;
        if (partLen > 255) partLen = 255;
        memcpy(set, Data, partLen);
        set[partLen] = '\0';
        memcpy(field, Data + partLen, partLen);
        field[partLen] = '\0';
        memcpy(expected, Data + 2 * partLen, partLen);
        expected[partLen] = '\0';
        cmsIT8SetTableByLabel(hIT8, set, field, expected);
    }

    // Fuzz cmsIT8GetDataRowCol
    if (Size > 1) {
        int row = (int)(Data[0] % 100);
        int col = (int)(Data[1] % 100);
        const char *result = cmsIT8GetDataRowCol(hIT8, row, col);
        (void)result;
    }

    // Fuzz cmsIT8EnumDataFormat
    char **SampleNames = NULL;
    int count = cmsIT8EnumDataFormat(hIT8, &SampleNames);
    if (count > 0 && SampleNames != NULL) {
        for (int i = 0; i < count; i++) {
            if (SampleNames[i] != NULL) {
                free(SampleNames[i]);
            }
        }
        free(SampleNames);
    }

    cleanup_handler(hIT8);
    cmsDeleteContext(Context);

    return 0;
}