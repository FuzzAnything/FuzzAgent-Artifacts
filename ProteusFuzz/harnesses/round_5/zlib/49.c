// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzseek at gzlib.c:438:17 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdarg.h>

static void cleanup_gzfile(gzFile file) {
    if (file != NULL) {
        gzclose(file);
    }
}

static int test_gzvprintf(gzFile file, const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    int ret = gzvprintf(file, fmt, va);
    va_end(va);
    return ret;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    gzFile file = NULL;
    int errnum;
    int ret;

    // Open file in write mode for testing gzputc, gzputs, gzprintf, gzvprintf
    file = gzopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }

    // Test gzputc
    for (size_t i = 0; i < Size; i++) {
        ret = gzputc(file, Data[i]);
        if (ret == -1) {
            const char *err = gzerror(file, &errnum);
            (void)err; // Suppress unused warning
            break;
        }
    }

    // Test gzputs with a null-terminated string derived from input
    if (Size > 1) {
        char str[256];
        size_t len = Size < 255 ? Size : 255;
        memcpy(str, Data, len);
        str[len] = '\0';
        ret = gzputs(file, str);
        if (ret == -1) {
            const char *err = gzerror(file, &errnum);
            (void)err;
        }
    }

    // Test gzprintf
    if (Size >= 2) {
        char fmt[32];
        size_t fmt_len = Size < 31 ? Size : 31;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        // Ensure format string is safe by limiting to simple patterns
        ret = gzprintf(file, "%s", fmt);
        if (ret == -1) {
            const char *err = gzerror(file, &errnum);
            (void)err;
        }
    }

    // Test gzvprintf via helper function
    if (Size >= 2) {
        char fmt[32];
        size_t fmt_len = Size < 31 ? Size : 31;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        ret = test_gzvprintf(file, "%s", fmt);
        if (ret == -1) {
            const char *err = gzerror(file, &errnum);
            (void)err;
        }
    }

    // Flush and close write mode
    gzclose(file);
    file = NULL;

    // Reopen in read mode to test gzseek and gzerror
    file = gzopen(filename, "rb");
    if (file != NULL) {
        // Test gzseek
        z_off_t offset = (Size % 1000) - 500; // Random offset
        int whence = SEEK_SET;
        if (Size > 1) {
            whence = Data[0] % 3; // SEEK_SET, SEEK_CUR, SEEK_END
        }
        ret = gzseek(file, offset, whence);
        if (ret == -1) {
            const char *err = gzerror(file, &errnum);
            (void)err;
        }

        // Test gzerror on valid file
        const char *err = gzerror(file, &errnum);
        (void)err;

        // Test gzerror on invalid file (after closing)
        gzclose(file);
        file = NULL;
        err = gzerror(file, &errnum);
        (void)err;
    }

    return 0;
}