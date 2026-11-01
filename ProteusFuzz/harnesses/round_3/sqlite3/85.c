// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strglob at sqlite3.c:133208:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
// sqlite3_strnicmp at sqlite3.c:36365:16 in sqlite3.h
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

static void test_keyword_check(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Test with the input data as the string
    // Ensure we don't pass a length that exceeds the actual buffer if it's not null-terminated
    // sqlite3_keyword_check expects a valid string, but it takes a length. 
    // However, the internal implementation might read past the length if not careful, 
    // or the fuzzer input might not be null-terminated.
    // The function signature is: int sqlite3_keyword_check(const char*, int);
    // It checks if the string of given length is a keyword.
    // We pass the data and size.
    sqlite3_keyword_check((const char *)Data, (int)Size);
    
    // Test with truncated lengths
    for (size_t i = 1; i < Size && i < 100; i++) {
        sqlite3_keyword_check((const char *)Data, (int)i);
    }
    
    // Test with zero length
    sqlite3_keyword_check((const char *)Data, 0);
}

static void test_keyword_name(const uint8_t *Data, size_t Size) {
    const char *pzName;
    int pnName;
    
    // SQLITE_N_KEYWORD is defined in sqlite3.h. 
    // We will try indices based on input data, but ensure they are reasonable.
    
    if (Size > 0) {
        // Try a small index derived from data
        int idx = (int)Data[0] % 100; 
        sqlite3_keyword_name(idx, &pzName, &pnName);
        
        // Test with negative index
        if (Size > 1) {
            int idx_neg = -(int)Data[1];
            sqlite3_keyword_name(idx_neg, &pzName, &pnName);
        }
        
        // Test with large index
        int idx_large = 1000 + (Size > 0 ? (int)Data[0] : 0);
        sqlite3_keyword_name(idx_large, &pzName, &pnName);
    }
    
    // Test with zero index
    sqlite3_keyword_name(0, &pzName, &pnName);
}

static void test_strnicmp(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Test with input as both strings
    // sqlite3_strnicmp compares up to N chars. It should be safe even if not null-terminated.
    sqlite3_strnicmp((const char *)Data, (const char *)Data, (int)Size);
    
    // Test with different lengths
    for (size_t i = 1; i < Size && i < 100; i++) {
        sqlite3_strnicmp((const char *)Data, (const char *)Data, (int)i);
    }
    
    // Test with NULL pointers
    sqlite3_strnicmp(NULL, (const char *)Data, (int)Size);
    sqlite3_strnicmp((const char *)Data, NULL, (int)Size);
    sqlite3_strnicmp(NULL, NULL, (int)Size);
    
    // Test with zero length
    sqlite3_strnicmp((const char *)Data, (const char *)Data, 0);
}

static void test_stricmp(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // sqlite3_stricmp expects null-terminated strings.
    // The fuzzer input 'Data' is not guaranteed to be null-terminated.
    // We must create a null-terminated copy or ensure the buffer has a null terminator.
    // Since we cannot modify the original Data buffer (it's const and from fuzzer),
    // we allocate a temporary buffer, copy the data, and add a null terminator.
    
    // Allocate buffer with size + 1 for null terminator
    char *temp_buf = (char *)malloc(Size + 1);
    if (!temp_buf) return; // Allocation failed
    
    memcpy(temp_buf, Data, Size);
    temp_buf[Size] = '\0'; // Null terminate
    
    // Test with input as both strings (now null-terminated)
    sqlite3_stricmp(temp_buf, temp_buf);
    
    // Test with NULL pointers
    sqlite3_stricmp(NULL, temp_buf);
    sqlite3_stricmp(temp_buf, NULL);
    sqlite3_stricmp(NULL, NULL);
    
    // Free the temporary buffer
    free(temp_buf);
}

static void test_strglob(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // sqlite3_strglob expects null-terminated strings.
    // Create a null-terminated copy.
    char *temp_buf = (char *)malloc(Size + 1);
    if (!temp_buf) return;
    
    memcpy(temp_buf, Data, Size);
    temp_buf[Size] = '\0';
    
    // Test with input as pattern and string
    sqlite3_strglob(temp_buf, temp_buf);
    
    // Test with NULL pointers
    sqlite3_strglob(NULL, temp_buf);
    sqlite3_strglob(temp_buf, NULL);
    sqlite3_strglob(NULL, NULL);
    
    // Split data into pattern and string if size allows
    if (Size >= 2) {
        size_t half = Size / 2;
        // We need two null-terminated strings for pattern and string
        char *temp_buf2 = (char *)malloc(Size + 1);
        if (temp_buf2) {
            memcpy(temp_buf2, Data + half, Size - half);
            temp_buf2[Size - half] = '\0';
            sqlite3_strglob(temp_buf, temp_buf2);
            free(temp_buf2);
        }
    }
    
    free(temp_buf);
}

static void test_strlike(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // sqlite3_strlike expects null-terminated strings.
    char *temp_buf = (char *)malloc(Size + 1);
    if (!temp_buf) return;
    
    memcpy(temp_buf, Data, Size);
    temp_buf[Size] = '\0';
    
    // Test with input as pattern and string
    sqlite3_strlike(temp_buf, temp_buf, 0);
    
    // Test with NULL pointers
    sqlite3_strlike(NULL, temp_buf, 0);
    sqlite3_strlike(temp_buf, NULL, 0);
    sqlite3_strlike(NULL, NULL, 0);
    
    // Test with different escape characters
    if (Size > 0) {
        unsigned int escape = (unsigned int)Data[0];
        sqlite3_strlike(temp_buf, temp_buf, escape);
    }
    
    // Split data into pattern and string if size allows
    if (Size >= 2) {
        size_t half = Size / 2;
        char *temp_buf2 = (char *)malloc(Size + 1);
        if (temp_buf2) {
            memcpy(temp_buf2, Data + half, Size - half);
            temp_buf2[Size - half] = '\0';
            sqlite3_strlike(temp_buf, temp_buf2, 0);
            free(temp_buf2);
        }
    }
    
    free(temp_buf);
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    // Test all target functions
    test_keyword_check(Data, Size);
    test_keyword_name(Data, Size);
    test_strnicmp(Data, Size);
    test_stricmp(Data, Size);
    test_strglob(Data, Size);
    test_strlike(Data, Size);
    
    return 0;
}