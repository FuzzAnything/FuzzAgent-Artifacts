// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
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
#include "/promefuzz/database/libmagic/latest/build_asan/src/magic.h"

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Create main magic handle
    magic_t ms = magic_open(0);
    if (ms == NULL) {
        return 0;
    }

    // Test magic_load with default database
    int load_result = magic_load(ms, NULL);
    if (load_result == -1) {
        // If load fails, get error message
        const char *error_msg = magic_error(ms);
        (void)error_msg; // Use to avoid unused variable warning
    }

    // Test magic_load_buffers with fuzz data
    if (Size > 0) {
        void *buffers[1] = {(void *)Data};
        size_t sizes[1] = {Size};
        magic_load_buffers(ms, buffers, sizes, 1);
    }

    // Test magic_buffer with fuzz data
    if (Size > 0) {
        const char *buffer_result = magic_buffer(ms, Data, Size);
        (void)buffer_result; // Use to avoid unused variable warning
    }

    // Test magic_file with dummy file
    if (Size > 0) {
        // Write fuzz data to dummy file
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp != NULL) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            // Call magic_file
            const char *file_result = magic_file(ms, "./dummy_file");
            (void)file_result; // Use to avoid unused variable warning
            
            // Remove dummy file
            remove("./dummy_file");
        }
    }

    // Test magic_error after operations
    const char *final_error = magic_error(ms);
    (void)final_error; // Use to avoid unused variable warning

    // Test magic_check with separate handle
    magic_t check_ms = magic_open(0);
    if (check_ms != NULL) {
        // Create dummy file for magic_check
        if (Size > 0) {
            FILE *check_fp = fopen("./dummy_check_file", "wb");
            if (check_fp != NULL) {
                fwrite(Data, 1, Size, check_fp);
                fclose(check_fp);
                
                // Call magic_check
                int check_result = magic_check(check_ms, "./dummy_check_file");
                (void)check_result; // Use to avoid unused variable warning
                
                // Get error if any
                const char *check_error = magic_error(check_ms);
                (void)check_error; // Use to avoid unused variable warning
                
                // Remove dummy file
                remove("./dummy_check_file");
            }
        }
        magic_close(check_ms);
    }

    // Clean up main handle
    magic_close(ms);

    return 0;
}