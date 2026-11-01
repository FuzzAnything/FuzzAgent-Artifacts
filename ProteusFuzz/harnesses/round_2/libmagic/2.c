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
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have some data to write to the dummy file
    if (Size > 1024) {
        Size = 1024;
    }
    write_dummy_file(Data, Size);

    // 1. magic_getpath
    const char *path = magic_getpath(NULL, 0);
    (void)path; // Use the return value to avoid warnings

    // 2. magic_version (first call)
    int version1 = magic_version();
    (void)version1;

    // 3. magic_version (second call)
    int version2 = magic_version();
    (void)version2;

    // 4. magic_open (main handle)
    magic_t main_handle = magic_open(MAGIC_MIME_TYPE);
    if (main_handle == NULL) {
        return 0;
    }

    // 5. magic_check (requires a separate handle)
    magic_t check_handle = magic_open(MAGIC_MIME_TYPE);
    if (check_handle != NULL) {
        magic_check(check_handle, "./dummy_file");
        magic_close(check_handle);
    }

    // 6. magic_compile
    magic_compile(main_handle, "./dummy_file");

    // 7. magic_list (requires a separate handle)
    magic_t list_handle = magic_open(MAGIC_MIME_TYPE);
    if (list_handle != NULL) {
        magic_list(list_handle, "./dummy_file");
        magic_close(list_handle);
    }

    // 8. magic_error
    const char *err_msg = magic_error(main_handle);
    (void)err_msg;

    // 9. magic_close (main handle)
    magic_close(main_handle);

    return 0;
}