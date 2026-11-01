// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_version at magic.c:607:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int create_dummy_magic_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    if (size > 0 && data != NULL) {
        size_t written = fwrite(data, 1, size, f);
        if (written != size) {
            fclose(f);
            return -1;
        }
    }
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    const char *dummy_file = "./dummy_file";
    const char *dummy_magic = "./dummy_magic";

    // 1. Test magic_version (no setup needed)
    int version = magic_version();
    (void)version; // Use the value to avoid unused variable warning

    // 2. Test magic_open and magic_load
    magic_t m = magic_open(0);
    if (m) {
        // Try to load the dummy file as a magic database (likely to fail, but tests error paths)
        int ret = magic_load(m, dummy_file);
        (void)ret;
        magic_close(m);
    }

    // 3. Test magic_open with flags and magic_load with valid-ish data
    // Create a dummy magic file from input data if available
    if (Size > 0) {
        if (create_dummy_magic_file(dummy_magic, Data, Size) == 0) {
            magic_t m2 = magic_open(MAGIC_MIME_TYPE);
            if (m2) {
                int ret = magic_load(m2, dummy_magic);
                (void)ret;
                magic_close(m2);
            }
        }
    }

    // 4. Test magic_check (requires a separate magic_set)
    if (Size > 0) {
        if (create_dummy_magic_file(dummy_magic, Data, Size) == 0) {
            magic_t m_check = magic_open(0);
            if (m_check) {
                int ret = magic_check(m_check, dummy_magic);
                (void)ret;
                magic_close(m_check);
            }
        }
    }

    // 5. Test magic_compile (requires a separate magic_set)
    if (Size > 0) {
        if (create_dummy_magic_file(dummy_magic, Data, Size) == 0) {
            magic_t m_compile = magic_open(0);
            if (m_compile) {
                int ret = magic_compile(m_compile, dummy_magic);
                (void)ret;
                magic_close(m_compile);
            }
        }
    }

    // 6. Test magic_list (requires a separate magic_set)
    if (Size > 0) {
        if (create_dummy_magic_file(dummy_magic, Data, Size) == 0) {
            magic_t m_list = magic_open(0);
            if (m_list) {
                int ret = magic_list(m_list, dummy_magic);
                (void)ret;
                magic_close(m_list);
            }
        }
    }

    // Cleanup: Remove dummy files
    unlink(dummy_file);
    unlink(dummy_magic);

    return 0;
}