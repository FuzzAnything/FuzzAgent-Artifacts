// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) {
        return 0;
    }

    // Prepare strings from fuzzer input
    // We need to ensure null-terminated strings, so we'll use a buffer
    char *buffer = NULL;
    if (Size > 0) {
        buffer = (char *)malloc(Size + 1);
        if (!buffer) {
            cmsIT8Free(hIT8);
            return 0;
        }
        memcpy(buffer, Data, Size);
        buffer[Size] = '\0';
    } else {
        // Empty input case
        buffer = (char *)malloc(1);
        if (!buffer) {
            cmsIT8Free(hIT8);
            return 0;
        }
        buffer[0] = '\0';
    }

    // Extract some values for row/col from the input
    int row = 0;
    int col = 0;
    if (Size >= 2) {
        row = Data[0];
        col = Data[1];
    }

    // Call target functions with various parameters
    // 1. cmsIT8SetDataRowCol
    if (Size > 0) {
        cmsIT8SetDataRowCol(hIT8, row, col, buffer);
    }

    // 2. cmsIT8SetPropertyUncooked
    cmsIT8SetPropertyUncooked(hIT8, "Key1", buffer);
    cmsIT8SetPropertyUncooked(hIT8, buffer, "Value1");

    // 3. cmsIT8SetComment
    if (Size > 0) {
        cmsIT8SetComment(hIT8, buffer);
    }

    // 4. cmsIT8FindDataFormat
    int format_index = cmsIT8FindDataFormat(hIT8, buffer);
    // Use the result if valid
    if (format_index >= 0) {
        cmsIT8SetDataRowCol(hIT8, row, format_index, "test");
    }

    // 5. cmsIT8SetPropertyMulti
    cmsIT8SetPropertyMulti(hIT8, "MultiKey", "SubKey", buffer);
    cmsIT8SetPropertyMulti(hIT8, buffer, "SubKey2", "Value2");

    // 6. cmsIT8SetPropertyStr
    if (Size > 0) {
        cmsIT8SetPropertyStr(hIT8, "Property1", buffer);
        cmsIT8SetPropertyStr(hIT8, buffer, "StringValue");
    }

    // Cleanup
    free(buffer);
    cmsIT8Free(hIT8);
    
    return 0;
}