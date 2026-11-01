// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Use input data to drive fuzzing parameters safely
    int row = Data[0] % 100;
    int col = Data[1] % 100;
    int n = Data[2] % 10;
    cmsFloat64Number val = (cmsFloat64Number)(Data[3] ^ Data[0]);
    const char *patch = "Patch";
    const char *sample = "Sample";
    const char *prop = "Property";
    const char *format = "Format";
    const char *strVal = "Value";

    // Fuzz cmsIT8SetDataFormat
    cmsIT8SetDataFormat(hIT8, n, format);

    // Fuzz cmsIT8SetDataDbl
    cmsIT8SetDataDbl(hIT8, patch, sample, val);

    // Fuzz cmsIT8SetDataRowCol
    cmsIT8SetDataRowCol(hIT8, row, col, strVal);

    // Fuzz cmsIT8SetPropertyDbl
    cmsIT8SetPropertyDbl(hIT8, prop, val);

    // Fuzz cmsIT8SetDataRowColDbl
    cmsIT8SetDataRowColDbl(hIT8, row, col, val);

    // Fuzz cmsIT8GetDataRowColDbl
    cmsFloat64Number result = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)result; // Suppress unused variable warning

    // Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);

    return 0;
}