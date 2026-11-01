// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context for IT8 allocation
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Safely extract fuzzing parameters from input
    int row = 0;
    int col = 0;
    int n = 0;
    
    if (Size > 0) row = (Data[0] % 100);
    if (Size > 1) col = (Data[1] % 100);
    if (Size > 2) n = (Data[2] % 10);
    
    // Create a double value from input bytes
    cmsFloat64Number val = 0.0;
    if (Size >= 8) {
        memcpy(&val, Data, 8);
    } else if (Size >= 2) {
        val = (double)(Data[0] + Data[1] * 256.0);
    } else if (Size >= 1) {
        val = (double)Data[0];
    }

    // Create string values from input
    char strVal[64];
    char patch[64];
    char sample[64];
    char prop[64];
    char formatStr[64];

    // Initialize strings to empty
    memset(strVal, 0, sizeof(strVal));
    memset(patch, 0, sizeof(patch));
    memset(sample, 0, sizeof(sample));
    memset(prop, 0, sizeof(prop));
    memset(formatStr, 0, sizeof(formatStr));

    // Safe copy with bounds checking
    size_t copyLen = (Size > 60) ? 60 : Size;
    if (Size > 0) {
        memcpy(strVal, Data, copyLen);
        strVal[copyLen] = '\0';
    }
    if (Size > 0) {
        memcpy(patch, Data, copyLen);
        patch[copyLen] = '\0';
    }
    if (Size > 0) {
        memcpy(sample, Data, copyLen);
        sample[copyLen] = '\0';
    }
    if (Size > 0) {
        memcpy(prop, Data, copyLen);
        prop[copyLen] = '\0';
    }
    if (Size > 0) {
        memcpy(formatStr, Data, copyLen);
        formatStr[copyLen] = '\0';
    }

    // Fuzz cmsIT8SetDataRowColDbl
    cmsIT8SetDataRowColDbl(hIT8, row, col, val);

    // Fuzz cmsIT8SetDataDbl
    cmsIT8SetDataDbl(hIT8, patch, sample, val);

    // Fuzz cmsIT8SetDataFormat
    cmsIT8SetDataFormat(hIT8, n, formatStr);

    // Fuzz cmsIT8GetDataRowColDbl
    cmsFloat64Number retrievedVal = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)retrievedVal; // Avoid unused variable warning

    // Fuzz cmsIT8SetDataRowCol
    cmsIT8SetDataRowCol(hIT8, row, col, strVal);

    // Fuzz cmsIT8SetPropertyDbl
    cmsIT8SetPropertyDbl(hIT8, prop, val);

    // Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);

    // Write dummy file if needed
    write_dummy_file(Data, Size);

    return 0;
}