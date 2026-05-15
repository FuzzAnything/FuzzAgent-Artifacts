// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create dummy file with fuzzer data
    write_dummy_file(Data, Size);

    // Test magic_open with various flags
    int flags = 0;
    if (Size > 0) {
        // Use first byte to determine some flags
        uint8_t flag_byte = Data[0];
        if (flag_byte & 0x01) flags |= MAGIC_NONE;
        if (flag_byte & 0x02) flags |= MAGIC_DEBUG;
        if (flag_byte & 0x04) flags |= MAGIC_SYMLINK;
        if (flag_byte & 0x08) flags |= MAGIC_COMPRESS;
        if (flag_byte & 0x10) flags |= MAGIC_DEVICES;
        if (flag_byte & 0x20) flags |= MAGIC_MIME_TYPE;
        if (flag_byte & 0x40) flags |= MAGIC_CONTINUE;
        if (flag_byte & 0x80) flags |= MAGIC_CHECK;
    }

    magic_t cookie = magic_open(flags);
    if (cookie == NULL) {
        cleanup_dummy_file();
        return 0;
    }

    // Test magic_load with default database
    int load_result = magic_load(cookie, NULL);
    
    // Test magic_file
    const char *file_result = magic_file(cookie, "./dummy_file");
    
    // Test magic_error after operations
    const char *error_msg = magic_error(cookie);
    
    // Test magic_buffer
    const char *buffer_result = magic_buffer(cookie, Data, Size);
    
    // Test magic_check with a separate handle
    magic_t check_cookie = magic_open(MAGIC_NONE);
    if (check_cookie != NULL) {
        // Write fuzzer data to a temporary file for magic_check
        FILE *check_fp = fopen("./check_file", "wb");
        if (check_fp) {
            fwrite(Data, 1, Size, check_fp);
            fclose(check_fp);
            
            int check_result = magic_check(check_cookie, "./check_file");
            (void)check_result; // Use result to avoid unused variable warning
            
            unlink("./check_file");
        }
        magic_close(check_cookie);
    }

    // Test with different flags combinations
    if (Size > 1) {
        int alt_flags = Data[1] * 256 + Data[0];
        magic_t alt_cookie = magic_open(alt_flags);
        if (alt_cookie != NULL) {
            magic_load(alt_cookie, NULL);
            const char *alt_file_result = magic_file(alt_cookie, "./dummy_file");
            const char *alt_buffer_result = magic_buffer(alt_cookie, Data, Size);
            magic_close(alt_cookie);
        }
    }

    // Test with MIME flags specifically
    if (Size > 2) {
        magic_t mime_cookie = magic_open(MAGIC_MIME_TYPE | MAGIC_MIME_ENCODING);
        if (mime_cookie != NULL) {
            magic_load(mime_cookie, NULL);
            const char *mime_result = magic_file(mime_cookie, "./dummy_file");
            magic_close(mime_cookie);
        }
    }

    // Clean up main cookie
    magic_close(cookie);
    
    // Clean up dummy file
    cleanup_dummy_file();

    // Use results to avoid unused variable warnings
    (void)load_result;
    (void)file_result;
    (void)error_msg;
    (void)buffer_result;

    return 0;
}