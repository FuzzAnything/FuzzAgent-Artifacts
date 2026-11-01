#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

int LLVMFuzzerTestOneInput_184(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Construct a test string to exercise ParseFloatNumber paths
    char test_buf[64];
    int idx = 0;

    // 1. Sign handling (Lines 721, 723)
    if (Data[0] % 3 == 0) {
        test_buf[idx++] = '-';
    } else if (Data[0] % 3 == 1) {
        test_buf[idx++] = '+';
    }

    // 2. Integer part (Lines 728, 731)
    int int_len = (Data[1] % 5) + 1;
    for (int i = 0; i < int_len; i++) {
        test_buf[idx++] = '0' + (Data[2 + i] % 10);
    }

    // 3. Decimal part (Lines 734, 739, 741, 745)
    if (Data[2] % 2 == 1) {
        test_buf[idx++] = '.';
        int frac_len = (Data[3] % 5) + 1;
        for (int i = 0; i < frac_len; i++) {
            test_buf[idx++] = '0' + (Data[4 + i] % 10);
        }
    }

    // 4. Exponent part (Lines 752, 757, 760, 763, 766, 769, 773, 777, 780)
    if (Data[4] % 2 == 1) {
        test_buf[idx++] = (Data[5] % 2 == 0) ? 'E' : 'e';
        if (Data[6] % 3 == 0) {
            test_buf[idx++] = '-';
        } else if (Data[6] % 3 == 1) {
            test_buf[idx++] = '+';
        }
        int exp_len = (Data[7] % 3) + 1;
        for (int i = 0; i < exp_len; i++) {
            test_buf[idx++] = '0' + (Data[8 + i] % 10);
        }
    }

    test_buf[idx] = '\0';

    // Fallback if empty
    if (idx == 0) {
        test_buf[0] = '0';
        test_buf[1] = '\0';
    }

    // Inject the string into the IT8 structure
    int row = Data[0] % 100;
    int col = Data[1] % 100;
    cmsIT8SetDataRowCol(hIT8, row, col, test_buf);

    // Trigger ParseFloatNumber via cmsIT8GetDataRowColDbl
    cmsFloat64Number result1 = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)result1;

    // Also exercise other call chains: cmsIT8GetDataDbl and cmsIT8GetPropertyDbl
    // These also call ParseFloatNumber internally
    const char *patch = "Patch";
    const char *sample = "Sample";
    cmsIT8SetDataDbl(hIT8, patch, sample, result1);
    cmsFloat64Number result2 = cmsIT8GetDataDbl(hIT8, patch, sample);
    (void)result2;

    char prop_name[32];
    snprintf(prop_name, sizeof(prop_name), "Prop%d", row);
    cmsIT8SetPropertyDbl(hIT8, prop_name, result1);
    cmsFloat64Number result3 = cmsIT8GetPropertyDbl(hIT8, prop_name);
    (void)result3;

    // Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);

    return 0;
}