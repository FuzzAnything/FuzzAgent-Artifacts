// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    gzFile file = NULL;
    int ret;
    z_off64_t off64;
    z_off_t off;
    int c;

    if (Size == 0) {
        return 0;
    }

    // Write input to a dummy file to simulate a gzip file
    write_dummy_file(Data, Size);

    // Try opening in read mode
    file = gzopen("./dummy_file", "rb");
    if (file) {
        // Test gztell64
        off64 = gztell64(file);
        (void)off64;

        // Test gzoffset64
        off64 = gzoffset64(file);
        (void)off64;

        // Test gztell
        off = gztell(file);
        (void)off;

        // Test gzoffset
        off = gzoffset(file);
        (void)off;

        // Try reading a byte to enable gzungetc
        c = gzgetc(file);
        if (c != EOF) {
            // Test gzungetc
            ret = gzungetc(c, file);
            (void)ret;
        }

        // Try seeking
        // Use a small offset derived from input to vary state
        z_off_t seek_offset = (z_off_t)(Data[0] % 100);
        off = gzseek(file, seek_offset, SEEK_SET);
        (void)off;

        // Seek from current position
        off = gzseek(file, seek_offset, SEEK_CUR);
        (void)off;

        gzclose(file);
    }

    // Try opening in write mode to test behavior on write-only files
    file = gzopen("./dummy_file", "wb");
    if (file) {
        // Write some data to make it a valid gzip stream
        const char *test_str = "fuzzing zlib";
        gzwrite(file, test_str, strlen(test_str));

        // Test gztell64 on write mode
        off64 = gztell64(file);
        (void)off64;

        // Test gzoffset64 on write mode
        off64 = gzoffset64(file);
        (void)off64;

        // Test gztell on write mode
        off = gztell(file);
        (void)off;

        // Test gzoffset on write mode
        off = gzoffset(file);
        (void)off;

        // gzseek on write mode may fail, test it
        z_off_t seek_offset = (z_off_t)(Data[0] % 100);
        off = gzseek(file, seek_offset, SEEK_SET);
        (void)off;

        gzclose(file);
    }

    // Test with NULL file handle
    off64 = gztell64(NULL);
    (void)off64;
    off64 = gzoffset64(NULL);
    (void)off64;
    off = gztell(NULL);
    (void)off;
    off = gzoffset(NULL);
    (void)off;
    off = gzseek(NULL, 0, SEEK_SET);
    (void)off;
    ret = gzungetc('a', NULL);
    (void)ret;

    return 0;
}