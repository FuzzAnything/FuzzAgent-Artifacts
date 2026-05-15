// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_files(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file for testing
    write_dummy_file(Data, Size);
    
    // Test magic_open with various flags from input
    int flags = Data[0] & 0xFF;
    magic_t ms = magic_open(flags);
    if (!ms) {
        cleanup_files();
        return 0;
    }
    
    // Test magic_load with different database paths
    const char *db_path = NULL;
    if (Size > 1 && (Data[1] & 0x01)) {
        // Use default database (NULL)
        db_path = NULL;
    } else {
        // Use dummy file as database (will likely fail, but we want to test error paths)
        db_path = "./dummy_file";
    }
    
    int load_result = magic_load(ms, db_path);
    
    // Test magic_file if load succeeded or use different approaches
    const char *file_result = NULL;
    if (load_result == 0) {
        file_result = magic_file(ms, "./dummy_file");
    } else {
        // Even if load failed, try magic_file to test error handling
        file_result = magic_file(ms, "./dummy_file");
    }
    
    // Test magic_error after various operations
    const char *error_msg = magic_error(ms);
    
    // Test magic_check with a separate magic_set as required
    magic_t ms_check = magic_open(flags);
    if (ms_check) {
        int check_result = magic_check(ms_check, db_path);
        // Test magic_error on the separate handle
        const char *check_error = magic_error(ms_check);
        magic_close(ms_check);
    }
    
    // Test with different flags combinations
    if (Size > 2) {
        int alt_flags = Data[2] & 0xFF;
        magic_t ms2 = magic_open(alt_flags);
        if (ms2) {
            magic_load(ms2, NULL);
            magic_file(ms2, "./dummy_file");
            magic_error(ms2);
            magic_close(ms2);
        }
    }
    
    // Test magic_file with NULL filename (reads from stdin)
    if (Size > 3 && (Data[3] & 0x01)) {
        // Note: This would require setting up stdin, which we can't easily do in fuzzer
        // So we'll just test the API call
        magic_file(ms, NULL);
    }
    
    // Clean up
    magic_close(ms);
    cleanup_files();
    
    return 0;
}