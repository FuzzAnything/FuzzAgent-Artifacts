// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_keyword_count at sqlite3.c:169742:16 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:252014:24 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:169743:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:169743:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:169743:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    // Function 1: sqlite3_keyword_count
    int keyword_count = sqlite3_keyword_count();
    
    // Function 2: sqlite3_sourceid
    const char *source_id = sqlite3_sourceid();
    
    // Function 3: sqlite3_compileoption_used
    // Create a null-terminated string from fuzzer input for option name
    char *opt_name = NULL;
    if (Size > 0) {
        opt_name = (char *)malloc(Size + 1);
        if (opt_name) {
            memcpy(opt_name, Data, Size);
            opt_name[Size] = '\0';
            int used = sqlite3_compileoption_used(opt_name);
            free(opt_name);
        }
    } else {
        // Test with NULL input
        sqlite3_compileoption_used(NULL);
    }
    
    // Function 4: sqlite3_compileoption_get
    // Try multiple indices to explore different options
    for (int i = 0; i < 20; i++) {
        const char *opt = sqlite3_compileoption_get(i);
        if (opt == NULL) break;
    }
    
    // Function 5: sqlite3_keyword_name
    if (keyword_count > 0) {
        // Try to access some keyword names
        for (int i = 0; i < keyword_count && i < 10; i++) {
            const char *keyword = NULL;
            int length = 0;
            int result = sqlite3_keyword_name(i, &keyword, &length);
        }
        
        // Test out-of-bounds access
        sqlite3_keyword_name(-1, NULL, NULL);
        sqlite3_keyword_name(keyword_count, NULL, NULL);
        sqlite3_keyword_name(keyword_count + 100, NULL, NULL);
    }
    
    // Function 6: sqlite3_keyword_check
    // Create test strings from fuzzer input
    if (Size > 0) {
        // Use the entire input as a potential keyword
        char *test_str = (char *)malloc(Size + 1);
        if (test_str) {
            memcpy(test_str, Data, Size);
            test_str[Size] = '\0';
            
            // Check with different lengths
            int check_result = sqlite3_keyword_check(test_str, (int)Size);
            
            // Also check with null-terminated string length
            if (Size < 100) {  // Avoid very long strings
                int check_result2 = sqlite3_keyword_check(test_str, (int)strlen(test_str));
            }
            
            free(test_str);
        }
    }
    
    // Test with empty string
    sqlite3_keyword_check("", 0);
    
    // Test with some common SQL keywords
    static const char *common_keywords[] = {
        "SELECT", "INSERT", "UPDATE", "DELETE", "CREATE",
        "DROP", "TABLE", "INDEX", "VIEW", "WHERE"
    };
    
    for (int i = 0; i < 10; i++) {
        sqlite3_keyword_check(common_keywords[i], (int)strlen(common_keywords[i]));
    }
    
    return 0;
}