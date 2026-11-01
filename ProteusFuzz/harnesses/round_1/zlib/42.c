// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

static void test_gzvprintf(gzFile file, const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    gzvprintf(file, fmt, va);
    va_end(va);
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *temp_file = "./dummy_file";
    FILE *fp = fopen(temp_file, "wb");
    if (!fp) {
        return 0;
    }
    
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    int fd = open(temp_file, O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    gzFile file = gzdopen(fd, "rb");
    if (!file) {
        close(fd);
        return 0;
    }

    // Test gzputc
    if (Size > 0) {
        gzputc(file, Data[0] & 0xFF);
    }

    // Test gzputs
    if (Size > 1) {
        char *str = (char *)malloc(Size);
        if (str) {
            memcpy(str, Data + 1, Size - 1);
            str[Size - 1] = '\0';
            gzputs(file, str);
            free(str);
        }
    }

    // Test gzprintf
    if (Size > 2) {
        char *fmt = (char *)malloc(Size - 1);
        if (fmt) {
            memcpy(fmt, Data + 2, Size - 2);
            fmt[Size - 2] = '\0';
            if (strlen(fmt) > 100) {
                fmt[100] = '\0';
            }
            int val1 = (Data[0] << 8) | Data[1];
            int val2 = (Size > 3) ? ((Data[2] << 8) | Data[3]) : 0;
            gzprintf(file, fmt, val1, val2);
            free(fmt);
        }
    }

    // Test gzvprintf via helper function
    if (Size > 3) {
        char *fmt = (char *)malloc(Size - 2);
        if (fmt) {
            memcpy(fmt, Data + 3, Size - 3);
            fmt[Size - 3] = '\0';
            if (strlen(fmt) > 100) {
                fmt[100] = '\0';
            }
            int val1 = (Data[0] << 8) | Data[1];
            int val2 = (Data[2] << 8) | Data[3];
            test_gzvprintf(file, fmt, val1, val2);
            free(fmt);
        }
    }

    gzclose(file);
    unlink(temp_file);

    return 0;
}