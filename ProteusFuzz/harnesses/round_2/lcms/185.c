// Source: Generated Fuzz Driver for lcms ParseFloatNumber coverage
// Target: ParseFloatNumber in cmscgats.c
// Strategy: Inject various float string formats via cmsIT8SetDataRowCol 
// and retrieve them via cmsIT8GetDataRowColDbl to trigger ParseFloatNumber.

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

// Helper to safely set a string into the IT8 structure
static void set_it8_string(cmsHANDLE hIT8, int row, int col, const char *str) {
    // Ensure the string is null-terminated for safety
    if (str == NULL) return;
    cmsIT8SetDataRowCol(hIT8, row, col, str);
}

int LLVMFuzzerTestOneInput_185(const uint8_t *Data, size_t Size) {
    // Minimum size to ensure we have enough data for basic operations
    if (Size < 4) return 0;

    // 1. Setup Context and IT8 Handle
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // 2. Prepare test strings based on input data to cover uncovered lines
    // We construct strings to hit: signs, decimals, exponents, and edge cases.
    
    // Create a buffer for our test string (max 64 chars to avoid overflow)
    char test_str[64];
    memset(test_str, 0, sizeof(test_str));

    // Extract parameters from input to vary the string construction
    int row = Data[0] % 10;
    int col = Data[1] % 10;
    int pattern_type = Data[2] % 5;
    int sign_type = Data[3] % 3;

    // Construct different float patterns to cover all branches in ParseFloatNumber
    switch (pattern_type) {
        case 0: // Simple integer with sign
            if (sign_type == 0) sprintf(test_str, "-%d", (Data[4] % 1000));
            else if (sign_type == 1) sprintf(test_str, "+%d", (Data[4] % 1000));
            else sprintf(test_str, "%d", (Data[4] % 1000));
            break;
        
        case 1: // Decimal number
            if (sign_type == 0) sprintf(test_str, "-%d.%d", (Data[4] % 100), (Data[5] % 100));
            else if (sign_type == 1) sprintf(test_str, "+%d.%d", (Data[4] % 100), (Data[5] % 100));
            else sprintf(test_str, "%d.%d", (Data[4] % 100), (Data[5] % 100));
            break;

        case 2: // Scientific notation with positive exponent
            if (sign_type == 0) sprintf(test_str, "-%d.%dE+%d", (Data[4]%10), (Data[5]%10), (Data[6]%100));
            else if (sign_type == 1) sprintf(test_str, "+%d.%dE+%d", (Data[4]%10), (Data[5]%10), (Data[6]%100));
            else sprintf(test_str, "%d.%dE+%d", (Data[4]%10), (Data[5]%10), (Data[6]%100));
            break;

        case 3: // Scientific notation with negative exponent
            if (sign_type == 0) sprintf(test_str, "-%d.%dE-%d", (Data[4]%10), (Data[5]%10), (Data[6]%100));
            else if (sign_type == 1) sprintf(test_str, "+%d.%dE-%d", (Data[4]%10), (Data[5]%10), (Data[6]%100));
            else sprintf(test_str, "%d.%dE-%d", (Data[4]%10), (Data[5]%10), (Data[6]%100));
            break;

        case 4: // Edge cases: just sign, just dot, just E, or large exponent
            if (sign_type == 0) strcpy(test_str, "-");
            else if (sign_type == 1) strcpy(test_str, "+");
            else if (sign_type == 2) strcpy(test_str, ".");
            else if (sign_type == 3) strcpy(test_str, "E");
            else {
                // Try to trigger overflow check logic (line 777) with a large exponent
                // Format: 1.0E+999999999
                sprintf(test_str, "1.0E+999999999");
            }
            break;
    }

    // 3. Inject the string into the IT8 structure
    set_it8_string(hIT8, row, col, test_str);

    // 4. Trigger ParseFloatNumber via cmsIT8GetDataRowColDbl
    // This will call cmsIT8GetDataRowCol -> ParseFloatNumber(test_str)
    cmsFloat64Number result = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)result; // Suppress unused warning

    // 5. Additional calls to ensure other paths are exercised if needed
    // Try a few more variations if input size permits
    if (Size > 8) {
        int row2 = Data[4] % 10;
        int col2 = Data[5] % 10;
        char test_str2[64];
        // Force a complex number to ensure all loops run
        sprintf(test_str2, "-123.456E-78");
        set_it8_string(hIT8, row2, col2, test_str2);
        result = cmsIT8GetDataRowColDbl(hIT8, row2, col2);
        (void)result;
    }

    // 6. Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);

    return 0;
}