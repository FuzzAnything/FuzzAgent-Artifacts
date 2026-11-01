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

int LLVMFuzzerTestOneInput_217(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Prepare a safe string from input to test parsing logic
    // We limit size to prevent buffer overflow and ensure null termination
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    size_t len = (Size < 255) ? Size : 255;
    memcpy(buffer, Data, len);
    buffer[len] = '\0';

    // 1. Trigger Line 719 (NULL check) via cmsIT8GetDataDbl
    // cmsIT8GetDataDbl does not check for NULL before calling ParseFloatNumber.
    // If the patch/sample does not exist, cmsIT8GetData returns NULL.
    cmsFloat64Number val_null = cmsIT8GetDataDbl(hIT8, "NONEXISTENT_PATCH", "NONEXISTENT_SAMPLE");
    (void)val_null;

    // 2. Trigger Lines 721-780 (Sign, Decimal, Exponent parsing)
    // Use cmsIT8SetDataRowCol to inject the fuzzed string into the grid.
    // The fuzzer will generate strings with signs, decimals, and exponents.
    int row = 0;
    int col = 0;
    
    cmsIT8SetDataRowCol(hIT8, row, col, buffer);
    
    // Retrieve and parse (triggers ParseFloatNumber with the string)
    cmsFloat64Number val_parsed = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)val_parsed;

    // Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);

    write_dummy_file(Data, Size);

    return 0;
}