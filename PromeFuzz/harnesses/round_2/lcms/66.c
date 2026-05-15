// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
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

static char* read_string(const uint8_t** data, size_t* size) {
    if (*size == 0) return NULL;
    
    size_t len = (*data)[0];
    if (len == 0 || len > *size - 1) {
        len = *size - 1;
    }
    
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, *data + 1, len);
    str[len] = '\0';
    
    *data += len + 1;
    *size -= len + 1;
    
    return str;
}

static int read_int(const uint8_t** data, size_t* size) {
    if (*size < 4) {
        int val = (*size > 0) ? (int)(**data) : 0;
        if (*size > 0) {
            (*data)++;
            (*size)--;
        }
        return val;
    }
    
    int val;
    memcpy(&val, *data, 4);
    *data += 4;
    *size -= 4;
    return val;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Write data to dummy file for IT8 loading
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Load IT8 handle from file
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "./dummy_file");
    if (!hIT8) {
        remove("./dummy_file");
        return 0;
    }
    
    // Parse parameters from fuzzer input
    const uint8_t* cursor = Data;
    size_t remaining = Size;
    
    // Read parameters for various functions
    int nPatch = read_int(&cursor, &remaining);
    char* cSet = read_string(&cursor, &remaining);
    char* cField = read_string(&cursor, &remaining);
    char* ExpectedType = read_string(&cursor, &remaining);
    char* Key = read_string(&cursor, &remaining);
    char* SubKey = read_string(&cursor, &remaining);
    char* cSample = read_string(&cursor, &remaining);
    char* Buffer = read_string(&cursor, &remaining);
    char* s1 = read_string(&cursor, &remaining);
    char* s2 = read_string(&cursor, &remaining);
    
    // Test cmsIT8GetPatchName
    char patchBuffer[256];
    const char* patchName = cmsIT8GetPatchName(hIT8, nPatch, NULL);
    if (patchName) {
        cmsIT8GetPatchName(hIT8, nPatch, patchBuffer);
    }
    
    // Test cmsIT8SetTableByLabel
    if (cSet && cField) {
        cmsIT8SetTableByLabel(hIT8, cSet, cField, ExpectedType);
    }
    
    // Test cmsstrcasecmp
    if (s1 && s2) {
        cmsstrcasecmp(s1, s2);
    }
    
    // Test cmsIT8GetPropertyMulti
    if (Key && SubKey) {
        const char* prop = cmsIT8GetPropertyMulti(hIT8, Key, SubKey);
        (void)prop; // Avoid unused variable warning
    }
    
    // Test cmsIT8FindDataFormat
    if (cSample) {
        cmsIT8FindDataFormat(hIT8, cSample);
    }
    
    // Test cmsIT8SetPropertyUncooked
    if (Key && Buffer) {
        cmsIT8SetPropertyUncooked(hIT8, Key, Buffer);
    }
    
    // Cleanup
    free(cSet);
    free(cField);
    free(ExpectedType);
    free(Key);
    free(SubKey);
    free(cSample);
    free(Buffer);
    free(s1);
    free(s2);
    
    cmsIT8Free(hIT8);
    remove("./dummy_file");
    
    return 0;
}