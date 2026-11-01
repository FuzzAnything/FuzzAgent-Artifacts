// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test 1: magic_open, magic_check, magic_close (separate handle)
    magic_t handle_check = magic_open(MAGIC_MIME_TYPE);
    if (handle_check) {
        // Use input data as a potential magic file content for checking
        if (write_dummy_file(Data, Size) == 0) {
            magic_check(handle_check, "./dummy_file");
        }
        magic_close(handle_check);
    }

    // Test 2: magic_open, magic_load, magic_file, magic_close
    magic_t handle_load = magic_open(MAGIC_MIME_TYPE);
    if (handle_load) {
        if (write_dummy_file(Data, Size) == 0) {
            magic_load(handle_load, "./dummy_file");
            // Try to identify the dummy file itself
            magic_file(handle_load, "./dummy_file");
        }
        magic_close(handle_load);
    }

    // Test 3: magic_open, magic_list, magic_close (separate handle)
    magic_t handle_list = magic_open(MAGIC_MIME_TYPE);
    if (handle_list) {
        if (write_dummy_file(Data, Size) == 0) {
            magic_list(handle_list, "./dummy_file");
        }
        magic_close(handle_list);
    }

    // Test 4: Error handling with NULL handles (implicit in API, but good to ensure no crash)
    magic_check(NULL, "./dummy_file");
    magic_load(NULL, "./dummy_file");
    magic_file(NULL, "./dummy_file");
    magic_list(NULL, "./dummy_file");
    magic_close(NULL);

    // Test 5: Invalid file path
    magic_t handle_invalid = magic_open(MAGIC_MIME_TYPE);
    if (handle_invalid) {
        magic_load(handle_invalid, "/nonexistent/path/magic.mgc");
        magic_file(handle_invalid, "/nonexistent/path/file");
        magic_check(handle_invalid, "/nonexistent/path/magic.mgc");
        magic_list(handle_invalid, "/nonexistent/path/magic.mgc");
        magic_close(handle_invalid);
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}