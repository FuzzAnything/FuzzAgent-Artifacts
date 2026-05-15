// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_operations(sqlite3_str *str, const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use first byte to decide operation mix
    uint8_t op_seed = data[0];
    size_t data_idx = 1;
    
    // Append some data if available
    if (data_idx < size) {
        size_t append_len = (size - data_idx) % 256;
        if (append_len > 0) {
            sqlite3_str_append(str, (const char*)data + data_idx, (int)append_len);
            data_idx += append_len;
        }
    }
    
    // Check length and error code
    (void)sqlite3_str_length(str);
    (void)sqlite3_str_errcode(str);
    
    // Truncate based on next byte if available
    if (data_idx < size && (op_seed & 0x01)) {
        int trunc_len = data[data_idx] % 64;
        sqlite3_str_truncate(str, trunc_len);
        data_idx++;
    }
    
    // Append formatted text if we have enough data
    if (data_idx + 2 < size && (op_seed & 0x02)) {
        // Use a simple format string with one integer parameter
        int fmt_param = data[data_idx];
        sqlite3_str_appendf(str, "Num:%d", fmt_param);
        data_idx++;
    }
    
    // Append characters if we have data
    if (data_idx + 1 < size && (op_seed & 0x04)) {
        int count = data[data_idx] % 32;
        char ch = (char)data[data_idx + 1];
        sqlite3_str_appendchar(str, count, ch);
        data_idx += 2;
    }
    
    // More appends with remaining data
    while (data_idx < size) {
        size_t chunk = (size - data_idx) % 128;
        if (chunk == 0) break;
        sqlite3_str_append(str, (const char*)data + data_idx, (int)chunk);
        data_idx += chunk;
        
        // Interleave with length checks
        (void)sqlite3_str_length(str);
    }
    
    // Final error check
    (void)sqlite3_str_errcode(str);
}

int LLVMFuzzerTestOneInput_51(const uint8_t *data, size_t size) {
    // Create a string accumulator
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (str == NULL) {
        return 0;
    }
    
    // Perform fuzzing operations
    fuzz_operations(str, data, size);
    
    // Clean up
    char *result = sqlite3_str_finish(str);
    if (result != NULL) {
        sqlite3_free(result);
    }
    
    return 0;
}