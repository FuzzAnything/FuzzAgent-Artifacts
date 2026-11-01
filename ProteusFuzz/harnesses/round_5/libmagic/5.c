// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    int ret;
    size_t param_val;
    const char *result;

    if (Size == 0) {
        return 0;
    }

    // 1. magic_open
    // Try different flags based on input data to explore states
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0x0F; // Limit to lower 4 bits for valid flag combinations
    }
    cookie = magic_open(flags);
    if (cookie == NULL) {
        return 0;
    }

    // 2. magic_setparam
    // Try setting various parameters if we have enough data
    if (Size >= 3) {
        int param_type = Data[1] % 10; // Arbitrary selection of parameter type
        size_t val = (size_t)Data[2] * 100 + 1; // Ensure non-zero
        magic_setparam(cookie, param_type, &val);
    }

    // 3. magic_load
    // Attempt to load a database. We use a dummy path or a system path if available.
    // Since we don't have a real magic file, this might fail, which is fine for fuzzing.
    // We try loading from a dummy file path constructed from input or a standard path.
    if (Size > 3) {
        char path[256];
        size_t path_len = Size > 250 ? 250 : Size;
        // Ensure we don't read past the end of Data
        if (Size >= 3 + path_len) {
            memcpy(path, Data + 3, path_len);
            path[path_len] = '\0';
            magic_load(cookie, path);
        }
    }
    // Also try loading a standard system path if available (often /usr/share/misc/magic)
    // This helps test the code path when a valid file exists.
    magic_load(cookie, "/usr/share/misc/magic");

    // 4. magic_compile
    // Similar to load, try compiling a file
    if (Size > 3) {
        char path[256];
        size_t path_len = Size > 250 ? 250 : Size;
        // Ensure we don't read past the end of Data
        if (Size >= 3 + path_len) {
            memcpy(path, Data + 3, path_len);
            path[path_len] = '\0';
            magic_compile(cookie, path);
        }
    }
    magic_compile(cookie, "/usr/share/misc/magic");

    // 5. magic_file
    // Write input data to a dummy file and try to identify it
    write_dummy_file(Data, Size);
    result = magic_file(cookie, "./dummy_file");
    (void)result; // Suppress unused warning, result is just for coverage

    // 6. magic_getparam
    // Retrieve parameters to exercise the getter logic
    if (Size >= 2) {
        int param_type = Data[1] % 10;
        magic_getparam(cookie, param_type, &param_val);
    }

    // Cleanup
    magic_close(cookie);

    return 0;
}