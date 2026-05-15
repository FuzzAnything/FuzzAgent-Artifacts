// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_keyword_check at sqlite3.c:169743:16 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:169742:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:169742:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:174799:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_sqlite3_keyword_check(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use the input data as the keyword to check
    // Ensure null termination for safety
    char *keyword = malloc(size + 1);
    if (!keyword) return;
    
    memcpy(keyword, data, size);
    keyword[size] = '\0';
    
    sqlite3_keyword_check(keyword, (int)size);
    free(keyword);
}

static void fuzz_sqlite3_keyword_name(void) {
    int count = sqlite3_keyword_count();
    
    // Test a few indices, including out of bounds
    for (int i = -1; i <= count + 1; i++) {
        const char *keyword = NULL;
        int length = 0;
        sqlite3_keyword_name(i, &keyword, &length);
    }
}

static void fuzz_sqlite3_compileoption_used(const uint8_t *data, size_t size) {
    if (size == 0) {
        // Test with NULL input
        sqlite3_compileoption_used(NULL);
        return;
    }
    
    // Use input as option name
    char *option = malloc(size + 1);
    if (!option) return;
    
    memcpy(option, data, size);
    option[size] = '\0';
    
    sqlite3_compileoption_used(option);
    free(option);
}

static void fuzz_sqlite3_compileoption_get(void) {
    // Test a range of indices
    for (int i = -10; i < 100; i++) {
        sqlite3_compileoption_get(i);
    }
}

int LLVMFuzzerTestOneInput_107(const uint8_t *data, size_t size) {
    // 1. Test sqlite3_keyword_check
    fuzz_sqlite3_keyword_check(data, size);
    
    // 2. Test sqlite3_keyword_count
    sqlite3_keyword_count();
    
    // 3. Test sqlite3_compileoption_used
    fuzz_sqlite3_compileoption_used(data, size);
    
    // 4. Test sqlite3_global_recover
    sqlite3_global_recover();
    
    // 5. Test sqlite3_compileoption_get
    fuzz_sqlite3_compileoption_get();
    
    // 6. Test sqlite3_keyword_name
    fuzz_sqlite3_keyword_name();
    
    return 0;
}