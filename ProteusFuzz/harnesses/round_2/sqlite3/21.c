// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3 API functions.
 * Target sequence: sqlite3_shutdown -> sqlite3_config -> sqlite3_hard_heap_limit64 
 *                  -> sqlite3_hard_heap_limit64 -> sqlite3_config -> sqlite3_snprintf
 */

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    char buffer[1024];
    int buffer_size = sizeof(buffer);
    sqlite3_int64 limit;
    int rc;
    int config_option;
    int config_option2;
    int n;
    char format[256];
    int arg1;
    const char *arg2;
    size_t str_len;
    char temp[256];

    // Step 1: sqlite3_shutdown
    // Call once to ensure clean state before configuration
    rc = sqlite3_shutdown();
    (void)rc;

    // Step 2: sqlite3_config
    // Use data from fuzz input to select a configuration option if possible
    config_option = SQLITE_CONFIG_SINGLETHREAD;
    if (Size > 0) {
        config_option = Data[0] % 3; // 0, 1, or 2
        if (config_option == 0) config_option = SQLITE_CONFIG_SINGLETHREAD;
        else if (config_option == 1) config_option = SQLITE_CONFIG_MULTITHREAD;
        else config_option = SQLITE_CONFIG_SERIALIZED;
    }
    rc = sqlite3_config(config_option);
    (void)rc;

    // Step 3: sqlite3_hard_heap_limit64 (First call)
    // Use fuzz data to set a limit
    sqlite3_int64 limit1 = 0;
    if (Size >= 8) {
        // Ensure alignment safety by reading bytes manually or casting carefully
        // Here we assume the fuzzer provides aligned data or we handle misalignment
        // To be safe, we read 8 bytes and construct the int64
        uint64_t raw_val = 0;
        for(int i=0; i<8; i++) {
            raw_val |= ((uint64_t)Data[i]) << (i*8);
        }
        limit1 = (sqlite3_int64)raw_val;
        if (limit1 < 0) limit1 = -1;
    } else if (Size > 0) {
        limit1 = (sqlite3_int64)Data[0] * 1024;
    }
    limit = sqlite3_hard_heap_limit64(limit1);

    // Step 4: sqlite3_hard_heap_limit64 (Second call)
    // Use different data or query current limit
    sqlite3_int64 limit2 = 0;
    if (Size >= 16) {
        uint64_t raw_val2 = 0;
        for(int i=0; i<8; i++) {
            raw_val2 |= ((uint64_t)Data[8+i]) << (i*8);
        }
        limit2 = (sqlite3_int64)raw_val2;
        if (limit2 < 0) limit2 = -1;
    } else if (Size > 8) {
        limit2 = (sqlite3_int64)Data[8] * 2048;
    }
    limit = sqlite3_hard_heap_limit64(limit2);

    // Step 5: sqlite3_config (Second call)
    // Try another configuration option
    config_option2 = SQLITE_CONFIG_SINGLETHREAD;
    if (Size > 1) {
        config_option2 = Data[1] % 3;
        if (config_option2 == 0) config_option2 = SQLITE_CONFIG_SINGLETHREAD;
        else if (config_option2 == 1) config_option2 = SQLITE_CONFIG_MULTITHREAD;
        else config_option2 = SQLITE_CONFIG_SERIALIZED;
    }
    rc = sqlite3_config(config_option2);
    (void)rc;

    // Step 6: sqlite3_snprintf
    // Use fuzz data for format string and arguments
    // Ensure null termination for format string
    n = 0;
    if (Size > 0) {
        n = Data[0] % 100; // Limit format string length
        if (n > Size - 1) n = Size - 1;
    }
    
    if (Size > 0 && n > 0) {
        memcpy(format, Data, n);
        format[n] = '\0';
    } else {
        strcpy(format, "%d %s");
    }

    // Prepare arguments based on available data
    arg1 = 0;
    arg2 = "test";
    
    // Check if we have enough data for arg1 (4 bytes) after the format string
    if (Size >= (size_t)(n + 5)) {
        // Read 4 bytes for arg1 safely
        uint32_t raw_arg1 = 0;
        for(int i=0; i<4; i++) {
            raw_arg1 |= ((uint32_t)Data[n + 1 + i]) << (i*8);
        }
        arg1 = (int)raw_arg1;
    }

    if (Size > (size_t)(n + 5)) {
        // Find a null-terminated string or use a substring
        str_len = 0;
        const uint8_t *p = Data + n + 5;
        const uint8_t *end = Data + Size;
        while (p < end && *p != '\0') {
            str_len++;
            p++;
        }
        if (str_len > 0 && str_len < 256) {
            memcpy(temp, Data + n + 5, str_len);
            temp[str_len] = '\0';
            arg2 = temp;
        }
    }

    sqlite3_snprintf(buffer_size, buffer, format, arg1, arg2);

    // Cleanup is minimal as sqlite3_shutdown was called at the start
    // and functions used are global or stateless in this context.
    return 0;
}