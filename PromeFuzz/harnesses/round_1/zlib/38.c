// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

static int gzvprintf_helper(gzFile file, const char *format, ...) {
    va_list va;
    va_start(va, format);
    int result = gzvprintf(file, format, va);
    va_end(va);
    return result;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    gzFile write_file = gzopen("./dummy_file", "wb");
    if (!write_file) {
        remove("./dummy_file");
        return 0;
    }

    gzFile read_file = gzopen("./dummy_file", "rb");
    if (!read_file) {
        gzclose(write_file);
        remove("./dummy_file");
        return 0;
    }

    int errnum = 0;
    const char *error_msg = NULL;

    if (Size > 0) {
        int byte = Data[0] & 0xFF;
        int putc_result = gzputc(write_file, byte);
        if (putc_result == -1) {
            error_msg = gzerror(write_file, &errnum);
        }
    }

    if (Size > 1) {
        const char *str = (const char *)(Data + 1);
        size_t str_len = Size - 1;
        char *null_term_str = malloc(str_len + 1);
        if (null_term_str) {
            memcpy(null_term_str, str, str_len);
            null_term_str[str_len] = '\0';
            int puts_result = gzputs(write_file, null_term_str);
            if (puts_result == -1) {
                error_msg = gzerror(write_file, &errnum);
            }
            free(null_term_str);
        }
    }

    if (Size > 2) {
        int fmt_len = Data[1] % 32;
        if (fmt_len > 0 && fmt_len < Size - 2) {
            char *format = malloc(fmt_len + 1);
            if (format) {
                memcpy(format, Data + 2, fmt_len);
                format[fmt_len] = '\0';
                int printf_result = gzprintf(write_file, format, (int)Data[0], (int)Size);
                if (printf_result == -1) {
                    error_msg = gzerror(write_file, &errnum);
                }
                free(format);
            }
        }
    }

    if (Size > 3) {
        int vfmt_len = Data[2] % 32;
        if (vfmt_len > 0 && vfmt_len < Size - 3) {
            char *vformat = malloc(vfmt_len + 1);
            if (vformat) {
                memcpy(vformat, Data + 3, vfmt_len);
                vformat[vfmt_len] = '\0';
                int vprintf_result = gzvprintf_helper(write_file, vformat);
                if (vprintf_result != Z_OK) {
                    error_msg = gzerror(write_file, &errnum);
                }
                free(vformat);
            }
        }
    }

    gzclose(write_file);

    if (read_file) {
        int getc_result = gzgetc(read_file);
        while (getc_result != -1) {
            getc_result = gzgetc(read_file);
        }
        
        error_msg = gzerror(read_file, &errnum);
        if (errnum != Z_OK && errnum != Z_STREAM_END) {
            gzclearerr(read_file);
        }
        
        gzclose(read_file);
    }

    remove("./dummy_file");
    return 0;
}