// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure we have enough data to create a valid gzip header and some content
    // We will write the input data to a file and try to open it as gzip.
    // Even if the data is not valid gzip, the library should handle it gracefully.
    
    // Write input to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    gzFile file = NULL;
    z_off64_t offset64;
    z_off_t offset32;
    z_off64_t tell64;
    z_off_t tell32;
    int ret;
    int seek_ret;
    z_off_t seek_ret32;

    // Test 1: Open for reading
    file = gzopen("./dummy_file", "rb");
    if (file != NULL) {
        // Test gztell64
        tell64 = gztell64(file);
        (void)tell64;

        // Test gztell
        tell32 = gztell(file);
        (void)tell32;

        // Test gzoffset64
        offset64 = gzoffset64(file);
        (void)offset64;

        // Test gzoffset
        offset32 = gzoffset(file);
        (void)offset32;

        // Test gzungetc with various characters from input
        if (Size > 0) {
            int c = Data[0] & 0xFF;
            ret = gzungetc(c, file);
            (void)ret;
        }
        
        // Test gzseek
        if (Size > 1) {
            z_off_t offset_val = (z_off_t)(Data[1]);
            seek_ret32 = gzseek(file, offset_val, SEEK_SET);
            (void)seek_ret32;
            
            seek_ret32 = gzseek(file, offset_val, SEEK_CUR);
            (void)seek_ret32;

            seek_ret32 = gzseek(file, offset_val, SEEK_END);
            (void)seek_ret32;
        }

        gzclose(file);
        file = NULL;
    }

    // Test 2: Open for writing
    file = gzopen("./dummy_file", "wb");
    if (file != NULL) {
        // Write some data to ensure state changes
        if (Size > 0) {
            gzwrite(file, Data, Size);
        }

        // Test gztell64
        tell64 = gztell64(file);
        (void)tell64;

        // Test gztell
        tell32 = gztell(file);
        (void)tell32;

        // Test gzoffset64
        offset64 = gzoffset64(file);
        (void)offset64;

        // Test gzoffset
        offset32 = gzoffset(file);
        (void)offset32;

        // gzungetc should fail on write mode, but we test it anyway
        if (Size > 0) {
            int c = Data[0] & 0xFF;
            ret = gzungetc(c, file);
            (void)ret;
        }

        // gzseek on write mode might fail or behave differently, test it
        if (Size > 1) {
            z_off_t offset_val = (z_off_t)(Data[1]);
            seek_ret32 = gzseek(file, offset_val, SEEK_SET);
            (void)seek_ret32;
        }

        gzclose(file);
        file = NULL;
    }

    // Test 3: Open with append mode if supported
    file = gzopen("./dummy_file", "ab");
    if (file != NULL) {
        // Write some data
        if (Size > 0) {
            gzwrite(file, Data, Size);
        }

        tell64 = gztell64(file);
        (void)tell64;
        tell32 = gztell(file);
        (void)tell32;
        offset64 = gzoffset64(file);
        (void)offset64;
        offset32 = gzoffset(file);
        (void)offset32;

        gzclose(file);
        file = NULL;
    }

    // Cleanup
    unlink("./dummy_file");

    return 0;
}