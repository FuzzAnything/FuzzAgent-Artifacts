// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write input to dummy file for potential IT8 loading
    write_dummy_file(Data, Size);
    
    // Create IT8 handle from file
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "./dummy_file");
    if (!hIT8) {
        // If loading fails, create an empty IT8 handle
        hIT8 = cmsIT8Alloc(NULL);
        if (!hIT8) return 0;
    }
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    
    // Extract strings from fuzzer input (safely)
    char str1[MAXSTR] = {0};
    char str2[MAXSTR] = {0};
    char str3[MAXSTR] = {0};
    
    size_t offset = 1;
    if (Size > offset) {
        size_t len1 = (Size - offset) < (MAXSTR-1) ? (Size - offset) : (MAXSTR-1);
        memcpy(str1, Data + offset, len1);
        str1[len1] = '\0';
        offset += len1;
    }
    
    if (Size > offset) {
        size_t len2 = (Size - offset) < (MAXSTR-1) ? (Size - offset) : (MAXSTR-1);
        memcpy(str2, Data + offset, len2);
        str2[len2] = '\0';
        offset += len2;
    }
    
    if (Size > offset) {
        size_t len3 = (Size - offset) < (MAXSTR-1) ? (Size - offset) : (MAXSTR-1);
        memcpy(str3, Data + offset, len3);
        str3[len3] = '\0';
    }
    
    // Test cmsIT8DefineDblFormat
    if (selector & 0x01) {
        cmsIT8DefineDblFormat(hIT8, str1[0] ? str1 : NULL);
    }
    
    // Test cmsIT8GetPatchName
    if (selector & 0x02) {
        int patch_index = (int)str1[0];  // Use first char as patch index
        char buffer[MAXSTR];
        const char* result = cmsIT8GetPatchName(hIT8, patch_index, buffer);
        if (!result) {
            result = cmsIT8GetPatchName(hIT8, patch_index, NULL);
        }
    }
    
    // Test cmsIT8GetPropertyMulti
    if (selector & 0x04) {
        const char* key = str1[0] ? str1 : "COLOR_REP";
        const char* subkey = str2[0] ? str2 : "RGB";
        const char* value = cmsIT8GetPropertyMulti(hIT8, key, subkey);
        (void)value;  // Suppress unused warning
    }
    
    // Test cmsIT8GetData
    if (selector & 0x08) {
        const char* patch = str1[0] ? str1 : "patch1";
        const char* sample = str2[0] ? str2 : "sample1";
        const char* data_val = cmsIT8GetData(hIT8, patch, sample);
        (void)data_val;  // Suppress unused warning
    }
    
    // Test cmsIT8GetSheetType
    if (selector & 0x10) {
        const char* sheet_type = cmsIT8GetSheetType(hIT8);
        (void)sheet_type;  // Suppress unused warning
    }
    
    // Test cmsIT8GetProperty
    if (selector & 0x20) {
        const char* prop = str1[0] ? str1 : "DESCRIPTOR";
        const char* prop_value = cmsIT8GetProperty(hIT8, prop);
        (void)prop_value;  // Suppress unused warning
    }
    
    // Cleanup
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    return 0;
}