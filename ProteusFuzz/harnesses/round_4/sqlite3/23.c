// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static char *create_safe_string(const uint8_t *data, size_t size, size_t *offset, size_t *len) {
    if (*offset >= size) {
        *len = 0;
        return NULL;
    }
    
    size_t start = *offset;
    size_t remaining = size - start;
    size_t str_len = 0;
    
    // Find the length of the string (up to 4096 or until null terminator)
    while (str_len < remaining && str_len < 4096) {
        if (data[start + str_len] == 0) {
            break;
        }
        str_len++;
    }
    
    *offset = start + str_len + 1; // Move past the string and the null terminator
    
    if (str_len == 0) {
        *len = 0;
        return NULL;
    }
    
    // Allocate a new buffer and copy the string, ensuring it is null-terminated
    char *safe_str = (char *)malloc(str_len + 1);
    if (!safe_str) {
        *len = 0;
        return NULL;
    }
    
    memcpy(safe_str, data + start, str_len);
    safe_str[str_len] = '\0';
    *len = str_len;
    
    return safe_str;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    size_t offset = 0;
    size_t len1, len2, len3, len4, len5, len6, len7, len8, len9, len10;
    
    char *s1 = create_safe_string(Data, Size, &offset, &len1);
    char *s2 = create_safe_string(Data, Size, &offset, &len2);
    char *s3 = create_safe_string(Data, Size, &offset, &len3);
    char *s4 = create_safe_string(Data, Size, &offset, &len4);
    char *s5 = create_safe_string(Data, Size, &offset, &len5);
    char *s6 = create_safe_string(Data, Size, &offset, &len6);
    char *s7 = create_safe_string(Data, Size, &offset, &len7);
    char *s8 = create_safe_string(Data, Size, &offset, &len8);
    char *s9 = create_safe_string(Data, Size, &offset, &len9);
    char *s10 = create_safe_string(Data, Size, &offset, &len10);

    // Invoke sqlite3_stricmp -> sqlite3_stricmp -> sqlite3_stricmp
    (void)sqlite3_stricmp(s1, s2);
    (void)sqlite3_stricmp(s3, s4);
    (void)sqlite3_stricmp(s5, s6);

    // Invoke sqlite3_strnicmp
    int n = (Size > 0) ? (int)(Data[0] % 100) : 0;
    if (n < 0) n = -n;
    (void)sqlite3_strnicmp(s7, s8, n);

    // Invoke sqlite3_stricmp -> sqlite3_stricmp -> sqlite3_stricmp
    (void)sqlite3_stricmp(s9, s10);
    (void)sqlite3_stricmp(s1, s3);
    (void)sqlite3_stricmp(s2, s4);

    // Invoke sqlite3_strglob -> sqlite3_strglob
    (void)sqlite3_strglob(s5, s6);
    (void)sqlite3_strglob(s7, s8);

    // Invoke sqlite3_stricmp
    (void)sqlite3_stricmp(s9, s10);

    // Cleanup allocated memory
    free(s1);
    free(s2);
    free(s3);
    free(s4);
    free(s5);
    free(s6);
    free(s7);
    free(s8);
    free(s9);
    free(s10);

    return 0;
}