// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_version at magic.c:607:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <unistd.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have at least some data to write
    if (Size > 0) {
        write_dummy_file(Data, Size);
    }

    // Test magic_version (no arguments, no cleanup needed)
    (void)magic_version();

    // Test magic_getpath (no cleanup needed)
    const char *path = magic_getpath("./dummy_file", MAGIC_MIME_TYPE);
    if (path) {
        (void)magic_getpath(NULL, 0);
        (void)magic_getpath("./dummy_file", 0);
    }

    // Test magic_check
    magic_t check_magic = magic_open(0);
    if (check_magic) {
        (void)magic_check(check_magic, "./dummy_file");
        (void)magic_check(check_magic, NULL);
        (void)magic_check(check_magic, "nonexistent_file");
        magic_close(check_magic);
    }

    // Test magic_list
    // MAGIC_LIST is not a standard flag in magic_open, so we use 0 and rely on magic_list behavior
    magic_t list_magic = magic_open(0);
    if (list_magic) {
        (void)magic_list(list_magic, "./dummy_file");
        (void)magic_list(list_magic, NULL);
        (void)magic_list(list_magic, "nonexistent_file");
        magic_close(list_magic);
    }

    // Test magic_compile
    magic_t compile_magic = magic_open(0);
    if (compile_magic) {
        (void)magic_compile(compile_magic, "./dummy_file");
        (void)magic_compile(compile_magic, NULL);
        (void)magic_compile(compile_magic, "nonexistent_file");
        magic_close(compile_magic);
    }

    // Test magic_load
    magic_t load_magic = magic_open(0);
    if (load_magic) {
        (void)magic_load(load_magic, "./dummy_file");
        (void)magic_load(load_magic, NULL);
        (void)magic_load(load_magic, "nonexistent_file");
        magic_close(load_magic);
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}