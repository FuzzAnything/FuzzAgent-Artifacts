// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_cleanup(void) {
    // Clean up dummy file if it exists
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) {
        return 0;
    }

    // Use fuzzer input to create property strings
    // Ensure we have at least some data
    if (Size == 0) {
        cmsIT8Free(hIT8);
        return 0;
    }

    // Create null-terminated strings from fuzzer data
    size_t str_len = Size > 100 ? 100 : Size;
    char *prop_str = (char *)malloc(str_len + 1);
    if (!prop_str) {
        cmsIT8Free(hIT8);
        return 0;
    }
    memcpy(prop_str, Data, str_len);
    prop_str[str_len] = '\0';

    // Extract integer value from fuzzer data
    cmsUInt32Number int_val = 0;
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&int_val, Data, sizeof(cmsUInt32Number));
    }

    // Call target API functions in various combinations
    // 1. Set sheet type
    cmsIT8SetSheetType(hIT8, prop_str);

    // 2. Set table
    cmsInt32Number table_idx = cmsIT8SetTable(hIT8, int_val % 10); // Limit to reasonable table count

    // 3. Set property as hex
    cmsIT8SetPropertyHex(hIT8, prop_str, int_val);

    // 4. Get table count
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);

    // 5. Save to memory - first get required size
    cmsUInt32Number bytes_needed = 0;
    cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
    
    // Then actually save if we have space
    if (bytes_needed > 0 && bytes_needed < 1024 * 1024) { // Limit to 1MB
        void *buffer = malloc(bytes_needed);
        if (buffer) {
            cmsIT8SaveToMem(hIT8, buffer, &bytes_needed);
            free(buffer);
        }
    }

    // 6. Save to file
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fclose(dummy_file);
        cmsIT8SaveToFile(hIT8, "./dummy_file");
    }

    // Try setting another table with different index
    if (table_count > 0) {
        cmsIT8SetTable(hIT8, (int_val + 1) % table_count);
        
        // Set another property
        char *prop_str2 = (char *)malloc(str_len + 1);
        if (prop_str2) {
            // Use different part of input for second property
            size_t offset = str_len / 2;
            if (offset + str_len <= Size) {
                memcpy(prop_str2, Data + offset, str_len);
            } else {
                memcpy(prop_str2, Data, str_len);
            }
            prop_str2[str_len] = '\0';
            
            cmsIT8SetPropertyHex(hIT8, prop_str2, int_val + 1);
            free(prop_str2);
        }
    }

    // Cleanup
    free(prop_str);
    cmsIT8Free(hIT8);
    dummy_cleanup();
    
    return 0;
}