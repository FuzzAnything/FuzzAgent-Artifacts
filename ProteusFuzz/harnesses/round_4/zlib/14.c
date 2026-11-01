// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <fcntl.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    gzFile file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        return 0;
    }

    int c;
    int ret;

    // Test gzgetc and gzgetc_
    c = gzgetc(file);
    if (c != -1) {
        // Test gzungetc if we read something
        ret = gzungetc(c, file);
    }

    // Test gzgetc_ alias
    c = gzgetc_(file);

    // Test gzeof
    ret = gzeof(file);

    // Test gzrewind
    ret = gzrewind(file);

    // Read a bit more to change state
    while ((c = gzgetc(file)) != -1) {
        if (c == 'A') {
            ret = gzungetc(c, file);
        }
    }

    // Test gzeof again
    ret = gzeof(file);

    // Try gzrewind again
    ret = gzrewind(file);

    // Try gzungetc with EOF character (0xFF)
    ret = gzungetc(0xFF, file);

    // Close the file
    ret = gzclose_r(file);

    // Test with NULL file using the function versions directly to avoid macro expansion issues
    // gzclose_r handles NULL safely
    ret = gzclose_r(NULL);
    
    // gzrewind handles NULL safely
    ret = gzrewind(NULL);
    
    // gzeof handles NULL safely
    ret = gzeof(NULL);
    
    // gzungetc handles NULL safely
    ret = gzungetc(0, NULL);
    
    // For gzgetc, we must use the function version gzgetc_ to avoid the macro expanding on NULL
    // The macro gzgetc tries to access struct members which fails on NULL
    c = gzgetc_(NULL);

    return 0;
}