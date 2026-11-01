// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int safe_stricmp(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 1 >= size) return 0;
    int len1 = data[*offset];
    (*offset)++;
    if (len1 < 0 || *offset + (size_t)len1 > size) return 0;
    const char *s1 = (const char *)&data[*offset];
    (*offset) += (size_t)len1;
    
    if (*offset + 1 >= size) return 0;
    int len2 = data[*offset];
    (*offset)++;
    if (len2 < 0 || *offset + (size_t)len2 > size) return 0;
    const char *s2 = (const char *)&data[*offset];
    (*offset) += (size_t)len2;
    
    // Ensure strings are null-terminated for sqlite3_stricmp safety if needed,
    // though sqlite3_stricmp handles non-null-terminated by design in some contexts,
    // the internal implementation expects valid memory. 
    // To be safe against buffer overflows in the library's internal logic if it assumes NUL termination:
    // We rely on the library's robustness, but ensure we don't pass pointers outside the buffer.
    // The library implementation checks for NUL. If our len is exact and no NUL exists, 
    // it might read past if it relies on NUL. 
    // However, sqlite3_stricmp is designed to work with strings. 
    // To prevent overflow, we must ensure the data acts as a valid C-string or the function handles it.
    // Since we are fuzzing, we might not have NUL. 
    // A safer approach for fuzzing without modifying the input buffer:
    // Just pass the pointers. The error indicates the library read past the buffer.
    // This implies the library expects NUL termination. 
    // We should ensure the strings are NUL terminated or the length is respected.
    // sqlite3_stricmp does NOT take a length. It scans until NUL.
    // Therefore, we MUST ensure s1 and s2 are NUL terminated within the bounds of 'data'.
    
    // Re-evaluating: We need to write a NUL byte at the end of the string in the buffer.
    // But we cannot modify the input buffer 'data' directly if it's const or if we want to be safe.
    // Actually, we can copy to a local buffer or ensure the input has space.
    // Better: Create local buffers with NUL termination.
    
    char buf1[4096];
    char buf2[4096];
    
    size_t copy1 = (size_t)len1 < sizeof(buf1) - 1 ? (size_t)len1 : sizeof(buf1) - 1;
    size_t copy2 = (size_t)len2 < sizeof(buf2) - 1 ? (size_t)len2 : sizeof(buf2) - 1;
    
    memcpy(buf1, s1, copy1);
    buf1[copy1] = '\0';
    
    memcpy(buf2, s2, copy2);
    buf2[copy2] = '\0';
    
    return sqlite3_stricmp(buf1, buf2);
}

static int safe_strnicmp(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 1 >= size) return 0;
    int len1 = data[*offset];
    (*offset)++;
    if (len1 < 0 || *offset + (size_t)len1 > size) return 0;
    const char *s1 = (const char *)&data[*offset];
    (*offset) += (size_t)len1;
    
    if (*offset + 1 >= size) return 0;
    int len2 = data[*offset];
    (*offset)++;
    if (len2 < 0 || *offset + (size_t)len2 > size) return 0;
    const char *s2 = (const char *)&data[*offset];
    (*offset) += (size_t)len2;
    
    if (*offset + 1 >= size) return 0;
    int n = data[*offset];
    (*offset)++;
    
    // sqlite3_strnicmp compares up to n chars. It does NOT require NUL termination within n.
    // But we must ensure s1 and s2 are valid memory for at least n bytes (or until NUL).
    // The input s1 and s2 are from the buffer, so they are valid for len1 and len2.
    // If n > len1 or n > len2, we might read past the string if the function relies on NUL?
    // No, strnicmp stops at n. But if the string is shorter than n and not NUL terminated in the buffer?
    // The buffer continues with other data. That's fine for strnicmp as long as we don't go past 'size'.
    // However, to be safe, we limit n to the available length of the strings in the buffer.
    
    int safe_n = n;
    if (safe_n > len1) safe_n = len1;
    if (safe_n > len2) safe_n = len2;
    if (safe_n < 0) safe_n = 0;
    
    return sqlite3_strnicmp(s1, s2, safe_n);
}

static int safe_strglob(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 1 >= size) return 0;
    int len1 = data[*offset];
    (*offset)++;
    if (len1 < 0 || *offset + (size_t)len1 > size) return 0;
    const char *pattern = (const char *)&data[*offset];
    (*offset) += (size_t)len1;
    
    if (*offset + 1 >= size) return 0;
    int len2 = data[*offset];
    (*offset)++;
    if (len2 < 0 || *offset + (size_t)len2 > size) return 0;
    const char *str = (const char *)&data[*offset];
    (*offset) += (size_t)len2;
    
    // sqlite3_strglob expects NUL terminated strings.
    char buf1[4096];
    char buf2[4096];
    
    size_t copy1 = (size_t)len1 < sizeof(buf1) - 1 ? (size_t)len1 : sizeof(buf1) - 1;
    size_t copy2 = (size_t)len2 < sizeof(buf2) - 1 ? (size_t)len2 : sizeof(buf2) - 1;
    
    memcpy(buf1, pattern, copy1);
    buf1[copy1] = '\0';
    
    memcpy(buf2, str, copy2);
    buf2[copy2] = '\0';
    
    return sqlite3_strglob(buf1, buf2);
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    size_t offset = 0;
    
    // 1. sqlite3_stricmp
    safe_stricmp(Data, Size, &offset);
    
    // 2. sqlite3_stricmp
    safe_stricmp(Data, Size, &offset);
    
    // 3. sqlite3_stricmp
    safe_stricmp(Data, Size, &offset);
    
    // 4. sqlite3_strnicmp
    safe_strnicmp(Data, Size, &offset);
    
    // 5. sqlite3_stricmp
    safe_stricmp(Data, Size, &offset);
    
    // 6. sqlite3_stricmp
    safe_stricmp(Data, Size, &offset);
    
    // 7. sqlite3_stricmp
    safe_stricmp(Data, Size, &offset);
    
    // 8. sqlite3_strglob
    safe_strglob(Data, Size, &offset);
    
    // 9. sqlite3_strglob
    safe_strglob(Data, Size, &offset);
    
    // 10. sqlite3_stricmp
    safe_stricmp(Data, Size, &offset);
    
    return 0;
}