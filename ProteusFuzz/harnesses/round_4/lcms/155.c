// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_it8(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
}

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // cmsCreateContext requires two arguments: Plugin and UserData
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Extract parameters from fuzz input
    int n = (Size > 0) ? (int)Data[0] : 0;
    int row = (Size > 1) ? (int)Data[1] : 0;
    int col = (Size > 2) ? (int)Data[2] : 0;
    cmsFloat64Number val = (Size > 3) ? (double)Data[3] : 0.0;

    // Create sample strings from input data
    char sample[64];
    char patch[64];
    char prop[64];
    char valStr[64];

    size_t sampleLen = (Size > 4) ? (Size - 4) : 0;
    size_t patchLen = (Size > 4 + sampleLen) ? (Size - 4 - sampleLen) : 0;
    size_t propLen = (Size > 4 + sampleLen + patchLen) ? (Size - 4 - sampleLen - patchLen) : 0;
    size_t valStrLen = (Size > 4 + sampleLen + patchLen + propLen) ? (Size - 4 - sampleLen - patchLen - propLen) : 0;

    if (sampleLen > 0 && sampleLen < sizeof(sample)) {
        memcpy(sample, Data + 4, sampleLen);
        sample[sampleLen] = '\0';
    } else {
        strcpy(sample, "Sample");
    }

    if (patchLen > 0 && patchLen < sizeof(patch)) {
        memcpy(patch, Data + 4 + sampleLen, patchLen);
        patch[patchLen] = '\0';
    } else {
        strcpy(patch, "Patch");
    }

    if (propLen > 0 && propLen < sizeof(prop)) {
        memcpy(prop, Data + 4 + sampleLen + patchLen, propLen);
        prop[propLen] = '\0';
    } else {
        strcpy(prop, "Property");
    }

    if (valStrLen > 0 && valStrLen < sizeof(valStr)) {
        memcpy(valStr, Data + 4 + sampleLen + patchLen + propLen, valStrLen);
        valStr[valStrLen] = '\0';
    } else {
        strcpy(valStr, "0.0");
    }

    // Fuzz cmsIT8SetDataFormat
    cmsIT8SetDataFormat(hIT8, n, sample);

    // Fuzz cmsIT8SetDataRowColDbl
    cmsIT8SetDataRowColDbl(hIT8, row, col, val);

    // Fuzz cmsIT8GetDataRowColDbl
    cmsFloat64Number retrievedVal = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)retrievedVal; // Suppress unused variable warning

    // Fuzz cmsIT8SetDataDbl
    cmsIT8SetDataDbl(hIT8, patch, sample, val);

    // Fuzz cmsIT8SetDataRowCol
    cmsIT8SetDataRowCol(hIT8, row, col, valStr);

    // Fuzz cmsIT8SetPropertyDbl
    cmsIT8SetPropertyDbl(hIT8, prop, val);

    // Cleanup
    cleanup_it8(hIT8);
    cmsDeleteContext(ContextID);

    return 0;
}