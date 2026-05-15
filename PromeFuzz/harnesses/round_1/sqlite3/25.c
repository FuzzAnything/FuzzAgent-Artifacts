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
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create null-terminated strings from input data
    char *str1 = (char *)malloc(Size + 1);
    char *str2 = (char *)malloc(Size + 1);
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return 0;
    }
    
    memcpy(str1, Data, Size);
    memcpy(str2, Data, Size);
    str1[Size] = '\0';
    str2[Size] = '\0';

    // Modify str2 slightly to create some variation
    if (Size > 10) {
        str2[Size/2] = (str2[Size/2] + 1) % 128;
    }

    // Call functions in specified order
    sqlite3_stricmp(str1, str2);
    sqlite3_stricmp(str2, str1);
    sqlite3_stricmp(str1, str1);
    
    sqlite3_strnicmp(str1, str2, (int)(Size % 100));
    
    sqlite3_stricmp(str2, str2);
    sqlite3_stricmp(NULL, str1);
    sqlite3_stricmp(str1, NULL);
    
    sqlite3_strglob("test*", str1);
    sqlite3_strglob(str1, "pattern*");
    
    sqlite3_stricmp(NULL, NULL);

    free(str1);
    free(str2);
    return 0;
}