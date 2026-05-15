// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_keyword_check at sqlite3.c:169743:16 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:174799:16 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:169742:16 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:169742:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_sqlite3_keyword_check(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *str = malloc(size + 1);
    if (!str) return;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    sqlite3_keyword_check(str, (int)size);
    
    free(str);
}

static void fuzz_sqlite3_compileoption_get(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int N = *(int*)data;
    const char *result = sqlite3_compileoption_get(N);
    (void)result;
}

static void fuzz_sqlite3_keyword_name(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int index = *(int*)data;
    const char *keyword = NULL;
    int length = 0;
    
    sqlite3_keyword_name(index, &keyword, &length);
}

static void fuzz_sqlite3_compileoption_used(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *opt_name = malloc(size + 1);
    if (!opt_name) return;
    
    memcpy(opt_name, data, size);
    opt_name[size] = '\0';
    
    sqlite3_compileoption_used(opt_name);
    
    free(opt_name);
}

int LLVMFuzzerTestOneInput_64(const uint8_t *data, size_t size) {
    fuzz_sqlite3_keyword_check(data, size);
    
    fuzz_sqlite3_compileoption_get(data, size);
    
    sqlite3_global_recover();
    
    fuzz_sqlite3_keyword_name(data, size);
    
    fuzz_sqlite3_compileoption_used(data, size);
    
    int keyword_count = sqlite3_keyword_count();
    (void)keyword_count;
    
    if (size >= sizeof(int)) {
        int index = *(int*)data;
        int count = sqlite3_keyword_count();
        
        if (count > 0) {
            int valid_index = abs(index) % count;
            const char *keyword = NULL;
            int length = 0;
            sqlite3_keyword_name(valid_index, &keyword, &length);
        }
    }
    
    return 0;
}