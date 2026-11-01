// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void safe_fuzz_call_keyword_check(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Test with various lengths
    for (int len = 1; len <= (int)size && len <= 100; len++) {
        sqlite3_keyword_check((const char *)data, len);
    }
    
    // Test with empty string
    sqlite3_keyword_check("", 0);
}

static void safe_fuzz_call_keyword_name(const uint8_t *data, size_t size) {
    const char *pzName = NULL;
    int pnName = 0;
    
    // Try indices based on input data
    // SQLITE_N_KEYWORD is typically 100, we use a safe upper bound
    int max_keywords = 100;
    
    for (int i = 0; i < 100; i++) {
        int idx = 0;
        if (size > 0) {
            idx = (data[i % size] % max_keywords) + (size > 1 ? (data[(i+1) % size] % max_keywords) : 0);
        }
        
        // Test valid range
        if (idx >= 0 && idx < max_keywords) {
            sqlite3_keyword_name(idx, &pzName, &pnName);
        }
        
        // Test out of bounds
        sqlite3_keyword_name(-1, &pzName, &pnName);
        sqlite3_keyword_name(max_keywords, &pzName, &pnName);
        sqlite3_keyword_name(1000, &pzName, &pnName);
    }
}

static void safe_fuzz_call_strnicmp(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create two substrings from input
    // Ensure we don't go out of bounds when accessing data + i or data + j
    size_t max_offset = size;
    
    // Test various combinations with strict bounds checking
    for (size_t i = 0; i < max_offset; i++) {
        for (size_t j = 0; j < max_offset; j++) {
            // Calculate maximum safe length for comparison
            size_t remaining_from_i = max_offset - i;
            size_t remaining_from_j = max_offset - j;
            size_t max_safe_len = (remaining_from_i < remaining_from_j) ? remaining_from_i : remaining_from_j;
            
            // Limit comparison length to avoid buffer overflow
            int cmp_len = 0;
            if (max_safe_len > 0) {
                cmp_len = (int)(max_safe_len % 100);
                if (cmp_len == 0) cmp_len = 1;
            }
            
            sqlite3_strnicmp((const char *)(data + i), (const char *)(data + j), cmp_len);
        }
    }
    
    // Test with NULL pointers
    sqlite3_strnicmp(NULL, NULL, 10);
    sqlite3_strnicmp(NULL, (const char *)data, 10);
    sqlite3_strnicmp((const char *)data, NULL, 10);
    
    // Test with zero length
    sqlite3_strnicmp((const char *)data, (const char *)data, 0);
}

static void safe_fuzz_call_stricmp(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Test various substrings - stricmp doesn't take length, so we just pass pointers
    // The function will read until null terminator, so we need to be careful
    // We'll create temporary null-terminated strings
    char buf1[256];
    char buf2[256];
    
    for (size_t i = 0; i < size && i < 255; i++) {
        for (size_t j = 0; j < size && j < 255; j++) {
            // Create null-terminated substrings
            size_t len1 = (size - i < 255) ? size - i : 255;
            size_t len2 = (size - j < 255) ? size - j : 255;
            
            memset(buf1, 0, sizeof(buf1));
            memset(buf2, 0, sizeof(buf2));
            
            if (len1 > 0) memcpy(buf1, data + i, len1);
            if (len2 > 0) memcpy(buf2, data + j, len2);
            
            sqlite3_stricmp(buf1, buf2);
        }
    }
    
    // Test with NULL pointers
    sqlite3_stricmp(NULL, NULL);
    sqlite3_stricmp(NULL, (const char *)data);
    sqlite3_stricmp((const char *)data, NULL);
}

static void safe_fuzz_call_strglob(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create temporary null-terminated strings for safe glob matching
    char buf1[256];
    char buf2[256];
    
    for (size_t i = 0; i < size && i < 255; i++) {
        for (size_t j = 0; j < size && j < 255; j++) {
            // Create null-terminated substrings
            size_t len1 = (size - i < 255) ? size - i : 255;
            size_t len2 = (size - j < 255) ? size - j : 255;
            
            memset(buf1, 0, sizeof(buf1));
            memset(buf2, 0, sizeof(buf2));
            
            if (len1 > 0) memcpy(buf1, data + i, len1);
            if (len2 > 0) memcpy(buf2, data + j, len2);
            
            sqlite3_strglob(buf1, buf2);
        }
    }
    
    // Test with NULL pointers
    sqlite3_strglob(NULL, NULL);
    sqlite3_strglob(NULL, (const char *)data);
    sqlite3_strglob((const char *)data, NULL);
}

static void safe_fuzz_call_strlike(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create temporary null-terminated strings for safe like matching
    char buf1[256];
    char buf2[256];
    
    for (size_t i = 0; i < size && i < 255; i++) {
        for (size_t j = 0; j < size && j < 255; j++) {
            // Create null-terminated substrings
            size_t len1 = (size - i < 255) ? size - i : 255;
            size_t len2 = (size - j < 255) ? size - j : 255;
            
            memset(buf1, 0, sizeof(buf1));
            memset(buf2, 0, sizeof(buf2));
            
            if (len1 > 0) memcpy(buf1, data + i, len1);
            if (len2 > 0) memcpy(buf2, data + j, len2);
            
            unsigned int esc = 0;
            if (size > 2) {
                esc = data[(i + j) % size];
            }
            sqlite3_strlike(buf1, buf2, esc);
        }
    }
    
    // Test with NULL pointers
    sqlite3_strlike(NULL, NULL, 0);
    sqlite3_strlike(NULL, (const char *)data, 0);
    sqlite3_strlike((const char *)data, NULL, 0);
    
    // Test with various escape characters
    for (int i = 0; i < 256; i++) {
        if (size > 0) {
            char buf[256];
            size_t len = (size < 255) ? size : 255;
            memset(buf, 0, sizeof(buf));
            if (len > 0) memcpy(buf, data, len);
            sqlite3_strlike(buf, buf, (unsigned int)i);
        }
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        // Test with empty input
        safe_fuzz_call_keyword_check(Data, 0);
        safe_fuzz_call_keyword_name(Data, 0);
        safe_fuzz_call_strnicmp(Data, 0);
        safe_fuzz_call_stricmp(Data, 0);
        safe_fuzz_call_strglob(Data, 0);
        safe_fuzz_call_strlike(Data, 0);
        return 0;
    }
    
    // Call all fuzz functions
    safe_fuzz_call_keyword_check(Data, Size);
    safe_fuzz_call_keyword_name(Data, Size);
    safe_fuzz_call_strnicmp(Data, Size);
    safe_fuzz_call_stricmp(Data, Size);
    safe_fuzz_call_strglob(Data, Size);
    safe_fuzz_call_strlike(Data, Size);
    
    return 0;
}