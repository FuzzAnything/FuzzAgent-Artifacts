// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHANDLE create_it8_handle(void) {
    cmsHANDLE h = cmsIT8Alloc(NULL);
    if (h == NULL) return NULL;
    // Initialize with a basic patch/sample to ensure some state exists
    cmsIT8SetData(h, "P1", "S1", "0.0");
    return h;
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsHANDLE hIT8 = create_it8_handle();
    if (hIT8 == NULL) return 0;

    // Extract parameters from fuzz data
    int row = (int)Data[0];
    int col = (int)Data[1];
    int sample_idx = (int)Data[1];

    // Create a double value from fuzz data safely
    cmsFloat64Number val = 0.0;
    if (Size >= 10) {
        // Ensure we have enough bytes to safely read 8 bytes for double
        // We use Data[2] to Data[9] (8 bytes)
        uint64_t temp_val;
        memcpy(&temp_val, Data + 2, sizeof(uint64_t));
        memcpy(&val, &temp_val, sizeof(double));
    } else {
        val = (double)Data[0] / 255.0;
    }

    // Fuzz cmsIT8SetDataDbl
    char patch[16], sample[16];
    if (Size > 10) {
        size_t p_len = (Size > 20) ? 20 : Size - 10;
        if (p_len > 15) p_len = 15;
        memcpy(patch, Data + 10, p_len);
        patch[p_len] = '\0';
        size_t s_len = (Size > 20 + p_len) ? 20 : Size - 10 - p_len;
        if (s_len > 15) s_len = 15;
        memcpy(sample, Data + 10 + p_len, s_len);
        sample[s_len] = '\0';
        cmsIT8SetDataDbl(hIT8, patch, sample, val);
    }

    // Fuzz cmsIT8SetPropertyDbl
    char prop[16];
    if (Size > 30) {
        size_t prop_len = (Size > 45) ? 15 : Size - 30;
        memcpy(prop, Data + 30, prop_len);
        prop[prop_len] = '\0';
        cmsIT8SetPropertyDbl(hIT8, prop, val);
    }

    // Fuzz cmsIT8SetDataRowCol
    char cell_val[16];
    if (Size > 40) {
        size_t cv_len = (Size > 55) ? 15 : Size - 40;
        memcpy(cell_val, Data + 40, cv_len);
        cell_val[cv_len] = '\0';
        cmsIT8SetDataRowCol(hIT8, row, col, cell_val);
    }

    // Fuzz cmsIT8SetDataFormat
    char format[16];
    if (Size > 50) {
        size_t fmt_len = (Size > 65) ? 15 : Size - 50;
        memcpy(format, Data + 50, fmt_len);
        format[fmt_len] = '\0';
        cmsIT8SetDataFormat(hIT8, sample_idx, format);
    }

    // Fuzz cmsIT8SetDataRowColDbl
    cmsIT8SetDataRowColDbl(hIT8, row, col, val);

    // Fuzz cmsIT8GetDataRowColDbl
    cmsFloat64Number result = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)result; // Use the result to avoid unused variable warning

    // Cleanup
    cmsIT8Free(hIT8);

    return 0;
}