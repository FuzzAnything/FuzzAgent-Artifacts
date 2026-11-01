// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <magic.h>

static void cleanup_magic(magic_t cookie) {
    if (cookie != NULL) {
        magic_close(cookie);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    size_t param_val;
    const char *result;
    int ret;

    // Initialize magic handle
    cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Test magic_setflags
    if (Size > 0) {
        int flags = (int)Data[0] & 0xFF;
        magic_setflags(cookie, flags);
    }

    // Test magic_setparam with various parameters
    // We iterate through a range of potential parameter IDs to cover different states
    // since specific MAGIC_PARAM_* constants might not be exposed or named differently in some builds.
    // Common known IDs: 0=INDIRECT_MAX, 1=REGEX_MAX, 2=NAME_MAX, 3=ELF_SHNUM_MAX, etc.
    if (Size > 1) {
        size_t val = (size_t)Data[1];
        // Try setting a few known parameter indices if available, or just generic ones
        // MAGIC_PARAM_INDIRECT_MAX is typically 0
        magic_setparam(cookie, 0, &val); 
        // MAGIC_PARAM_REGEX_MAX is typically 1
        magic_setparam(cookie, 1, &val);
        // MAGIC_PARAM_NAME_MAX is typically 2
        magic_setparam(cookie, 2, &val);
        // MAGIC_PARAM_ELF_SHNUM_MAX is typically 3
        magic_setparam(cookie, 3, &val);
        // MAGIC_PARAM_ELF_PHNUM_MAX is typically 4
        magic_setparam(cookie, 4, &val);
        // MAGIC_PARAM_ELF_NOTES_MAX is typically 5
        magic_setparam(cookie, 5, &val);
        // MAGIC_PARAM_MAGWARN_MAX is typically 6
        magic_setparam(cookie, 6, &val);
        // MAGIC_PARAM_BYTES_MAX is typically 7
        magic_setparam(cookie, 7, &val);
        // MAGIC_PARAM_ENCODING_MAX is typically 8
        magic_setparam(cookie, 8, &val);
        // MAGIC_PARAM_ELF_SHSIZE_MAX is typically 9
        magic_setparam(cookie, 9, &val);
    }

    // Test magic_getparam
    if (Size > 2) {
        int param_id = (int)Data[2] & 0xFF;
        // Limit param_id to a reasonable range to avoid undefined behavior if the library checks bounds
        if (param_id < 20) {
            magic_getparam(cookie, param_id, &param_val);
        }
    }

    // Test magic_buffer
    if (Size > 0) {
        result = magic_buffer(cookie, Data, Size);
        if (result != NULL) {
            // Result is internal to libmagic, no need to free
        }
    }

    // Test magic_load_buffers
    if (Size > 0) {
        void *bufs[1];
        size_t lens[1];
        bufs[0] = (void *)Data;
        lens[0] = Size;
        magic_load_buffers(cookie, bufs, lens, 1);
    }

    // Test magic_compile with a dummy file
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
        magic_compile(cookie, dummy_file);
        unlink(dummy_file);
    }

    // Cleanup
    cleanup_magic(cookie);
    return 0;
}