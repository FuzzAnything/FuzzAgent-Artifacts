// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"

#define MAXSTR 256

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

static cmsHANDLE create_test_it8_handle(const uint8_t *data, size_t size) {
    // Try to load from file first
    write_dummy_file(data, size);
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "./dummy_file");
    remove_dummy_file();
    
    // If loading failed, create an empty handle
    if (!hIT8) {
        hIT8 = cmsIT8Alloc(NULL);
    }
    
    return hIT8;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create IT8 handle
    cmsHANDLE hIT8 = create_test_it8_handle(Data, Size);
    if (!hIT8) return 0;
    
    // Extract some strings from fuzz data for function parameters
    // Use safe string extraction to avoid overflows
    char fmt_buf[32] = {0};
    char key_buf[64] = {0};
    char subkey_buf[64] = {0};
    char patch_buf[64] = {0};
    char sample_buf[64] = {0};
    char prop_buf[64] = {0};
    
    size_t offset = 0;
    
    // Extract format string for cmsIT8DefineDblFormat
    if (offset < Size) {
        size_t fmt_len = Data[offset] % (sizeof(fmt_buf) - 1);
        if (fmt_len > 0 && offset + fmt_len < Size) {
            memcpy(fmt_buf, Data + offset + 1, fmt_len);
            fmt_buf[fmt_len] = '\0';
        }
        offset += 1 + fmt_len;
    }
    
    // Extract key for property functions
    if (offset < Size) {
        size_t key_len = Data[offset] % (sizeof(key_buf) - 1);
        if (key_len > 0 && offset + key_len < Size) {
            memcpy(key_buf, Data + offset + 1, key_len);
            key_buf[key_len] = '\0';
        }
        offset += 1 + key_len;
    }
    
    // Extract subkey for multi property
    if (offset < Size) {
        size_t subkey_len = Data[offset] % (sizeof(subkey_buf) - 1);
        if (subkey_len > 0 && offset + subkey_len < Size) {
            memcpy(subkey_buf, Data + offset + 1, subkey_len);
            subkey_buf[subkey_len] = '\0';
        }
        offset += 1 + subkey_len;
    }
    
    // Extract patch name
    if (offset < Size) {
        size_t patch_len = Data[offset] % (sizeof(patch_buf) - 1);
        if (patch_len > 0 && offset + patch_len < Size) {
            memcpy(patch_buf, Data + offset + 1, patch_len);
            patch_buf[patch_len] = '\0';
        }
        offset += 1 + patch_len;
    }
    
    // Extract sample name
    if (offset < Size) {
        size_t sample_len = Data[offset] % (sizeof(sample_buf) - 1);
        if (sample_len > 0 && offset + sample_len < Size) {
            memcpy(sample_buf, Data + offset + 1, sample_len);
            sample_buf[sample_len] = '\0';
        }
        offset += 1 + sample_len;
    }
    
    // Extract property name
    if (offset < Size) {
        size_t prop_len = Data[offset] % (sizeof(prop_buf) - 1);
        if (prop_len > 0 && offset + prop_len < Size) {
            memcpy(prop_buf, Data + offset + 1, prop_len);
            prop_buf[prop_len] = '\0';
        }
    }
    
    // Generate patch index from fuzz data
    int nPatch = 0;
    if (Size >= 2) {
        nPatch = Data[0] | (Data[1] << 8);
    }
    
    // Call target API functions
    
    // 1. cmsIT8DefineDblFormat
    cmsIT8DefineDblFormat(hIT8, fmt_buf[0] ? fmt_buf : NULL);
    
    // 2. cmsIT8GetPatchName
    char buffer[MAXSTR];
    const char* patch_result = cmsIT8GetPatchName(hIT8, nPatch, buffer);
    if (patch_result) {
        // Result is either in buffer or returned pointer
        volatile char dummy = patch_result[0];
        (void)dummy;
    }
    
    // Also test with NULL buffer
    patch_result = cmsIT8GetPatchName(hIT8, nPatch, NULL);
    if (patch_result) {
        volatile char dummy = patch_result[0];
        (void)dummy;
    }
    
    // 3. cmsIT8GetPropertyMulti
    const char* multi_result = cmsIT8GetPropertyMulti(hIT8, 
                                                      key_buf[0] ? key_buf : "COLOR_REP",
                                                      subkey_buf[0] ? subkey_buf : "RGB");
    if (multi_result) {
        volatile char dummy = multi_result[0];
        (void)dummy;
    }
    
    // 4. cmsIT8GetData
    const char* data_result = cmsIT8GetData(hIT8,
                                           patch_buf[0] ? patch_buf : "patch1",
                                           sample_buf[0] ? sample_buf : "sample1");
    if (data_result) {
        volatile char dummy = data_result[0];
        (void)dummy;
    }
    
    // 5. cmsIT8GetSheetType
    const char* sheet_type = cmsIT8GetSheetType(hIT8);
    if (sheet_type) {
        volatile char dummy = sheet_type[0];
        (void)dummy;
    }
    
    // 6. cmsIT8GetProperty
    const char* prop_result = cmsIT8GetProperty(hIT8, prop_buf[0] ? prop_buf : "DESCRIPTOR");
    if (prop_result) {
        volatile char dummy = prop_result[0];
        (void)dummy;
    }
    
    // Cleanup
    cmsIT8Free(hIT8);
    
    return 0;
}