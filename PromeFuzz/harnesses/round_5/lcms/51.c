// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
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
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    // Use first byte to determine operation variations
    uint8_t mode = Data[0] % 6;
    
    // Prepare string data from input
    char prop_name[256] = {0};
    char prop_value[256] = {0};
    char comment[256] = {0};
    char patch_name[256] = {0};
    
    size_t str_len = Size - 1;
    if (str_len > 255) str_len = 255;
    
    // Copy string data safely
    if (str_len > 0) {
        memcpy(prop_name, Data + 1, str_len);
        prop_name[str_len] = '\0';
        
        // Create variations of strings for different functions
        if (str_len > 10) {
            memcpy(prop_value, Data + 1 + (str_len/2), str_len/2);
            prop_value[str_len/2] = '\0';
            
            memcpy(comment, Data + 1, str_len/3);
            comment[str_len/3] = '\0';
            
            memcpy(patch_name, Data + 1 + (str_len/4), str_len/2);
            patch_name[str_len/2] = '\0';
        } else {
            strcpy(prop_value, "test_value");
            strcpy(comment, "test_comment");
            strcpy(patch_name, "test_patch");
        }
    }
    
    // Call target functions based on mode
    switch (mode) {
        case 0: {
            // Test cmsIT8GetPatchName
            char buffer[256];
            int patch_index = (int)(Data[1] % 100);
            const char* result = cmsIT8GetPatchName(hIT8, patch_index, buffer);
            if (result) {
                // Use result to avoid dead code elimination
                volatile char dummy = result[0];
                (void)dummy;
            }
            break;
        }
        
        case 1: {
            // Test cmsIT8SetPropertyStr
            cmsBool success = cmsIT8SetPropertyStr(hIT8, prop_name, prop_value);
            (void)success; // Use result
            break;
        }
        
        case 2: {
            // Test cmsIT8SetComment
            cmsBool success = cmsIT8SetComment(hIT8, comment);
            (void)success;
            break;
        }
        
        case 3: {
            // Test cmsIT8GetPatchByName
            int index = cmsIT8GetPatchByName(hIT8, patch_name);
            (void)index;
            break;
        }
        
        case 4: {
            // Test cmsIT8GetDataRowCol
            int row = (int)(Data[1] % 100);
            int col = (int)(Data[2] % 100);
            const char* result = cmsIT8GetDataRowCol(hIT8, row, col);
            if (result) {
                volatile char dummy = result[0];
                (void)dummy;
            }
            break;
        }
        
        case 5: {
            // Test cmsIT8SaveToFile
            // Write some data to dummy file first
            FILE* f = fopen("./dummy_file", "wb");
            if (f) {
                if (Size > 10) {
                    fwrite(Data, 1, Size > 1000 ? 1000 : Size, f);
                } else {
                    const char* dummy_data = "SAMPLE_DATA\n1 2 3\nEND\n";
                    fwrite(dummy_data, 1, strlen(dummy_data), f);
                }
                fclose(f);
            }
            
            cmsBool success = cmsIT8SaveToFile(hIT8, "./dummy_file");
            (void)success;
            dummy_cleanup();
            break;
        }
    }
    
    // Cleanup
    cmsIT8Free(hIT8);
    
    return 0;
}