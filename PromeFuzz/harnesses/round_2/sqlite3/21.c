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
    // Handle empty input
    if (Size == 0) {
        return 0;
    }
    
    // Create two null-terminated strings from the input data
    // Split input into two parts for string comparisons
    size_t half_size = Size / 2;
    char *str1 = (char *)malloc(half_size + 1);
    char *str2 = (char *)malloc(Size - half_size + 1);
    
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return 0;
    }
    
    // Copy data to strings and null-terminate
    memcpy(str1, Data, half_size);
    str1[half_size] = '\0';
    
    memcpy(str2, Data + half_size, Size - half_size);
    str2[Size - half_size] = '\0';
    
    // Call target functions in specified order
    sqlite3_stricmp(str1, str2);
    sqlite3_stricmp(str2, str1);
    sqlite3_stricmp(str1, str1);
    
    // For strnicmp, use a reasonable length (minimum of string lengths or fixed value)
    int cmp_len = (half_size < (Size - half_size)) ? half_size : (Size - half_size);
    if (cmp_len > 100) cmp_len = 100; // Limit to reasonable size
    sqlite3_strnicmp(str1, str2, cmp_len);
    
    sqlite3_stricmp(str1, NULL);
    sqlite3_stricmp(NULL, str2);
    sqlite3_stricmp(NULL, NULL);
    
    sqlite3_strglob(str1, str2);
    sqlite3_strglob(str2, str1);
    
    sqlite3_stricmp(str1, str2);
    
    // Cleanup
    free(str1);
    free(str2);
    
    return 0;
}