// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void cleanup_file(const char *filename) {
    if (filename && filename[0] != '\0') {
        unlink(filename);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    gzFile file = NULL;
    int ret;
    int fd;
    unsigned int buffer_size;
    int level;
    int strategy;
    int flush_mode;
    unsigned int write_len;

    // Cleanup previous runs
    cleanup_file(filename);

    // Create a dummy file with fuzzed data
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        }
        close(fd);
    }

    // Test 1: gzdirect on a file opened for reading
    file = gzopen(filename, "rb");
    if (file != NULL) {
        ret = gzdirect(file);
        (void)ret;
        gzclose(file);
        file = NULL;
    }

    // Test 2: gzdirect on a file opened for writing
    file = gzopen(filename, "wb");
    if (file != NULL) {
        ret = gzdirect(file);
        (void)ret;
        gzclose(file);
        file = NULL;
    }

    // Test 3: gzbuffer and gzsetparams and gzwrite and gzflush and gzclose_w
    // We need to open for writing to test these
    file = gzopen(filename, "wb");
    if (file != NULL) {
        // Derive parameters from input data to explore state space
        if (Size > 0) {
            buffer_size = (Data[0] % 100) + 8; // Min 8 bytes
            if (buffer_size > 1024 * 1024) buffer_size = 1024 * 1024; // Cap at 1MB
            ret = gzbuffer(file, buffer_size);
            (void)ret;
        }

        if (Size > 2) {
            level = (Data[1] % 10) - 1; // -1 to 8
            strategy = (Data[2] % 4);   // 0 to 3
            ret = gzsetparams(file, level, strategy);
            (void)ret;
        }

        // Write some data
        if (Size > 3) {
            write_len = (Data[3] % 100) + 1;
            if (write_len > Size - 4) write_len = Size - 4;
            ret = gzwrite(file, (voidpc)(Data + 4), write_len);
            (void)ret;
        }

        // Flush
        if (Size > 4) {
            flush_mode = (Data[4] % 6); // 0 to 5 (Z_FINISH is 4, but we try others)
            ret = gzflush(file, flush_mode);
            (void)ret;
        }

        // Close write
        ret = gzclose_w(file);
        (void)ret;
        file = NULL;
    }

    // Test 4: gzdirect on NULL
    ret = gzdirect(NULL);
    (void)ret;

    // Test 5: gzclose_w on NULL
    ret = gzclose_w(NULL);
    (void)ret;

    // Test 6: gzbuffer on NULL
    ret = gzbuffer(NULL, 1024);
    (void)ret;

    // Test 7: gzsetparams on NULL
    ret = gzsetparams(NULL, 6, Z_DEFAULT_STRATEGY);
    (void)ret;

    // Test 8: gzflush on NULL
    ret = gzflush(NULL, Z_NO_FLUSH);
    (void)ret;

    // Test 9: gzwrite on NULL
    ret = gzwrite(NULL, "test", 4);
    (void)ret;

    // Cleanup
    cleanup_file(filename);

    return 0;
}