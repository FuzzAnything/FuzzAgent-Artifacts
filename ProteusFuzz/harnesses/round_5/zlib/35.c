// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Create and write to the temporary file
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    ssize_t written = write(fd, Data, Size);
    close(fd);
    if (written < 0) {
        return 0;
    }

    // Open the file for reading using gzopen
    gzFile file = gzopen(filename, "rb");
    if (file == NULL) {
        unlink(filename);
        return 0;
    }

    // Allocate buffer for gzread
    // Ensure size fits in int to avoid immediate failure per API spec
    unsigned int read_len = (Size < (size_t)INT_MAX) ? (unsigned int)Size : (unsigned int)INT_MAX;
    unsigned char *buffer = (unsigned char *)malloc(read_len + 1);
    if (buffer == NULL) {
        gzclose_r(file);
        unlink(filename);
        return 0;
    }

    // Test gzread
    int read_result = gzread(file, buffer, read_len);
    (void)read_result;

    // Test gzeof
    int eof_result = gzeof(file);
    (void)eof_result;

    // Test gzgetc
    int c = gzgetc(file);
    
    // Test gzgetc_
    int c_ = gzgetc_(file);

    // Test gzungetc (push back a character if we read one)
    if (c != -1) {
        gzungetc(c, file);
    }
    if (c_ != -1) {
        gzungetc(c_, file);
    }

    // Cleanup
    free(buffer);
    gzclose_r(file);
    unlink(filename);

    return 0;
}