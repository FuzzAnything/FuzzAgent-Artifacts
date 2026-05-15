// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "code/zlib.h"

static int call_gzvprintf(gzFile file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = gzvprintf(file, format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create dummy files for reading and writing
    const char *read_filename = "./dummy_read.gz";
    const char *write_filename = "./dummy_write.gz";
    
    // Write data to read file
    FILE *fp = fopen(read_filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open gzip files
    gzFile read_file = gzopen(read_filename, "rb");
    gzFile write_file = gzopen(write_filename, "wb");
    
    if (!read_file || !write_file) {
        if (read_file) gzclose(read_file);
        if (write_file) gzclose(write_file);
        remove(read_filename);
        remove(write_filename);
        return 0;
    }

    // Test gzgetc - read bytes until EOF
    int c;
    while ((c = gzgetc(read_file)) != -1) {
        // Just consume the bytes
    }

    // Reset read file position by reopening
    gzclose(read_file);
    read_file = gzopen(read_filename, "rb");
    if (!read_file) {
        gzclose(write_file);
        remove(read_filename);
        remove(write_filename);
        return 0;
    }

    // Test gzputs - write a null-terminated string
    // Create a null-terminated string from input data
    size_t str_len = Size > 100 ? 100 : Size;
    char *str = (char *)malloc(str_len + 1);
    if (str) {
        memcpy(str, Data, str_len);
        str[str_len] = '\0';
        gzputs(write_file, str);
        free(str);
    }

    // Test gzputc - write individual bytes
    for (size_t i = 0; i < (Size > 10 ? 10 : Size); i++) {
        gzputc(write_file, Data[i]);
    }

    // Test gzwrite - write a buffer
    size_t write_len = Size > 50 ? 50 : Size;
    gzwrite(write_file, (voidpc)Data, (unsigned)write_len);

    // Test gzvprintf - write formatted data
    if (Size > 20) {
        char format_buf[20];
        memcpy(format_buf, Data, 19);
        format_buf[19] = '\0';
        call_gzvprintf(write_file, "Test: %s, %d, %x\n", format_buf, (int)Size, Data[0]);
    }

    // Test gzerror - check for errors
    int errnum;
    const char *errmsg;
    
    errmsg = gzerror(read_file, &errnum);
    errmsg = gzerror(write_file, &errnum);

    // Clean up
    gzclose(read_file);
    gzclose(write_file);
    
    remove(read_filename);
    remove(write_filename);
    
    return 0;
}