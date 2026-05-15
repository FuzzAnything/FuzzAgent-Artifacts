// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    write_dummy_file(Data, Size);

    // Test gzwrite
    gzFile write_file = gzopen("./dummy_file", "wb");
    if (write_file) {
        unsigned write_len = (unsigned)(Size % 65536);
        if (write_len > 0) {
            gzbuffer(write_file, (unsigned)(Data[0] % 8192 + 1));
            gzwrite(write_file, (voidpc)Data, write_len);
        }
        gzclose(write_file);
    }

    // Test gzread and related functions
    gzFile read_file = gzopen("./dummy_file", "rb");
    if (read_file) {
        unsigned buf_size = (unsigned)(Data[1] % 8192 + 1);
        gzbuffer(read_file, buf_size);

        unsigned char *read_buf = (unsigned char *)malloc(buf_size);
        if (read_buf) {
            // Test gzread
            unsigned read_len = (unsigned)(Size % buf_size);
            if (read_len > 0) {
                gzread(read_file, read_buf, read_len);
            }

            // Test gzgetc and gzungetc
            if (Size > 10) {
                int c = gzgetc(read_file);
                if (c != -1) {
                    gzungetc(c, read_file);
                    gzgetc(read_file); // Read again
                }
            }

            free(read_buf);
        }
        gzclose(read_file);
    }

    cleanup_dummy_file();
    return 0;
}