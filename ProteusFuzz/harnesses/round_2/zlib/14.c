// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with input data
    write_dummy_file(Data, Size);

    // Open the file for reading
    gzFile file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        return 0;
    }

    // Test gzdirect
    int direct = gzdirect(file);
    (void)direct;

    // Read some bytes using gzgetc
    int c1 = gzgetc(file);
    if (c1 != -1) {
        // Test gzungetc: push back the character
        int ret_ungetc = gzungetc(c1, file);
        (void)ret_ungetc;

        // Read again to verify push back
        int c2 = gzgetc(file);
        (void)c2;
    }

    // Test gzgetc_ (alias)
    int c3 = gzgetc_(file);
    (void)c3;

    // Test gzrewind
    int ret_rewind = gzrewind(file);
    if (ret_rewind == 0) {
        // Read again after rewind
        int c4 = gzgetc(file);
        (void)c4;
    }

    // Test gzclose_r
    int ret_close = gzclose_r(file);
    (void)ret_close;

    return 0;
}