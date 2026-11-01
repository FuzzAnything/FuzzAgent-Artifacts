// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_version at magic.c:607:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test magic_version (no state required)
    (void)magic_version();

    // Test magic_getpath (no state required)
    const char *path = magic_getpath(NULL, 0);
    (void)path;

    // Prepare a dummy file for testing
    write_dummy_file(Data, Size);

    // Test magic_check with a new handle
    magic_t mg_check = magic_open(MAGIC_CHECK);
    if (mg_check) {
        (void)magic_check(mg_check, "./dummy_file");
        magic_close(mg_check);
    }

    // Test magic_compile with a new handle
    magic_t mg_compile = magic_open(MAGIC_RAW);
    if (mg_compile) {
        (void)magic_compile(mg_compile, "./dummy_file");
        magic_close(mg_compile);
    }

    // Test magic_load with a new handle
    magic_t mg_load = magic_open(MAGIC_RAW);
    if (mg_load) {
        (void)magic_load(mg_load, "./dummy_file");
        magic_close(mg_load);
    }

    // Test magic_list with a new handle
    // MAGIC_LIST is not a standard flag for magic_open in all versions.
    // We use MAGIC_RAW and rely on magic_list to set the internal list mode or fail gracefully.
    // Alternatively, we just pass the handle to magic_list which expects a valid handle.
    magic_t mg_list = magic_open(MAGIC_RAW);
    if (mg_list) {
        (void)magic_list(mg_list, "./dummy_file");
        magic_close(mg_list);
    }

    // Test with invalid handle (NULL) for all functions that take magic_t
    (void)magic_check(NULL, "./dummy_file");
    (void)magic_compile(NULL, "./dummy_file");
    (void)magic_load(NULL, "./dummy_file");
    (void)magic_list(NULL, "./dummy_file");

    // Test with invalid path
    magic_t mg_invalid_path = magic_open(MAGIC_RAW);
    if (mg_invalid_path) {
        (void)magic_check(mg_invalid_path, "/nonexistent/path/magic");
        (void)magic_compile(mg_invalid_path, "/nonexistent/path/magic");
        (void)magic_load(mg_invalid_path, "/nonexistent/path/magic");
        (void)magic_list(mg_invalid_path, "/nonexistent/path/magic");
        magic_close(mg_invalid_path);
    }

    return 0;
}