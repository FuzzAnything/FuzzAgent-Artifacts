// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "build_asan/src/magic.h"

static void write_to_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy file with fuzzer data
    write_to_dummy_file(Data, Size);

    // Test magic_open with various flags from fuzzer input
    int flags = Data[0] % 32;  // Use first byte for flags
    magic_t cookie = magic_open(flags);
    if (cookie == NULL) {
        return 0;
    }

    // Test magic_load with different database paths
    const char *db_path = NULL;
    if (Size > 1 && Data[1] % 2 == 0) {
        db_path = "./dummy_file";  // Use fuzzer data as magic database
    }
    int load_result = magic_load(cookie, db_path);

    // Test magic_file with different inputs
    const char *filename = "./dummy_file";
    if (Size > 2 && Data[2] % 3 == 0) {
        filename = NULL;  // Test reading from stdin
    }
    const char *file_result = magic_file(cookie, filename);

    // Test magic_error
    const char *error_msg = magic_error(cookie);

    // Test magic_check with a SEPARATE magic_set as required
    magic_t check_cookie = magic_open(flags);
    if (check_cookie != NULL) {
        const char *check_path = "./dummy_file";
        if (Size > 3 && Data[3] % 2 == 0) {
            check_path = NULL;
        }
        int check_result = magic_check(check_cookie, check_path);
        const char *check_error = magic_error(check_cookie);
        magic_close(check_cookie);
    }

    // Cleanup
    magic_close(cookie);

    // Remove dummy file
    unlink("./dummy_file");

    return 0;
}