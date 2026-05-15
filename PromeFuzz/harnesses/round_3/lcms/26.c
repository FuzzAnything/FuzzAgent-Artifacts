// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

#define MAXSTR 256

static void extract_strings(const uint8_t *data, size_t size, char *str1, char *str2, int max_len) {
    size_t pos = 0;
    int str_idx = 0;
    char *strs[2] = {str1, str2};
    
    for (int i = 0; i < 2 && pos < size; i++) {
        size_t len = 0;
        while (pos + len < size && len < (size_t)max_len - 1 && data[pos + len] != 0) {
            len++;
        }
        if (len > 0) {
            memcpy(strs[i], data + pos, len);
            strs[i][len] = '\0';
        } else {
            strs[i][0] = '\0';
        }
        pos += len + 1;
    }
}

static int extract_int(const uint8_t *data, size_t size, size_t *pos) {
    if (*pos + sizeof(int) <= size) {
        int value;
        memcpy(&value, data + *pos, sizeof(int));
        *pos += sizeof(int);
        return value;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    // Create a dummy IT8 handle by loading from memory
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(NULL, Data, Size);
    if (hIT8 == NULL) {
        // If loading fails, create an empty IT8 handle
        hIT8 = cmsIT8Alloc(NULL);
        if (hIT8 == NULL) return 0;
        
        // We cannot use setter functions as they're not in the provided API
        // We'll just use the empty handle for testing getter functions
    }
    
    // Test cmsIT8DefineDblFormat
    char formatter[100];
    size_t pos = 0;
    size_t len = Size > 100 ? 100 : Size;
    if (len > 0) {
        memcpy(formatter, Data, len);
        formatter[len - 1] = '\0';
        cmsIT8DefineDblFormat(hIT8, formatter);
    }
    
    // Test cmsIT8GetData
    char patch[100], sample[100];
    extract_strings(Data + (pos % Size), Size - (pos % Size), patch, sample, 100);
    const char *data_result = cmsIT8GetData(hIT8, patch, sample);
    (void)data_result;
    
    // Test cmsIT8GetSheetType
    const char *sheet_type = cmsIT8GetSheetType(hIT8);
    (void)sheet_type;
    
    // Test cmsIT8GetPropertyMulti
    char key[100], subkey[100];
    extract_strings(Data + ((pos + 10) % Size), Size - ((pos + 10) % Size), key, subkey, 100);
    const char *prop_multi = cmsIT8GetPropertyMulti(hIT8, key, subkey);
    (void)prop_multi;
    
    // Test cmsIT8GetPatchName
    int patch_index = extract_int(Data, Size, &pos);
    char buffer[MAXSTR];
    const char *patch_name = cmsIT8GetPatchName(hIT8, patch_index, buffer);
    (void)patch_name;
    
    // Test cmsIT8GetProperty
    char prop[100];
    size_t prop_pos = (pos + 20) % Size;
    size_t prop_len = Size - prop_pos > 100 ? 100 : Size - prop_pos;
    if (prop_len > 0) {
        memcpy(prop, Data + prop_pos, prop_len);
        prop[prop_len - 1] = '\0';
        const char *property = cmsIT8GetProperty(hIT8, prop);
        (void)property;
    }
    
    // Cleanup
    cmsIT8Free(hIT8);
    
    return 0;
}