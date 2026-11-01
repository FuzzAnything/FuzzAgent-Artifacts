// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
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
#include <sys/types.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    magic_t cookie = NULL;
    int ret = 0;
    size_t param_val = 0;
    void *buffers[1];
    size_t sizes[1];

    // 1. Prepare environment: Open magic handle
    // Try different flags based on input to explore states
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0xFF;
    }
    cookie = magic_open(flags);
    if (cookie == NULL) {
        return 0;
    }

    // 2. Set parameters and flags to explore internal states
    // Use input data to fuzz parameter values
    if (Size > 1) {
        param_val = Data[1];
        ret = magic_setparam(cookie, MAGIC_PARAM_BYTES_MAX, &param_val);
        (void)ret;
    }
    if (Size > 2) {
        param_val = Data[2];
        ret = magic_setparam(cookie, MAGIC_PARAM_REGEX_MAX, &param_val);
        (void)ret;
    }

    // Fuzz flags
    if (Size > 3) {
        int fuzz_flags = Data[3] & 0xFF;
        ret = magic_setflags(cookie, fuzz_flags);
        (void)ret;
    }

    // 3. Load buffers directly (in-memory analysis)
    // Prepare buffer array
    buffers[0] = (void *)Data;
    sizes[0] = Size;
    
    ret = magic_load_buffers(cookie, buffers, sizes, 1);
    if (ret != 0) {
        // If buffer loading fails, try file-based loading as fallback
        // Write data to dummy file
        if (write_dummy_file(Data, Size) == 0) {
            ret = magic_compile(cookie, "./dummy_file");
            (void)ret;
        }
    }

    // 4. Get parameters to ensure state retrieval works
    size_t retrieved_val = 0;
    ret = magic_getparam(cookie, MAGIC_PARAM_BYTES_MAX, &retrieved_val);
    (void)ret;
    ret = magic_getparam(cookie, MAGIC_PARAM_REGEX_MAX, &retrieved_val);
    (void)ret;

    // Cleanup
    magic_close(cookie);
    
    // Cleanup dummy file if it exists
    remove("./dummy_file");

    return 0;
}