// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_keyword_check at sqlite3.c:169743:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:169742:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:252014:24 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:169742:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_keyword_check(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use the fuzzer input as the keyword to check
    // Make a null-terminated copy since sqlite3_keyword_check expects a C string
    char *keyword = (char *)malloc(size + 1);
    if (!keyword) return;
    
    memcpy(keyword, data, size);
    keyword[size] = '\0';
    
    // Call the target function
    sqlite3_keyword_check(keyword, (int)size);
    
    free(keyword);
}

static void fuzz_compileoption_used(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use the fuzzer input as the option name
    char *option = (char *)malloc(size + 1);
    if (!option) return;
    
    memcpy(option, data, size);
    option[size] = '\0';
    
    // Call the target function
    sqlite3_compileoption_used(option);
    
    free(option);
}

static void fuzz_compileoption_get(const uint8_t *data, size_t size) {
    // Use the first 4 bytes as integer N, or use size if smaller
    int N = 0;
    if (size >= 4) {
        N = *(int *)data;
    } else if (size > 0) {
        N = (int)data[0];
    }
    
    // Call the target function
    sqlite3_compileoption_get(N);
}

static void fuzz_keyword_name(const uint8_t *data, size_t size) {
    // Get total keyword count first
    int count = sqlite3_keyword_count();
    if (count <= 0) return;
    
    // Use fuzzer input to determine index (mod count to stay in bounds)
    int index = 0;
    if (size >= 4) {
        index = *(int *)data % count;
    } else if (size > 0) {
        index = (int)data[0] % count;
    }
    
    // Ensure index is non-negative
    if (index < 0) index = -index % count;
    
    const char *keyword = NULL;
    int length = 0;
    
    // Call the target function
    sqlite3_keyword_name(index, &keyword, &length);
}

int LLVMFuzzerTestOneInput_91(const uint8_t *data, size_t size) {
    // Fuzz sqlite3_keyword_check
    fuzz_keyword_check(data, size);
    
    // Fuzz sqlite3_sourceid (no input needed)
    (void)sqlite3_sourceid();
    
    // Fuzz sqlite3_compileoption_used
    fuzz_compileoption_used(data, size);
    
    // Fuzz sqlite3_compileoption_get
    fuzz_compileoption_get(data, size);
    
    // Fuzz sqlite3_keyword_count (no input needed)
    (void)sqlite3_keyword_count();
    
    // Fuzz sqlite3_keyword_name
    fuzz_keyword_name(data, size);
    
    return 0;
}