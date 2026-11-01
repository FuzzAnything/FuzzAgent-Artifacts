// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
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

    // Write input data to a dummy file to simulate a gzip file
    write_dummy_file(Data, Size);

    // Open the file for reading
    gzFile file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        return 0;
    }

    // Test gzeof
    int eof_check = gzeof(file);
    (void)eof_check;

    // Test gzgetc
    int c = gzgetc(file);
    if (c != -1) {
        // Test gzungetc if a character was read
        gzungetc(c, file);
    }

    // Test gzgetc_ (alias for gzgetc)
    int c_ = gzgetc_(file);
    (void)c_;

    // Test gzrewind
    int rewind_res = gzrewind(file);
    (void)rewind_res;

    // Read a few more bytes to exercise state changes
    for (int i = 0; i < 5 && i < (int)Size; i++) {
        gzgetc(file);
    }

    // Check EOF again after reading
    eof_check = gzeof(file);
    (void)eof_check;

    // Test gzclose_r
    int close_res = gzclose_r(file);
    (void)close_res;

    return 0;
}