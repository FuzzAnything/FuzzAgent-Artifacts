// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getpath at magic.c:254:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. magic_getpath
    const char *path = magic_getpath(NULL, 0);
    (void)path; // Use the return value to avoid unused warnings

    // 2. magic_version
    int v1 = magic_version();
    (void)v1;

    // 3. magic_version (called again as per sequence)
    int v2 = magic_version();
    (void)v2;

    // 4. magic_open
    magic_t main_handle = magic_open(MAGIC_NONE);
    if (main_handle == NULL) {
        return 0;
    }

    // 5. magic_check
    // Hint: Requires a NEW magic_set structure
    magic_t check_handle = magic_open(MAGIC_NONE);
    if (check_handle != NULL) {
        write_dummy_file(Data, Size);
        magic_check(check_handle, "./dummy_file");
        magic_close(check_handle);
    }

    // 6. magic_compile
    write_dummy_file(Data, Size);
    magic_compile(main_handle, "./dummy_file");

    // 7. magic_list
    // Hint: Requires a NEW magic_set structure
    magic_t list_handle = magic_open(MAGIC_NONE);
    if (list_handle != NULL) {
        write_dummy_file(Data, Size);
        magic_list(list_handle, "./dummy_file");
        magic_close(list_handle);
    }

    // 8. magic_error
    const char *err = magic_error(main_handle);
    (void)err;

    // 9. magic_close
    magic_close(main_handle);

    return 0;
}