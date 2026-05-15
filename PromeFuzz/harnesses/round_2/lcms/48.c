// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 256

static cmsHANDLE create_test_it8_handle(void) {
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return NULL;
    
    // Create a basic table structure to work with
    cmsIT8SetTable(hIT8, 0);
    cmsIT8SetSheetType(hIT8, "SAMPLE");
    
    // Add some columns
    cmsIT8SetPropertyStr(hIT8, "NUMBER_OF_FIELDS", "4");
    cmsIT8SetPropertyStr(hIT8, "FIELD_0", "SAMPLE_ID");
    cmsIT8SetPropertyStr(hIT8, "FIELD_1", "X");
    cmsIT8SetPropertyStr(hIT8, "FIELD_2", "Y");
    cmsIT8SetPropertyStr(hIT8, "FIELD_3", "Z");
    
    // Add some data rows using patch names and sample names
    for (int i = 0; i < 5; i++) {
        char patch_name[32];
        snprintf(patch_name, sizeof(patch_name), "Patch_%d", i);
        cmsIT8SetData(hIT8, patch_name, "SAMPLE_ID", patch_name);
        cmsIT8SetData(hIT8, patch_name, "X", "0.5");
        cmsIT8SetData(hIT8, patch_name, "Y", "0.5");
        cmsIT8SetData(hIT8, patch_name, "Z", "0.5");
    }
    
    return hIT8;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsHANDLE hIT8 = create_test_it8_handle();
    if (!hIT8) return 0;
    
    // Use first byte to determine which function to test
    uint8_t selector = Data[0] % 6;
    
    // Prepare null-terminated string from fuzzer input
    char *input_str = NULL;
    if (Size > 1) {
        input_str = (char*)malloc(Size);
        if (input_str) {
            memcpy(input_str, Data + 1, Size - 1);
            input_str[Size - 1] = '\0';
        }
    }
    
    switch (selector) {
        case 0: {
            // Test cmsIT8SetIndexColumn
            if (input_str) {
                cmsIT8SetIndexColumn(hIT8, input_str);
            }
            break;
        }
        case 1: {
            // Test cmsIT8GetPatchName
            int patch_idx = (Size > 1) ? (Data[1] % 10) : 0;
            char buffer[MAXSTR];
            const char* result = cmsIT8GetPatchName(hIT8, patch_idx, buffer);
            if (result) {
                // Just use the result to avoid dead code elimination
                volatile const char* unused = result;
                (void)unused;
            }
            break;
        }
        case 2: {
            // Test cmsIT8FindDataFormat
            if (input_str) {
                int idx = cmsIT8FindDataFormat(hIT8, input_str);
                volatile int unused = idx;
                (void)unused;
            }
            break;
        }
        case 3: {
            // Test cmsIT8SetDataRowCol
            if (input_str && Size > 3) {
                int row = Data[1] % 10;
                int col = Data[2] % 4;
                cmsIT8SetDataRowCol(hIT8, row, col, input_str);
            }
            break;
        }
        case 4: {
            // Test cmsIT8GetDataRowCol
            if (Size > 2) {
                int row = Data[1] % 10;
                int col = Data[2] % 4;
                const char* result = cmsIT8GetDataRowCol(hIT8, row, col);
                if (result) {
                    volatile const char* unused = result;
                    (void)unused;
                }
            }
            break;
        }
        case 5: {
            // Test cmsIT8SetDataFormat
            if (input_str && Size > 2) {
                int col_idx = Data[1] % 4;
                cmsIT8SetDataFormat(hIT8, col_idx, input_str);
            }
            break;
        }
    }
    
    if (input_str) free(input_str);
    cmsIT8Free(hIT8);
    
    return 0;
}