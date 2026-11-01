// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdarg.h>
#include <limits.h>

static int helper_gzvprintf(gzFile f, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = gzvprintf(f, fmt, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *filename = "./dummy_file";
    gzFile file = gzopen(filename, "wb");
    if (file == NULL) return 0;

    // Ensure the input data is treated as a string for gzputs by ensuring null termination
    char *str_buf = (char *)malloc(Size + 1);
    if (str_buf == NULL) {
        gzclose(file);
        return 0;
    }
    memcpy(str_buf, Data, Size);
    str_buf[Size] = '\0';

    // Fuzz gzputs
    // Limit the string length to avoid integer overflow issues mentioned in docs
    if (Size < (size_t)INT_MAX) {
        gzputs(file, str_buf);
    }

    // Fuzz gzputc
    if (Size > 0) {
        gzputc(file, Data[0] & 0xFF);
    }

    // Fuzz gzprintf with various formats derived from input
    if (Size > 0) {
        // Use a safe format string to avoid crashes from bad format specifiers in fuzzing
        if (Data[0] == 0) {
            gzprintf(file, "test");
        } else if (Data[0] == 1 && Size > 1) {
            gzprintf(file, "%d", (int)(Data[1] & 0xFF));
        } else if (Data[0] == 2 && Size > 2) {
            gzprintf(file, "%s", str_buf);
        } else {
            gzprintf(file, "fuzz");
        }
    }

    // Fuzz gzvprintf via helper
    if (Size > 0) {
        if (Data[0] == 0) {
            helper_gzvprintf(file, "test");
        } else if (Data[0] == 1 && Size > 1) {
            helper_gzvprintf(file, "%d", (int)(Data[1] & 0xFF));
        } else {
            helper_gzvprintf(file, "fuzz");
        }
    }

    // Fuzz gzseek
    // Use data to determine offset and whence
    z_off_t offset = 0;
    int whence = SEEK_SET;
    
    if (Size > 0) {
        offset = (z_off_t)(Data[0] & 0xFF);
    }
    if (Size > 1) {
        whence = Data[1] % 3; // SEEK_SET, SEEK_CUR, SEEK_END
    }
    
    // Seek operations might fail on write-only files or with invalid offsets, which is expected
    gzseek(file, offset, whence);

    // Fuzz gzerror
    int errnum = 0;
    const char *err_str = gzerror(file, &errnum);
    (void)err_str; // Suppress unused variable warning

    // Cleanup
    free(str_buf);
    gzclose(file);
    
    // Remove the dummy file
    remove(filename);

    return 0;
}