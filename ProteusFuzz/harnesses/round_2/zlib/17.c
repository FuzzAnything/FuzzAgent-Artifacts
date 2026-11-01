// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with the input data
    write_dummy_file(Data, Size);

    // Test gzopen for reading
    gzFile file_r = gzopen("./dummy_file", "rb");
    if (file_r != NULL) {
        unsigned char buf[4096];
        int c;
        int ret;
        z_off_t offset;
        int whence;

        // Fuzz gzbuffer (must be called before any I/O)
        if (Size > 0) {
            unsigned size = Data[0] + 1; // Ensure at least 1
            gzbuffer(file_r, size);
        }

        // Fuzz gzgetc
        for (int i = 0; i < 10 && Size > 0; i++) {
            c = gzgetc(file_r);
            if (c == -1) break;
        }

        // Fuzz gzungetc
        if (Size > 0) {
            int push_char = Data[0];
            gzungetc(push_char, file_r);
            // Try multiple pushes
            if (Size > 1) {
                gzungetc(Data[1], file_r);
            }
        }

        // Fuzz gzgets
        if (Size > 0) {
            char linebuf[256];
            int len = (Data[0] % 256) + 1;
            gzgets(file_r, linebuf, len);
        }

        // Fuzz gzread
        if (Size > 0) {
            unsigned len = (Data[0] % 1024) + 1;
            ret = gzread(file_r, buf, len);
            (void)ret;
        }

        // Fuzz gzseek - Ensure we have enough data before accessing indices
        if (Size >= 4) {
            offset = (z_off_t)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
            // Cycle through SEEK_SET, SEEK_CUR, SEEK_END
            for (int w = 0; w < 3; w++) {
                whence = w; // 0, 1, 2
                ret = gzseek(file_r, offset, whence);
                (void)ret;
            }
        }

        gzclose(file_r);
    }

    // Test gzopen for writing to ensure write-only mode handling for gzseek/gzungetc
    gzFile file_w = gzopen("./dummy_file", "wb");
    if (file_w != NULL) {
        unsigned char buf[4096];
        int c;
        int ret;
        z_off_t offset;
        int whence;

        // Write some data to make file non-empty for seek tests if needed
        if (Size > 0) {
            unsigned len = (Data[0] % 1024) + 1;
            memset(buf, Data[0], len);
            gzwrite(file_w, buf, len);
        }

        // Fuzz gzseek on write mode (should fail) - Ensure we have enough data
        if (Size >= 4) {
            offset = (z_off_t)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
            for (int w = 0; w < 3; w++) {
                whence = w;
                ret = gzseek(file_w, offset, whence);
                (void)ret;
            }
        }

        // Fuzz gzungetc on write mode (should fail)
        if (Size > 0) {
            int push_char = Data[0];
            c = gzungetc(push_char, file_w);
            (void)c;
        }

        gzclose(file_w);
    }

    return 0;
}