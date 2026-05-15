// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAXSTR 256
#define DUMMY_FILE "./dummy_file"

static char* extract_string(const uint8_t** data, size_t* size) {
    if (*size == 0) return NULL;
    
    size_t len = (*data)[0] % (MAXSTR - 1);
    if (len == 0) len = 1;
    if (len > *size - 1) len = *size - 1;
    
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, *data + 1, len);
    str[len] = '\0';
    
    *data += len + 1;
    *size -= len + 1;
    
    return str;
}

static int extract_int(const uint8_t** data, size_t* size) {
    if (*size < sizeof(int)) return 0;
    
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    
    return value;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    const uint8_t* cursor = Data;
    size_t remaining = Size;
    
    // Create IT8 handle from a file
    FILE* f = fopen(DUMMY_FILE, "wb");
    if (!f) return 0;
    
    // Write minimal IT8 data to file
    const char* it8_header = "CGATS.17\nBEGIN_DATA_FORMAT\nSAMPLE_ID\nEND_DATA_FORMAT\nBEGIN_DATA\n1\nEND_DATA\n";
    fwrite(it8_header, 1, strlen(it8_header), f);
    fclose(f);
    
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, DUMMY_FILE);
    if (!hIT8) {
        remove(DUMMY_FILE);
        return 0;
    }
    
    // Extract parameters for cmsIT8SetIndexColumn
    char* sample1 = extract_string(&cursor, &remaining);
    if (sample1) {
        cmsIT8SetIndexColumn(hIT8, sample1);
        free(sample1);
    }
    
    // Extract parameters for cmsIT8SetData
    char* patch1 = extract_string(&cursor, &remaining);
    char* sample2 = extract_string(&cursor, &remaining);
    char* value = extract_string(&cursor, &remaining);
    
    if (patch1 && sample2 && value) {
        cmsIT8SetData(hIT8, patch1, sample2, value);
    }
    
    if (patch1) free(patch1);
    if (sample2) free(sample2);
    if (value) free(value);
    
    // Extract parameters for cmsIT8GetPatchName
    int nPatch = extract_int(&cursor, &remaining);
    char buffer[MAXSTR];
    const char* result = cmsIT8GetPatchName(hIT8, nPatch, buffer);
    if (result) {
        // Also test with NULL buffer
        cmsIT8GetPatchName(hIT8, nPatch, NULL);
    }
    
    // Extract parameters for cmsIT8SetDataFormat
    int n = extract_int(&cursor, &remaining);
    char* sample3 = extract_string(&cursor, &remaining);
    if (sample3) {
        cmsIT8SetDataFormat(hIT8, n, sample3);
        free(sample3);
    }
    
    // Extract parameters for cmsIT8SetTableByLabel
    char* set = extract_string(&cursor, &remaining);
    char* field = extract_string(&cursor, &remaining);
    char* expectedType = extract_string(&cursor, &remaining);
    
    if (set && field) {
        cmsIT8SetTableByLabel(hIT8, set, field, expectedType);
    }
    
    if (set) free(set);
    if (field) free(field);
    if (expectedType) free(expectedType);
    
    // Extract parameters for cmsIT8GetPatchByName
    char* patch2 = extract_string(&cursor, &remaining);
    if (patch2) {
        cmsIT8GetPatchByName(hIT8, patch2);
        free(patch2);
    }
    
    // Cleanup
    cmsIT8Free(hIT8);
    remove(DUMMY_FILE);
    
    return 0;
}