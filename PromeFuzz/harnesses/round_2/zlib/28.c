// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
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

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Write data to dummy file for gzopen */
    write_dummy_file(Data, Size);

    /* Open file for reading */
    gzFile read_file = gzopen("./dummy_file", "rb");
    if (!read_file) return 0;

    /* Test gzbuffer on read file */
    unsigned buffer_size = (Data[0] % 65536) + 1; /* 1-65536 */
    gzbuffer(read_file, buffer_size);

    /* Test gzgetc and gzungetc */
    int c = gzgetc(read_file);
    if (c != -1) {
        gzungetc(c, read_file);
        /* Read it back */
        int c2 = gzgetc(read_file);
        (void)c2; /* Ignore result */
    }

    /* Test gzread with small buffer */
    char read_buf[256];
    unsigned read_len = (Data[0] % sizeof(read_buf)) + 1;
    gzread(read_file, read_buf, read_len);

    /* Close read file */
    gzclose(read_file);

    /* Open file for writing */
    gzFile write_file = gzopen("./dummy_file", "wb");
    if (!write_file) return 0;

    /* Test gzbuffer on write file */
    gzbuffer(write_file, buffer_size);

    /* Test gzwrite */
    char write_buf[256];
    unsigned write_len = Size < sizeof(write_buf) ? Size : sizeof(write_buf);
    memcpy(write_buf, Data, write_len);
    gzwrite(write_file, write_buf, write_len);

    /* Close write file */
    gzclose(write_file);

    /* Clean up dummy file */
    unlink("./dummy_file");

    return 0;
}