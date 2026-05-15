// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cmsIT8SetData(cmsHANDLE hIT8, const char* cPatch, const char* cSample, const char* Val) {
    cmsIT8SetData(hIT8, cPatch, cSample, Val);
}

static void fuzz_cmsIT8SetPropertyUncooked(cmsHANDLE hIT8, const char* Key, const char* Buffer) {
    cmsIT8SetPropertyUncooked(hIT8, Key, Buffer);
}

static void fuzz_cmsstrcasecmp(const char* s1, const char* s2) {
    cmsstrcasecmp(s1, s2);
}

static void fuzz_cmsIT8SetPropertyMulti(cmsHANDLE hIT8, const char* Key, const char* SubKey, const char* Buffer) {
    cmsIT8SetPropertyMulti(hIT8, Key, SubKey, Buffer);
}

static void fuzz_cmsIT8GetPropertyMulti(cmsHANDLE hIT8, const char* Key, const char* SubKey) {
    cmsIT8GetPropertyMulti(hIT8, Key, SubKey);
}

static void fuzz_cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char* cSet, const char* cField, const char* ExpectedType) {
    cmsIT8SetTableByLabel(hIT8, cSet, cField, ExpectedType);
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create IT8 handle from file
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) return 0;
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "./dummy_file");
    if (!hIT8) {
        // If loading fails, create empty IT8 handle
        hIT8 = cmsIT8Alloc(NULL);
        if (!hIT8) return 0;
    }
    
    // Prepare strings from fuzz data
    size_t str_len = Size > 100 ? 100 : Size;
    char* str1 = (char*)malloc(str_len + 1);
    char* str2 = (char*)malloc(str_len + 1);
    char* str3 = (char*)malloc(str_len + 1);
    char* str4 = (char*)malloc(str_len + 1);
    
    if (!str1 || !str2 || !str3 || !str4) {
        free(str1); free(str2); free(str3); free(str4);
        cmsIT8Free(hIT8);
        return 0;
    }
    
    memcpy(str1, Data, str_len);
    memcpy(str2, Data, str_len);
    memcpy(str3, Data, str_len);
    memcpy(str4, Data, str_len);
    str1[str_len] = str2[str_len] = str3[str_len] = str4[str_len] = '\0';
    
    // Randomly select which functions to call based on input data
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_cmsIT8SetData(hIT8, str1, str2, str3);
            break;
        case 1:
            fuzz_cmsIT8SetPropertyUncooked(hIT8, str1, str2);
            break;
        case 2:
            fuzz_cmsstrcasecmp(str1, str2);
            break;
        case 3:
            fuzz_cmsIT8SetPropertyMulti(hIT8, str1, str2, str3);
            break;
        case 4:
            fuzz_cmsIT8GetPropertyMulti(hIT8, str1, str2);
            break;
        case 5:
            fuzz_cmsIT8SetTableByLabel(hIT8, str1, str2, str3);
            break;
    }
    
    // Clean up
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    cmsIT8Free(hIT8);
    
    return 0;
}