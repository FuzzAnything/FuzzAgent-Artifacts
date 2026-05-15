// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
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

static char* create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) {
        char* str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    size_t len = size < max_len ? size : max_len;
    char* str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

static void free_strings(char** strings, size_t count) {
    for (size_t i = 0; i < count; i++) {
        free(strings[i]);
    }
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    size_t max_strings = 10;
    char* strings[10];
    size_t string_count = 0;
    
    const uint8_t* current = Data;
    size_t remaining = Size;
    
    while (remaining > 0 && string_count < max_strings) {
        size_t chunk_size = remaining > 32 ? 32 : remaining;
        strings[string_count] = create_null_terminated_string(current, chunk_size, 255);
        if (!strings[string_count]) break;
        
        string_count++;
        current += chunk_size;
        remaining = (remaining > chunk_size) ? remaining - chunk_size : 0;
    }
    
    if (string_count >= 4) {
        cmsIT8SetTableByLabel(hIT8, 
                             strings[0] ? strings[0] : "SET",
                             strings[1] ? strings[1] : "FIELD", 
                             strings[2] ? strings[2] : "TYPE");
    }
    
    if (string_count >= 2) {
        cmsIT8SetPropertyStr(hIT8, 
                            strings[0] ? strings[0] : "PROP",
                            strings[1] ? strings[1] : "VALUE");
    }
    
    if (string_count >= 3) {
        cmsIT8SetPropertyMulti(hIT8,
                              strings[0] ? strings[0] : "KEY",
                              strings[1] ? strings[1] : "SUBKEY",
                              strings[2] ? strings[2] : "BUFFER");
    }
    
    if (string_count >= 1) {
        cmsIT8GetPatchByName(hIT8, strings[0] ? strings[0] : "PATCH");
    }
    
    if (string_count >= 1) {
        cmsIT8FindDataFormat(hIT8, strings[0] ? strings[0] : "SAMPLE");
    }
    
    if (Size >= 8) {
        int row = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        int col = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
        const char* result = cmsIT8GetDataRowCol(hIT8, row, col);
        (void)result;
    }
    
    free_strings(strings, string_count);
    cmsIT8Free(hIT8);
    
    return 0;
}