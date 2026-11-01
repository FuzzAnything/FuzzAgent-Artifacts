// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static void cleanup_gzfile(gzFile file) {
    if (file != NULL) {
        gzclose(file);
    }
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    
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

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test gzdopen with a file descriptor for reading
    if (write_dummy_file(Data, Size) == 0) {
        int fd = open("./dummy_file", O_RDONLY);
        if (fd >= 0) {
            gzFile file = gzdopen(fd, "rb");
            if (file != NULL) {
                // Test gzdirect
                gzdirect(file);
                
                // Test gzgetc
                int c1 = gzgetc(file);
                if (c1 != -1) {
                    // Test gzungetc
                    gzungetc(c1, file);
                    // Read again to verify
                    gzgetc(file);
                }
                
                // Test gzgetc_
                int c2 = gzgetc_(file);
                (void)c2;
                
                cleanup_gzfile(file);
            }
            close(fd);
        }
    }

    // Test gzdopen with write mode
    if (write_dummy_file(Data, Size) == 0) {
        int fd = open("./dummy_file", O_RDWR);
        if (fd >= 0) {
            gzFile file = gzdopen(fd, "wb");
            if (file != NULL) {
                // Write some data to ensure buffer is not empty
                gzputc(file, Data[0]);
                
                // Test gzflush with various modes
                gzflush(file, Z_NO_FLUSH);
                gzflush(file, Z_PARTIAL_FLUSH);
                gzflush(file, Z_SYNC_FLUSH);
                gzflush(file, Z_FULL_FLUSH);
                gzflush(file, Z_FINISH);
                
                cleanup_gzfile(file);
            }
            close(fd);
        }
    }

    // Test with invalid file descriptor
    gzFile invalid_file = gzdopen(-1, "rb");
    if (invalid_file != NULL) {
        gzdirect(invalid_file);
        gzgetc(invalid_file);
        gzgetc_(invalid_file);
        gzungetc('a', invalid_file);
        gzflush(invalid_file, Z_NO_FLUSH);
        cleanup_gzfile(invalid_file);
    }

    // Test with NULL file using the function versions to avoid macro expansion issues
    gzdirect(NULL);
    gzgetc_(NULL);
    gzungetc('a', NULL);
    gzflush(NULL, Z_NO_FLUSH);

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}