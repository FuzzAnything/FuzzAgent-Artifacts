// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void free_memory(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size < 7) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Extract data from input for fuzzing parameters safely
    int n = (int)(Data[0] % 100);
    int row = (int)(Data[1] % 100);
    int col = (int)(Data[2] % 100);
    cmsUInt32Number val = ((cmsUInt32Number)Data[3] << 24) | 
                          ((cmsUInt32Number)Data[4] << 16) | 
                          ((cmsUInt32Number)Data[5] << 8) | 
                          (cmsUInt32Number)Data[6];

    // Prepare string data safely
    char comment[256];
    char sample[256];
    char property[64];
    char cell_val[256];

    size_t str_len = (Size > 7) ? (Size - 7) : 0;
    if (str_len > sizeof(comment) - 1) str_len = sizeof(comment) - 1;
    memcpy(comment, Data + 7, str_len);
    comment[str_len] = '\0';

    if (str_len > sizeof(sample) - 1) str_len = sizeof(sample) - 1;
    memcpy(sample, Data + 7, str_len);
    sample[str_len] = '\0';

    if (str_len > sizeof(property) - 1) str_len = sizeof(property) - 1;
    memcpy(property, Data + 7, str_len);
    property[str_len] = '\0';

    if (str_len > sizeof(cell_val) - 1) str_len = sizeof(cell_val) - 1;
    memcpy(cell_val, Data + 7, str_len);
    cell_val[str_len] = '\0';

    // Fuzz cmsIT8SetComment
    cmsIT8SetComment(hIT8, comment);

    // Fuzz cmsIT8SetDataFormat
    cmsIT8SetDataFormat(hIT8, n, sample);

    // Fuzz cmsIT8SetTable
    cmsIT8SetTable(hIT8, (cmsUInt32Number)n);

    // Fuzz cmsIT8SetDataRowCol
    cmsIT8SetDataRowCol(hIT8, row, col, cell_val);

    // Fuzz cmsIT8SetPropertyHex
    cmsIT8SetPropertyHex(hIT8, property, val);

    // Fuzz cmsIT8SaveToMem
    cmsUInt32Number bytesNeeded = 0;
    if (!cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded)) {
        cmsIT8Free(hIT8);
        cmsDeleteContext(ContextID);
        return 0;
    }

    if (bytesNeeded > 0) {
        void *memPtr = malloc(bytesNeeded);
        if (memPtr != NULL) {
            cmsIT8SaveToMem(hIT8, memPtr, &bytesNeeded);
            free_memory(memPtr);
        }
    }

    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);
    return 0;
}