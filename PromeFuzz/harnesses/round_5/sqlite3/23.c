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
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static void copy_to_buffer(char *dest, size_t dest_size, const uint8_t *src, size_t src_size, size_t *offset) {
    size_t i;
    for (i = 0; i < dest_size - 1 && *offset < src_size; i++, (*offset)++) {
        dest[i] = src[*offset];
    }
    dest[i] = '\0';
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    char s1[256], s2[256], s3[256], s4[256];
    size_t offset = 0;
    
    copy_to_buffer(s1, sizeof(s1), Data, Size, &offset);
    copy_to_buffer(s2, sizeof(s2), Data, Size, &offset);
    copy_to_buffer(s3, sizeof(s3), Data, Size, &offset);
    copy_to_buffer(s4, sizeof(s4), Data, Size, &offset);
    
    int n = (Data[0] % 256) + 1;
    
    sqlite3_stricmp(s1, s2);
    sqlite3_stricmp(s2, s3);
    sqlite3_stricmp(s3, s4);
    sqlite3_strnicmp(s1, s2, n);
    sqlite3_stricmp(s4, s1);
    sqlite3_stricmp(s2, s4);
    sqlite3_stricmp(s3, s1);
    sqlite3_strglob(s1, s2);
    sqlite3_strglob(s3, s4);
    sqlite3_stricmp(s1, s3);
    
    return 0;
}