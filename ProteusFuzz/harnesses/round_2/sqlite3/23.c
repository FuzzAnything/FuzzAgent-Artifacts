// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void safe_write_to_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create two strings from the input data
    const char *s1 = (const char *)Data;
    const char *s2 = (const char *)(Data + 1);

    // Ensure we have valid null-terminated strings by limiting length
    // We use the input data as raw bytes, but the functions expect C strings.
    // We will rely on the functions' internal handling or ensure safety.
    // Since the input is arbitrary, we must be careful not to read past Size.
    // However, these functions expect null-terminated strings. 
    // To make this safe for fuzzing, we will treat the input as potentially 
    // containing embedded nulls or not. The functions handle NULL pointers, 
    // but not necessarily non-null-terminated buffers if they read past bounds.
    // A common fuzzing strategy for string functions is to ensure a null terminator 
    // or use a fixed buffer. Here, we will just pass the pointers directly 
    // as the functions are robust, but we must ensure we don't access out of bounds 
    // if the functions assume null termination. 
    // Actually, sqlite3_stricmp and similar functions read until a null byte.
    // If the input doesn't have one, it might read beyond 'Size'.
    // To prevent this, we will create a copy with a guaranteed null terminator 
    // if one doesn't exist within the buffer, or simply limit the usage.
    // However, for simplicity and to test edge cases, we will pass the raw data 
    // but ensure the buffer is large enough or handle the case where no null exists.
    // A safer approach for fuzzing string functions that expect null-termination 
    // is to ensure the last byte is 0 or use a separate buffer.
    
    // Let's create two temporary buffers with guaranteed null termination 
    // to avoid reading past the input buffer.
    char buf1[256];
    char buf2[256];
    
    size_t len1 = (Size > 255) ? 255 : Size - 1;
    size_t len2 = (Size > 255) ? 255 : Size - 1;
    
    // Copy data ensuring null termination
    memcpy(buf1, Data, len1);
    buf1[len1] = '\0';
    
    // For the second string, use the rest of the data or a subset
    // If Size is small, we might not have enough for a second distinct string, 
    // but we can reuse or shift.
    if (Size > 1) {
        memcpy(buf2, Data + 1, len2);
        buf2[len2] = '\0';
    } else {
        buf2[0] = '\0';
    }

    const char *str1 = buf1;
    const char *str2 = buf2;

    // 1. sqlite3_stricmp
    (void)sqlite3_stricmp(str1, str2);

    // 2. sqlite3_stricmp
    (void)sqlite3_stricmp(str1, str1);

    // 3. sqlite3_stricmp
    (void)sqlite3_stricmp(str2, str1);

    // 4. sqlite3_strnicmp
    // Use a length based on the shorter of the two or a random portion of Size
    int nlen = (Size > 100) ? 100 : Size;
    if (nlen > 255) nlen = 255;
    (void)sqlite3_strnicmp(str1, str2, nlen);

    // 5. sqlite3_stricmp
    (void)sqlite3_stricmp(str1, NULL);

    // 6. sqlite3_stricmp
    (void)sqlite3_stricmp(NULL, str1);

    // 7. sqlite3_stricmp
    (void)sqlite3_stricmp(NULL, NULL);

    // 8. sqlite3_strglob
    // Use str1 as pattern, str2 as string
    (void)sqlite3_strglob(str1, str2);

    // 9. sqlite3_strglob
    // Use str2 as pattern, str1 as string
    (void)sqlite3_strglob(str2, str1);

    // 10. sqlite3_stricmp
    (void)sqlite3_stricmp(str1, str2);

    return 0;
}