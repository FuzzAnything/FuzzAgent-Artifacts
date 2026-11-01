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
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Minimum size check to ensure we have enough bytes for basic logic
    if (Size < 4) {
        return 0;
    }

    // Create safe, null-terminated strings from the input.
    // The input 'Data' is raw bytes and may not contain a null terminator.
    // We will create two local buffers on the stack, copy a portion of the input,
    // and ensure they are null-terminated.
    
    #define MAX_STR_LEN 1024
    
    char s1_buf[MAX_STR_LEN + 1];
    char s2_buf[MAX_STR_LEN + 1];
    
    // Calculate lengths safely
    size_t len1 = (Size > 0) ? (Data[0] % Size) : 0;
    if (len1 > MAX_STR_LEN) len1 = MAX_STR_LEN;
    
    size_t len2 = (Size > 0) ? (Data[1] % Size) : 0;
    if (len2 > MAX_STR_LEN) len2 = MAX_STR_LEN;
    
    // Copy data for s1
    memcpy(s1_buf, Data, len1);
    s1_buf[len1] = '\0';
    
    // Calculate offset for s2 safely
    // Ensure we don't access out of bounds
    size_t offset2 = 0;
    if (Size > 2) {
        offset2 = (Data[2] % (Size - 1));
    }
    
    // Calculate remaining bytes for s2
    size_t remaining = (Size > offset2) ? (Size - offset2) : 0;
    size_t actual_copy2 = len2;
    if (actual_copy2 > remaining) actual_copy2 = remaining;
    if (actual_copy2 > MAX_STR_LEN) actual_copy2 = MAX_STR_LEN;
    
    // Copy data for s2
    if (actual_copy2 > 0) {
        memcpy(s2_buf, Data + offset2, actual_copy2);
    }
    s2_buf[actual_copy2] = '\0';
    
    const char *s1 = s1_buf;
    const char *s2 = s2_buf;
    
    // Determine length for strnicmp safely
    int n_len = 0;
    if (Size > 3) {
        n_len = (Data[3] % 100) + 1;
    } else {
        n_len = 1;
    }

    // Invoke target functions in the specified order
    
    // 1. sqlite3_stricmp
    (void)sqlite3_stricmp(s1, s2);

    // 2. sqlite3_stricmp
    (void)sqlite3_stricmp(s1, NULL);

    // 3. sqlite3_stricmp
    (void)sqlite3_stricmp(NULL, s2);

    // 4. sqlite3_strnicmp
    (void)sqlite3_strnicmp(s1, s2, n_len);

    // 5. sqlite3_stricmp
    (void)sqlite3_stricmp(s1, s1);

    // 6. sqlite3_stricmp
    (void)sqlite3_stricmp(NULL, NULL);

    // 7. sqlite3_stricmp
    (void)sqlite3_stricmp(s2, s1);

    // 8. sqlite3_strglob
    (void)sqlite3_strglob(s1, s2);

    // 9. sqlite3_strglob
    (void)sqlite3_strglob(s2, s1);

    // 10. sqlite3_stricmp
    (void)sqlite3_stricmp(s1, s2);

    return 0;
}