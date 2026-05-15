// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "lcms2.h"

static char* extract_string(const uint8_t** data, size_t* size, size_t max_len) {
    if (*size == 0) return NULL;
    
    size_t len = 0;
    while (len < *size && len < max_len && (*data)[len] != '\0') {
        len++;
    }
    
    char* str = (char*)malloc(len + 1);
    if (!str) return NULL;
    
    if (len > 0) {
        memcpy(str, *data, len);
    }
    str[len] = '\0';
    
    *data += len;
    *size -= len;
    
    if (*size > 0 && (*data)[0] == '\0') {
        (*data)++;
        (*size)--;
    }
    
    return str;
}

static void free_strings(int count, ...) {
    va_list args;
    va_start(args, count);
    
    for (int i = 0; i < count; i++) {
        char* str = va_arg(args, char*);
        if (str) free(str);
    }
    
    va_end(args);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;

    size_t remaining = Size;
    const uint8_t* ptr = Data;

    // Extract strings for cmsIT8SetDataFormat
    char* sample = extract_string(&ptr, &remaining, 100);
    int n = (remaining > 0) ? (int)ptr[0] % 100 : 0;
    if (remaining > 0) {
        ptr++;
        remaining--;
    }
    
    if (sample) {
        cmsIT8SetDataFormat(hIT8, n, sample);
    }

    // Extract strings for cmsIT8SetDataDbl
    char* cPatch = extract_string(&ptr, &remaining, 100);
    char* cSample = extract_string(&ptr, &remaining, 100);
    
    cmsFloat64Number val = 0.0;
    if (remaining >= sizeof(cmsFloat64Number)) {
        memcpy(&val, ptr, sizeof(cmsFloat64Number));
        ptr += sizeof(cmsFloat64Number);
        remaining -= sizeof(cmsFloat64Number);
    }
    
    if (cPatch && cSample) {
        cmsIT8SetDataDbl(hIT8, cPatch, cSample, val);
    }

    // Extract strings for cmsIT8SetPropertyMulti
    char* key = extract_string(&ptr, &remaining, 100);
    char* subkey = extract_string(&ptr, &remaining, 100);
    char* buffer = extract_string(&ptr, &remaining, 100);
    
    if (key && subkey && buffer) {
        cmsIT8SetPropertyMulti(hIT8, key, subkey, buffer);
    }

    // Extract strings for cmsIT8SetPropertyStr
    char* cProp = extract_string(&ptr, &remaining, 100);
    char* str = extract_string(&ptr, &remaining, 100);
    
    if (cProp && str) {
        cmsIT8SetPropertyStr(hIT8, cProp, str);
    }

    // Extract string for cmsIT8SetPropertyDbl
    char* cProp2 = extract_string(&ptr, &remaining, 100);
    
    cmsFloat64Number val2 = 0.0;
    if (remaining >= sizeof(cmsFloat64Number)) {
        memcpy(&val2, ptr, sizeof(cmsFloat64Number));
        ptr += sizeof(cmsFloat64Number);
        remaining -= sizeof(cmsFloat64Number);
    }
    
    if (cProp2) {
        cmsIT8SetPropertyDbl(hIT8, cProp2, val2);
    }

    // Save to file
    cmsIT8SaveToFile(hIT8, "./dummy_file");

    // Cleanup
    cmsIT8Free(hIT8);
    
    // Free all allocated strings
    free_strings(8, sample, cPatch, cSample, key, subkey, buffer, cProp, str);
    if (cProp2) free(cProp2);
    
    return 0;
}