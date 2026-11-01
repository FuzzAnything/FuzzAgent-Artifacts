// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void cleanup_gzfile(gzFile *file) {
    if (file != NULL && *file != NULL) {
        gzclose(*file);
        *file = NULL;
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a temporary file for testing
    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    close(fd);

    // Open file for writing
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) return 0;

    int result = 0;

    // Test gzbuffer
    if (Size > 0) {
        unsigned size = (Data[0] % 65536) + 8; // Ensure min 8 bytes
        result = gzbuffer(file, size);
        (void)result;
    }

    // Test gzputc
    if (Size > 0) {
        int c = Data[0];
        result = gzputc(file, c);
        (void)result;
    }

    // Test gzsetparams
    if (Size > 1) {
        int level = Data[1] % 10; // 0-9
        int strategy = (Data[1] >> 4) % 5; // 0-4
        result = gzsetparams(file, level, strategy);
        (void)result;
    }

    // Write some dummy data if available
    if (Size > 2) {
        size_t write_len = (Size - 2) % 1024;
        if (write_len > 0) {
            result = gzwrite(file, Data + 2, write_len);
            (void)result;
        }
    }

    // Test gzflush with various flush modes
    if (Size > 0) {
        int flush_mode = Data[0] % 6; // 0 to Z_FINISH (5)
        result = gzflush(file, flush_mode);
        (void)result;
    }

    // Test gzdirect (should return 0 for compressed, 1 for transparent)
    result = gzdirect(file);
    (void)result;

    // Test gzclose_w
    result = gzclose_w(file);
    (void)result;

    // Cleanup
    unlink(filename);

    return 0;
}