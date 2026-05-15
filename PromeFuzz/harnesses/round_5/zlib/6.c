// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Write data to dummy file for potential reading operations
    write_dummy_file(Data, Size);
    
    // Use first byte as mode selector
    int mode_selector = Data[0] % 4;
    const char *modes[] = {"wb", "ab", "wb+", "ab+"};
    
    // Open file for writing
    gzFile file = gzopen("./dummy_file", modes[mode_selector]);
    if (!file) return 0;
    
    // Use second byte for gzputc
    int byte_to_write = Data[1];
    int putc_result = gzputc(file, byte_to_write);
    (void)putc_result; // Result intentionally unused
    
    // Create a string for gzputs from input data
    static char str_buffer[256];
    size_t str_len = Size < 256 ? Size : 255;
    memcpy(str_buffer, Data, str_len);
    str_buffer[str_len] = '\0';
    
    int puts_result = gzputs(file, str_buffer);
    (void)puts_result; // Result intentionally unused
    
    // First gzerror call
    int errnum1;
    const char *error1 = gzerror(file, &errnum1);
    (void)error1; // Result intentionally unused
    
    // Prepare format string for gzprintf
    static char format_buffer[256];
    size_t format_len = (Size / 2) < 256 ? (Size / 2) : 255;
    memcpy(format_buffer, Data, format_len);
    format_buffer[format_len] = '\0';
    
    // Use remaining bytes for integer argument
    int int_arg = 0;
    if (Size > format_len + 4) {
        int_arg = (Data[format_len] << 24) | 
                  (Data[format_len + 1] << 16) | 
                  (Data[format_len + 2] << 8) | 
                  Data[format_len + 3];
    }
    
    int printf_result = gzprintf(file, "%s: %d\n", format_buffer, int_arg);
    (void)printf_result; // Result intentionally unused
    
    // Second gzerror call
    int errnum2;
    const char *error2 = gzerror(file, &errnum2);
    (void)error2; // Result intentionally unused
    
    // Prepare parameters for gzseek
    z_off_t offset = 0;
    if (Size > 8) {
        offset = (Data[Size - 8] << 24) | 
                 (Data[Size - 7] << 16) | 
                 (Data[Size - 6] << 8) | 
                 Data[Size - 5];
    }
    
    int whence = Data[Size - 1] % 3; // 0=SEEK_SET, 1=SEEK_CUR, 2=SEEK_END
    z_off_t seek_result = gzseek(file, offset, whence);
    (void)seek_result; // Result intentionally unused
    
    // Cleanup
    gzclose(file);
    
    return 0;
}