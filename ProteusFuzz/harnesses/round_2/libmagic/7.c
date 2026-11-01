// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    if (size > 0) {
        if (write(fd, data, size) != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

static void cleanup_magic(magic_t m) {
    if (m) {
        magic_close(m);
    }
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    magic_t m = NULL;
    int ret;
    size_t param_val;
    void *bufs[2];
    size_t sizes[2];

    // 1. Prepare environment: Open magic handle
    // Try different flags to explore states
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0x0F; // Limit to lower 4 bits for safety
    }
    m = magic_open(flags);
    if (!m) {
        return 0;
    }

    // 2. Set flags (magic_setflags)
    int set_flags = 0;
    if (Size > 1) {
        set_flags = Data[1] & 0xFF;
    }
    magic_setflags(m, set_flags);

    // 3. Set parameters (magic_setparam)
    // Try setting various parameters if data is available
    if (Size > 2) {
        int param_type = Data[2] & 0x0F; // Limit range
        size_t val = (Size > 3) ? Data[3] : 0;
        magic_setparam(m, param_type, &val);
    }

    // 4. Get parameters (magic_getparam)
    if (Size > 4) {
        int get_param_type = Data[4] & 0x0F;
        magic_getparam(m, get_param_type, &param_val);
    }

    // 5. Compile magic file (magic_compile)
    // Create a dummy file with input data to test compilation
    if (write_dummy_file(Data, Size) == 0) {
        magic_compile(m, "./dummy_file");
    }

    // 6. Load buffers (magic_load_buffers)
    // Prepare buffers from input data
    if (Size > 0) {
        // Split data into two buffers if possible
        size_t split = Size / 2;
        if (split == 0) split = 1;
        if (split > Size) split = Size;

        // Allocate memory for buffers to simulate file content
        void *buf1 = malloc(split);
        void *buf2 = malloc(Size - split);

        if (buf1 && buf2) {
            memcpy(buf1, Data, split);
            memcpy(buf2, Data + split, Size - split);

            bufs[0] = buf1;
            bufs[1] = buf2;
            sizes[0] = split;
            sizes[1] = Size - split;

            magic_load_buffers(m, bufs, sizes, 2);

            free(buf1);
            free(buf2);
        } else if (buf1) {
            // Fallback if second allocation fails
            free(buf1);
        }
    }

    // 7. Cleanup
    cleanup_magic(m);

    return 0;
}