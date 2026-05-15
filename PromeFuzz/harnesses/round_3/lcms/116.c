// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
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

static void split_input(const uint8_t *Data, size_t Size, char *str1, char *str2, char *str3) {
    size_t part_size = Size / 3;
    if (part_size > MAX_STR_LEN - 1) part_size = MAX_STR_LEN - 1;
    
    if (part_size > 0) {
        memcpy(str1, Data, part_size);
        str1[part_size] = '\0';
        
        if (Size >= part_size * 2) {
            memcpy(str2, Data + part_size, part_size);
            str2[part_size] = '\0';
            
            size_t remaining = Size - (part_size * 2);
            if (remaining > MAX_STR_LEN - 1) remaining = MAX_STR_LEN - 1;
            if (remaining > 0) {
                memcpy(str3, Data + (part_size * 2), remaining);
                str3[remaining] = '\0';
            } else {
                str3[0] = '\0';
            }
        } else {
            str2[0] = '\0';
            str3[0] = '\0';
        }
    } else {
        str1[0] = '\0';
        str2[0] = '\0';
        str3[0] = '\0';
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    
    char str1[MAX_STR_LEN];
    char str2[MAX_STR_LEN];
    char str3[MAX_STR_LEN];
    
    split_input(Data, Size, str1, str2, str3);
    
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (!hIT8) return 0;
    
    // Test cmsIT8SetTableByLabel
    cmsIT8SetTableByLabel(hIT8, str1, str2, str3);
    
    // Test cmsIT8SetPropertyUncooked
    cmsIT8SetPropertyUncooked(hIT8, str1, str2);
    
    // Test cmsIT8SetData
    cmsIT8SetData(hIT8, str1, str2, str3);
    
    // Test cmsIT8GetPropertyMulti
    const char* prop = cmsIT8GetPropertyMulti(hIT8, str1, str2);
    (void)prop; // Use result to avoid unused variable warning
    
    // Test cmsIT8GetPatchName
    char buffer[MAX_STR_LEN];
    int patch_index = Data[0] % 100; // Use first byte for patch index
    const char* patch_name = cmsIT8GetPatchName(hIT8, patch_index, buffer);
    (void)patch_name; // Use result to avoid unused variable warning
    
    // Test cmsstrcasecmp
    int cmp_result = cmsstrcasecmp(str1, str2);
    (void)cmp_result; // Use result to avoid unused variable warning
    
    cmsIT8Free(hIT8);
    return 0;
}