// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
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
#include <sys/stat.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    if (size > 0) {
        ssize_t written = write(fd, data, size);
        if (written != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment: create a dummy file with fuzz input
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    gzFile file = NULL;
    uint8_t read_buf[1024];
    uint8_t write_buf[1024];
    z_size_t nitems, size;

    // Test 1: gzopen64 for reading
    file = gzopen64("./dummy_file", "rb");
    if (file != NULL) {
        nitems = (Size > 100) ? 100 : Size;
        size = (Size > 100) ? 1 : Size;
        gzfread(read_buf, size, nitems, file);
        gzclearerr(file);
        gzfread(read_buf, size, nitems, file);
        gzclose(file);
        file = NULL;
    }

    // Test 2: gzopen for reading
    file = gzopen("./dummy_file", "rb");
    if (file != NULL) {
        nitems = (Size > 100) ? 100 : Size;
        size = (Size > 100) ? 1 : Size;
        gzfread(read_buf, size, nitems, file);
        gzclearerr(file);
        gzclose(file);
        file = NULL;
    }

    // Test 3: gzopen64 for writing
    file = gzopen64("./dummy_file", "wb");
    if (file != NULL) {
        nitems = (Size > 100) ? 100 : Size;
        size = (Size > 100) ? 1 : Size;
        memcpy(write_buf, Data, Size);
        gzfwrite(write_buf, size, nitems, file);
        gzclearerr(file);
        gzclose(file);
        file = NULL;
    }

    // Test 4: gzopen for writing
    file = gzopen("./dummy_file", "wb");
    if (file != NULL) {
        nitems = (Size > 100) ? 100 : Size;
        size = (Size > 100) ? 1 : Size;
        memcpy(write_buf, Data, Size);
        gzfwrite(write_buf, size, nitems, file);
        gzclearerr(file);
        gzclose(file);
        file = NULL;
    }

    // Test 5: gzdopen
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        file = gzdopen(fd, "rb");
        if (file != NULL) {
            nitems = (Size > 100) ? 100 : Size;
            size = (Size > 100) ? 1 : Size;
            gzfread(read_buf, size, nitems, file);
            gzclearerr(file);
            gzclose(file);
            file = NULL;
        }
        close(fd);
    }

    // Test 6: gzdopen for writing
    fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        file = gzdopen(fd, "wb");
        if (file != NULL) {
            nitems = (Size > 100) ? 100 : Size;
            size = (Size > 100) ? 1 : Size;
            memcpy(write_buf, Data, Size);
            gzfwrite(write_buf, size, nitems, file);
            gzclearerr(file);
            gzclose(file);
            file = NULL;
        }
        close(fd);
    }

    // Cleanup
    unlink("./dummy_file");

    return 0;
}