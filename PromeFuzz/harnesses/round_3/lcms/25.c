// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

#define MAX_STR_LEN 256

static char* extract_string(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) return NULL;
    
    size_t max_len = size - *offset;
    size_t str_len = 0;
    
    while (str_len < max_len && data[*offset + str_len] != 0) {
        str_len++;
    }
    
    char* str = (char*)malloc(str_len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + *offset, str_len);
    str[str_len] = '\0';
    
    *offset += str_len + 1;
    return str;
}

static int extract_int(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(int) > size) return 0;
    
    int value;
    memcpy(&value, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return value;
}

int LLVMFuzzerTestOneInput_25(const uint8_t* Data, size_t Size) {
    if (Size < 10) return 0;
    
    size_t offset = 0;
    
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    char* str1 = extract_string(Data, Size, &offset);
    char* str2 = extract_string(Data, Size, &offset);
    char* str3 = extract_string(Data, Size, &offset);
    
    if (str1 && str2 && str3) {
        cmsIT8SetTableByLabel(hIT8, str1, str2, str3);
    }
    
    free(str1);
    free(str2);
    free(str3);
    
    str1 = extract_string(Data, Size, &offset);
    if (str1) {
        cmsIT8SetIndexColumn(hIT8, str1);
        free(str1);
    }
    
    str1 = extract_string(Data, Size, &offset);
    if (str1) {
        cmsIT8FindDataFormat(hIT8, str1);
        free(str1);
    }
    
    int patch_index = extract_int(Data, Size, &offset);
    char buffer[MAX_STR_LEN];
    cmsIT8GetPatchName(hIT8, patch_index, buffer);
    
    int col_index = extract_int(Data, Size, &offset);
    str1 = extract_string(Data, Size, &offset);
    if (str1) {
        cmsIT8SetDataFormat(hIT8, col_index, str1);
        free(str1);
    }
    
    str1 = extract_string(Data, Size, &offset);
    if (str1) {
        cmsIT8SetComment(hIT8, str1);
        free(str1);
    }
    
    cmsIT8Free(hIT8);
    return 0;
}