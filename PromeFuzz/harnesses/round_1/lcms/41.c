// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
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

static const char* extract_string(const uint8_t* data, size_t size, size_t* offset, char* buffer, size_t buffer_size) {
    if (*offset >= size || buffer_size == 0) {
        return NULL;
    }
    
    size_t max_len = size - *offset;
    if (max_len > buffer_size - 1) {
        max_len = buffer_size - 1;
    }
    
    if (max_len == 0) {
        return NULL;
    }
    
    memcpy(buffer, data + *offset, max_len);
    buffer[max_len] = '\0';
    *offset += max_len;
    
    return buffer;
}

static int extract_int(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    
    int value;
    memcpy(&value, data + *offset, sizeof(int));
    *offset += sizeof(int);
    
    return value;
}

int LLVMFuzzerTestOneInput_41(const uint8_t* Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }
    
    size_t offset = 0;
    
    char key_buffer[256];
    char subkey_buffer[256];
    char patch_buffer[256];
    char sample_buffer[256];
    char format_buffer[256];
    char name_buffer[256];
    
    const char* key = extract_string(Data, Size, &offset, key_buffer, sizeof(key_buffer));
    const char* subkey = extract_string(Data, Size, &offset, subkey_buffer, sizeof(subkey_buffer));
    const char* patch = extract_string(Data, Size, &offset, patch_buffer, sizeof(patch_buffer));
    const char* sample = extract_string(Data, Size, &offset, sample_buffer, sizeof(sample_buffer));
    const char* format = extract_string(Data, Size, &offset, format_buffer, sizeof(format_buffer));
    
    int nPatch = extract_int(Data, Size, &offset);
    
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "./dummy_file");
    if (!hIT8) {
        remove("./dummy_file");
        return 0;
    }
    
    const char* sheet_type = cmsIT8GetSheetType(hIT8);
    if (sheet_type) {
        volatile const char* unused = sheet_type;
        (void)unused;
    }
    
    const char* patch_name = cmsIT8GetPatchName(hIT8, nPatch, NULL);
    if (patch_name) {
        volatile const char* unused = patch_name;
        (void)unused;
    }
    
    patch_name = cmsIT8GetPatchName(hIT8, nPatch, name_buffer);
    if (patch_name) {
        volatile const char* unused = patch_name;
        (void)unused;
    }
    
    if (key) {
        const char* property_multi = cmsIT8GetPropertyMulti(hIT8, key, subkey);
        if (property_multi) {
            volatile const char* unused = property_multi;
            (void)unused;
        }
        
        const char* property = cmsIT8GetProperty(hIT8, key);
        if (property) {
            volatile const char* unused = property;
            (void)unused;
        }
    }
    
    if (patch && sample) {
        const char* data = cmsIT8GetData(hIT8, patch, sample);
        if (data) {
            volatile const char* unused = data;
            (void)unused;
        }
    }
    
    if (format) {
        cmsIT8DefineDblFormat(hIT8, format);
    } else {
        cmsIT8DefineDblFormat(hIT8, NULL);
    }
    
    cmsIT8Free(hIT8);
    remove("./dummy_file");
    
    return 0;
}