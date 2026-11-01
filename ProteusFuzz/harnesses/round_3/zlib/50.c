// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdarg.h>

// Wrapper function to allow calling gzvprintf with a constructed va_list
static int test_gzvprintf_wrapper(gzFile file, const char *format, ...) {
    va_list va;
    va_start(va, format);
    int ret = gzvprintf(file, format, va);
    va_end(va);
    return ret;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *filename = "./dummy_file";
    gzFile file = NULL;
    int errnum;
    int ret;

    // Initialize a file for writing
    file = gzopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }

    // Test gzputc
    if (Size > 0) {
        ret = gzputc(file, Data[0] & 0xFF);
        if (ret == -1) {
            gzerror(file, &errnum);
        }
    }

    // Test gzputs with a null-terminated string derived from input
    if (Size > 1) {
        char *str = (char *)malloc(Size + 1);
        if (str != NULL) {
            memcpy(str, Data, Size);
            str[Size] = '\0';
            ret = gzputs(file, str);
            if (ret == -1) {
                gzerror(file, &errnum);
            }
            free(str);
        }
    }

    // Test gzprintf with format strings derived from input
    if (Size > 2) {
        char fmt[64];
        size_t fmt_len = (Size > 63) ? 63 : Size;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        ret = gzprintf(file, fmt, (int)(Data[0] + Data[1]));
        if (ret == -1) {
            gzerror(file, &errnum);
        }
    }

    // Test gzvprintf via the wrapper
    if (Size > 3) {
        char fmt[64];
        size_t fmt_len = (Size > 63) ? 63 : Size;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        
        ret = test_gzvprintf_wrapper(file, fmt, (int)Data[0], (int)Data[1], (int)Data[2]);
        if (ret == -1) {
            gzerror(file, &errnum);
        }
    }

    // Test gzseek
    // Ensure we have at least 2 bytes before accessing Data[1]
    if (Size > 1) {
        z_off_t offset = (z_off_t)(Data[0] * 1000);
        int whence = Data[1] % 3; // SEEK_SET, SEEK_CUR, SEEK_END
        ret = gzseek(file, offset, whence);
        if (ret == -1) {
            gzerror(file, &errnum);
        }
    }

    // Test gzerror on a valid file
    const char *err_msg = gzerror(file, &errnum);
    (void)err_msg; // Suppress unused warning

    // Cleanup
    gzclose(file);

    // Remove the dummy file
    remove(filename);

    return 0;
}