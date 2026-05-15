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

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Handle empty input
    if (Size == 0) {
        return 0;
    }
    
    // Create two null-terminated strings from the input data
    // Split input into two parts for string comparison
    size_t split_point = Size / 2;
    if (split_point == 0) {
        split_point = 1;
    }
    
    // Allocate buffers for strings with null termination
    char *str1 = (char*)malloc(split_point + 1);
    char *str2 = (char*)malloc((Size - split_point) + 1);
    
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return 0;
    }
    
    // Copy data to strings and null-terminate
    memcpy(str1, Data, split_point);
    str1[split_point] = '\0';
    
    memcpy(str2, Data + split_point, Size - split_point);
    str2[Size - split_point] = '\0';
    
    // Call functions in the specified order
    // 1st sqlite3_stricmp
    sqlite3_stricmp(str1, str2);
    
    // 2nd sqlite3_stricmp
    sqlite3_stricmp(str2, str1);
    
    // 3rd sqlite3_stricmp
    sqlite3_stricmp(str1, str1);
    
    // sqlite3_strnicmp
    int n = (int)(Size % 256); // Use input size modulo 256 for length parameter
    if (n < 0) n = -n; // Ensure non-negative
    sqlite3_strnicmp(str1, str2, n);
    
    // 4th sqlite3_stricmp
    sqlite3_stricmp(str2, str2);
    
    // 5th sqlite3_stricmp (with NULL)
    sqlite3_stricmp(str1, NULL);
    
    // 6th sqlite3_stricmp (with NULL)
    sqlite3_stricmp(NULL, str2);
    
    // 1st sqlite3_strglob
    sqlite3_strglob(str1, str2);
    
    // 2nd sqlite3_strglob
    sqlite3_strglob(str2, str1);
    
    // 7th sqlite3_stricmp (both NULL)
    sqlite3_stricmp(NULL, NULL);
    
    // Cleanup
    free(str1);
    free(str2);
    
    return 0;
}