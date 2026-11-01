// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzerror at gzlib.c:513:22 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <zlib.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare data for writing to a temporary file
    write_dummy_file(Data, Size);

    gzFile file = NULL;
    int fd = -1;
    int errnum;
    uint8_t buffer[4096];
    z_size_t items_read, items_written;
    const char *err_str;

    // Test 1: gzopen64 (read mode)
    file = gzopen64("./dummy_file", "rb");
    if (file != NULL) {
        // Test gzfread
        items_read = gzfread(buffer, 1, Size, file);
        (void)items_read;

        // Test gzerror
        err_str = gzerror(file, &errnum);
        (void)err_str;

        // Test gzclearerr
        gzclearerr(file);

        gzclose(file);
        file = NULL;
    }

    // Test 2: gzopen64 (write mode)
    file = gzopen64("./dummy_file", "wb");
    if (file != NULL) {
        // Test gzfwrite
        items_written = gzfwrite(Data, 1, Size, file);
        (void)items_written;

        // Test gzerror
        err_str = gzerror(file, &errnum);
        (void)err_str;

        // Test gzclearerr
        gzclearerr(file);

        gzclose(file);
        file = NULL;
    }

    // Test 3: gzdopen
    fd = open("./dummy_file", O_RDONLY);
    if (fd != -1) {
        file = gzdopen(fd, "rb");
        if (file != NULL) {
            items_read = gzfread(buffer, 1, Size, file);
            (void)items_read;
            err_str = gzerror(file, &errnum);
            (void)err_str;
            gzclearerr(file);
            gzclose(file);
        }
        close(fd);
    }

    fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        file = gzdopen(fd, "wb");
        if (file != NULL) {
            items_written = gzfwrite(Data, 1, Size, file);
            (void)items_written;
            err_str = gzerror(file, &errnum);
            (void)err_str;
            gzclearerr(file);
            gzclose(file);
        }
        close(fd);
    }

    // Test 4: Edge cases with NULL or invalid file
    gzclearerr(NULL);
    err_str = gzerror(NULL, &errnum);
    (void)err_str;
    items_read = gzfread(buffer, 1, Size, NULL);
    (void)items_read;
    items_written = gzfwrite(Data, 1, Size, NULL);
    (void)items_written;

    return 0;
}