// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <unistd.h>

static void setup_fuzz_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f != NULL) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a temporary file for writing */
    setup_fuzz_file(Data, Size);

    /* Open the file for writing in gzip mode */
    gzFile file = gzopen("./dummy_file", "wb");
    if (file == NULL) {
        return 0;
    }

    /* Test gzputc with various characters derived from input */
    int c = Data[0] & 0xFF;
    int ret = gzputc(file, c);
    (void)ret;

    /* Test gzputs with a null-terminated string if possible */
    if (Size > 1) {
        char str[256];
        /* Ensure we don't read past the input buffer or write past str buffer */
        size_t max_len = (Size - 1 < 255) ? (Size - 1) : 255;
        memcpy(str, Data + 1, max_len);
        str[max_len] = '\0';
        ret = gzputs(file, str);
        (void)ret;
    }

    /* Test gzsetparams with various levels and strategies */
    int level = 0;
    int strategy = 0;
    if (Size > 0) {
        level = (Data[0] % 10); /* 0-9 */
    }
    if (Size > 1) {
        strategy = (Data[1] % 4); /* 0-3 */
    }
    ret = gzsetparams(file, level, strategy);
    (void)ret;

    /* Test gzflush with various flush modes */
    int flush = 0;
    if (Size > 2) {
        flush = (Data[2] % 6); /* 0 to Z_FINISH (5) */
    }
    ret = gzflush(file, flush);
    (void)ret;

    /* Test gzdirect (though it's more meaningful for read mode, we test it anyway) */
    int is_direct = gzdirect(file);
    (void)is_direct;

    /* Test gzclose_w to safely close the file */
    ret = gzclose_w(file);
    (void)ret;

    /* Cleanup: remove the dummy file */
    unlink("./dummy_file");

    return 0;
}