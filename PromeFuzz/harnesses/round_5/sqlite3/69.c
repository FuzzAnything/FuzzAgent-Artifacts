// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:169742:16 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:252014:24 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:169743:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    // Function 1: sqlite3_compileoption_get
    // Use first 4 bytes as integer N for the option index
    if (Size >= sizeof(int)) {
        int N = *(int*)Data;
        sqlite3_compileoption_get(N);
    }

    // Function 2: sqlite3_keyword_count
    int keyword_count = sqlite3_keyword_count();
    
    // Function 3: sqlite3_sourceid
    const char *source_id = sqlite3_sourceid();
    (void)source_id; // Prevent unused variable warning
    
    // Function 4: sqlite3_keyword_name
    // Use keyword_count to bound the index
    if (keyword_count > 0 && Size >= sizeof(int)) {
        int idx = *(int*)Data % keyword_count;
        if (idx < 0) idx = -idx;
        
        const char *keyword;
        int length;
        sqlite3_keyword_name(idx, &keyword, &length);
    }
    
    // Function 5: sqlite3_compileoption_used
    // Create a null-terminated string from the input data
    if (Size > 0) {
        // Allocate buffer with space for null terminator
        char *opt_name = (char*)malloc(Size + 1);
        if (opt_name) {
            memcpy(opt_name, Data, Size);
            opt_name[Size] = '\0';
            sqlite3_compileoption_used(opt_name);
            free(opt_name);
        }
    }
    
    // Function 6: sqlite3_keyword_check
    // Check if the input data (as string) is a keyword
    if (Size > 0) {
        // Create a string from the input data
        char *test_string = (char*)malloc(Size + 1);
        if (test_string) {
            memcpy(test_string, Data, Size);
            test_string[Size] = '\0';
            sqlite3_keyword_check(test_string, Size);
            free(test_string);
        }
    }
    
    return 0;
}