// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"

#define MAX_STR_LEN 256
#define DUMMY_FILE "./dummy_file"

static void cleanup_dummy_file(void) {
    remove(DUMMY_FILE);
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen(DUMMY_FILE, "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 10) return 0;

    // Write data to dummy file for potential loading
    write_dummy_file(Data, Size);

    // Create an IT8 handle by loading from the dummy file
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, DUMMY_FILE);
    if (!hIT8) {
        // If loading fails, create an empty IT8 handle
        hIT8 = cmsIT8Alloc(NULL);
        if (!hIT8) {
            cleanup_dummy_file();
            return 0;
        }
    }

    // Extract some strings from the fuzzer input for function arguments
    // We'll use different offsets to get varied inputs
    size_t offset = 0;
    
    // For cmsIT8DefineDblFormat - use first 100 bytes or less as formatter string
    size_t fmt_len = Size > 100 ? 100 : Size;
    char *formatter = (char*)malloc(fmt_len + 1);
    if (formatter) {
        memcpy(formatter, Data, fmt_len);
        formatter[fmt_len] = '\0';
        cmsIT8DefineDblFormat(hIT8, formatter);
        // Also test with NULL
        cmsIT8DefineDblFormat(hIT8, NULL);
        free(formatter);
        offset += fmt_len;
    }

    // For cmsIT8GetProperty - use next chunk as property key
    if (offset < Size) {
        size_t prop_len = (Size - offset) > 50 ? 50 : (Size - offset);
        char *prop = (char*)malloc(prop_len + 1);
        if (prop) {
            memcpy(prop, Data + offset, prop_len);
            prop[prop_len] = '\0';
            const char *result = cmsIT8GetProperty(hIT8, prop);
            (void)result; // Use result to avoid unused variable warning
            free(prop);
            offset += prop_len;
        }
    }

    // For cmsIT8GetData - use two chunks as patch and sample names
    if (offset < Size) {
        size_t patch_len = (Size - offset) > 30 ? 30 : (Size - offset);
        char *patch = (char*)malloc(patch_len + 1);
        if (patch) {
            memcpy(patch, Data + offset, patch_len);
            patch[patch_len] = '\0';
            offset += patch_len;
            
            if (offset < Size) {
                size_t sample_len = (Size - offset) > 30 ? 30 : (Size - offset);
                char *sample = (char*)malloc(sample_len + 1);
                if (sample) {
                    memcpy(sample, Data + offset, sample_len);
                    sample[sample_len] = '\0';
                    const char *data_result = cmsIT8GetData(hIT8, patch, sample);
                    (void)data_result;
                    free(sample);
                    offset += sample_len;
                }
            }
            free(patch);
        }
    }

    // For cmsIT8GetSheetType
    const char *sheet_type = cmsIT8GetSheetType(hIT8);
    (void)sheet_type;

    // For cmsIT8GetPatchName - test with various indices and buffers
    for (int i = -1; i <= 5; i++) {
        // First with NULL buffer
        const char *name1 = cmsIT8GetPatchName(hIT8, i, NULL);
        (void)name1;
        
        // Then with allocated buffer
        char buffer[MAX_STR_LEN];
        const char *name2 = cmsIT8GetPatchName(hIT8, i, buffer);
        (void)name2;
    }

    // For cmsIT8GetPropertyMulti - use two chunks as key and subkey
    if (offset < Size) {
        size_t key_len = (Size - offset) > 30 ? 30 : (Size - offset);
        char *key = (char*)malloc(key_len + 1);
        if (key) {
            memcpy(key, Data + offset, key_len);
            key[key_len] = '\0';
            offset += key_len;
            
            if (offset < Size) {
                size_t subkey_len = (Size - offset) > 30 ? 30 : (Size - offset);
                char *subkey = (char*)malloc(subkey_len + 1);
                if (subkey) {
                    memcpy(subkey, Data + offset, subkey_len);
                    subkey[subkey_len] = '\0';
                    const char *multi_result = cmsIT8GetPropertyMulti(hIT8, key, subkey);
                    (void)multi_result;
                    free(subkey);
                }
            }
            free(key);
        }
    }

    // Cleanup
    cmsIT8Free(hIT8);
    cleanup_dummy_file();
    
    return 0;
}