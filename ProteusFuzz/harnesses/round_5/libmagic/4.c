// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_version at magic.c:607:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test magic_version (no side effects, no cleanup needed)
    (void)magic_version();

    // Test magic_open and magic_load
    magic_t m1 = magic_open(0);
    if (m1) {
        write_dummy_file(Data, Size);
        (void)magic_load(m1, "./dummy_file");
        magic_close(m1);
    }

    // Test magic_open and magic_compile
    magic_t m2 = magic_open(0);
    if (m2) {
        write_dummy_file(Data, Size);
        (void)magic_compile(m2, "./dummy_file");
        magic_close(m2);
    }

    // Test magic_open and magic_list (requires separate handle)
    magic_t m3 = magic_open(0);
    if (m3) {
        write_dummy_file(Data, Size);
        (void)magic_list(m3, "./dummy_file");
        magic_close(m3);
    }

    // Test magic_open and magic_check (requires separate handle)
    magic_t m4 = magic_open(0);
    if (m4) {
        write_dummy_file(Data, Size);
        (void)magic_check(m4, "./dummy_file");
        magic_close(m4);
    }

    // Test with different flags to explore more states
    // Using only standard flags known to exist in libmagic
    int flags[] = {
        0,
        MAGIC_DEBUG,
        MAGIC_SYMLINK,
        MAGIC_NO_CHECK_COMPRESS,
        MAGIC_NO_CHECK_SOFT,
        MAGIC_NO_CHECK_ENCODING,
        MAGIC_NO_CHECK_TOKENS,
        MAGIC_NO_CHECK_BUILTIN,
        MAGIC_NO_CHECK_CSV,
        MAGIC_NO_CHECK_TEXT,
        MAGIC_NO_CHECK_TROFF,
        MAGIC_MIME_TYPE,
        MAGIC_MIME_ENCODING,
        MAGIC_RAW,
        MAGIC_ERROR,
        MAGIC_PRESERVE_ATIME
    };
    size_t num_flags = sizeof(flags) / sizeof(flags[0]);

    for (size_t i = 0; i < num_flags; i++) {
        magic_t m = magic_open(flags[i]);
        if (m) {
            write_dummy_file(Data, Size);
            (void)magic_load(m, "./dummy_file");
            magic_close(m);
        }
    }

    return 0;
}