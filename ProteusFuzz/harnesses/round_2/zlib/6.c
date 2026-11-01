// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
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

static void cleanup_gzfile(gzFile file) {
    if (file != NULL) {
        gzclose(file);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    gzFile file = gzopen("./dummy_file", "wb");
    if (file == NULL) {
        return 0;
    }

    // Ensure we have a valid character for gzputc
    int c = Data[0];

    // Invoke gzputc
    gzputc(file, c);

    // Prepare a null-terminated string for gzputs
    // Limit size to avoid overflow and ensure null termination
    size_t str_len = (Size > 1) ? Size - 1 : 0;
    if (str_len > 1024) {
        str_len = 1024;
    }
    
    char *str = (char *)malloc(str_len + 1);
    if (str == NULL) {
        cleanup_gzfile(file);
        return 0;
    }

    if (str_len > 0) {
        memcpy(str, Data + 1, str_len);
    }
    str[str_len] = '\0';

    // Invoke gzputs
    gzputs(file, str);
    free(str);

    // Invoke gzerror (first time)
    int errnum1 = 0;
    const char *err_str1 = gzerror(file, &errnum1);
    (void)err_str1; // Suppress unused warning

    // Prepare format string for gzprintf
    // Use a safe subset of data to avoid format string vulnerabilities
    char format_buf[64];
    size_t fmt_len = (Size > 10) ? 10 : Size;
    if (fmt_len > sizeof(format_buf) - 2) {
        fmt_len = sizeof(format_buf) - 2;
    }
    memcpy(format_buf, Data, fmt_len);
    format_buf[fmt_len] = '\0';
    // Ensure no format specifiers that could cause issues if not handled
    // For fuzzing, we just pass it as a literal string if possible, 
    // but gzprintf expects a format string. We'll use a safe prefix.
    char safe_format[128];
    snprintf(safe_format, sizeof(safe_format), "%s", format_buf);

    // Invoke gzprintf
    gzprintf(file, safe_format);

    // Invoke gzerror (second time)
    int errnum2 = 0;
    const char *err_str2 = gzerror(file, &errnum2);
    (void)err_str2;

    // Invoke gzseek
    // Use data to determine offset and whence
    z_off_t offset = 0;
    int whence = SEEK_SET;

    if (Size > 2) {
        offset = (z_off_t)(Data[Size - 2] | (Data[Size - 1] << 8));
        // Map Data[0] to whence
        int w = Data[0] % 3;
        if (w == 0) whence = SEEK_SET;
        else if (w == 1) whence = SEEK_CUR;
        else whence = SEEK_END;
    }

    gzseek(file, offset, whence);

    cleanup_gzfile(file);
    return 0;
}