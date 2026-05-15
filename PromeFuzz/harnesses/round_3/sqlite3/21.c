// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:22358:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:119201:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:119201:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Split input into two null-terminated strings for comparison
    size_t half = Size / 2;
    char *str1 = NULL;
    char *str2 = NULL;
    
    // Allocate and copy first half
    if (half > 0) {
        str1 = (char *)malloc(half + 1);
        if (!str1) return 0;
        memcpy(str1, Data, half);
        str1[half] = '\0';
    }
    
    // Allocate and copy second half
    size_t second_half = Size - half;
    if (second_half > 0) {
        str2 = (char *)malloc(second_half + 1);
        if (!str2) {
            free(str1);
            return 0;
        }
        memcpy(str2, Data + half, second_half);
        str2[second_half] = '\0';
    }
    
    // Prepare integer parameter for sqlite3_strnicmp
    int n = 0;
    if (Size > 0) {
        n = Data[0] % 256; // Use first byte for comparison length
    }
    
    // Call functions in specified order
    sqlite3_stricmp(str1, str2);
    sqlite3_stricmp(str2, str1);
    sqlite3_stricmp(str1, str1);
    sqlite3_strnicmp(str1, str2, n);
    sqlite3_stricmp(str2, str2);
    sqlite3_stricmp(str1, str2);
    sqlite3_stricmp(str2, str1);
    sqlite3_strglob(str1, str2);
    sqlite3_strglob(str2, str1);
    sqlite3_stricmp(str1, str2);
    
    // Cleanup
    free(str1);
    free(str2);
    
    return 0;
}