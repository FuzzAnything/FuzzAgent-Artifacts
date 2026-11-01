// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return;
    if (size > 0) {
        write(fd, data, size);
    }
    close(fd);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    const char *mode_read = "rb";
    const char *mode_write = "wb";

    // Write input data to dummy file for file-based operations
    write_dummy_file(filename, Data, Size);

    gzFile file = NULL;
    int fd = -1;
    int ret;
    z_size_t nread, nwritten;
    uint8_t *buffer = NULL;
    size_t buf_size;

    // Allocate buffer for read/write operations
    if (Size > 0) {
        buf_size = (Size > 1024) ? 1024 : Size;
        buffer = (uint8_t *)malloc(buf_size);
        if (!buffer) return 0;
    } else {
        buf_size = 1;
        buffer = (uint8_t *)malloc(1);
        if (!buffer) return 0;
    }

    // Test Case 1: gzopen and gzfread
    file = gzopen(filename, mode_read);
    if (file) {
        nread = gzfread(buffer, 1, buf_size, file);
        gzclearerr(file);
        gzclose(file);
        file = NULL;
    }

    // Test Case 2: gzopen64 and gzfread
    file = gzopen64(filename, mode_read);
    if (file) {
        nread = gzfread(buffer, 1, buf_size, file);
        gzclearerr(file);
        gzclose(file);
        file = NULL;
    }

    // Test Case 3: gzdopen and gzfread
    fd = open(filename, O_RDONLY);
    if (fd >= 0) {
        file = gzdopen(fd, mode_read);
        if (file) {
            nread = gzfread(buffer, 1, buf_size, file);
            gzclearerr(file);
            gzclose(file);
            file = NULL;
        }
        close(fd);
        fd = -1;
    }

    // Test Case 4: gzopen and gzfwrite
    file = gzopen(filename, mode_write);
    if (file) {
        nwritten = gzfwrite(Data, 1, Size, file);
        gzclearerr(file);
        gzclose(file);
        file = NULL;
    }

    // Test Case 5: gzopen64 and gzfwrite
    file = gzopen64(filename, mode_write);
    if (file) {
        nwritten = gzfwrite(Data, 1, Size, file);
        gzclearerr(file);
        gzclose(file);
        file = NULL;
    }

    // Test Case 6: gzdopen and gzfwrite
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        file = gzdopen(fd, mode_write);
        if (file) {
            nwritten = gzfwrite(Data, 1, Size, file);
            gzclearerr(file);
            gzclose(file);
            file = NULL;
        }
        close(fd);
        fd = -1;
    }

    // Cleanup
    free(buffer);
    if (fd >= 0) close(fd);
    if (file) gzclose(file);

    return 0;
}