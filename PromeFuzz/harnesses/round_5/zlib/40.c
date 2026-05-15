// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Create dummy file for gzFile operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the file for reading with gzip
    gzFile file_read = gzopen("./dummy_file", "rb");
    if (file_read) {
        // Test gzgets
        char buffer[1024];
        for (int i = 0; i < 5 && !gzeof(file_read); i++) {
            gzgets(file_read, buffer, sizeof(buffer));
        }
        
        // Test gzungetc
        int c = gzgetc(file_read);
        if (c != -1) {
            gzungetc(c, file_read);
        }
        
        // Test gztell
        gztell(file_read);
        
        // Test gzerror
        int errnum;
        gzerror(file_read, &errnum);
        
        // Test zError with various error codes
        for (int err = -5; err <= 5; err++) {
            zError(err);
        }
        
        gzclose(file_read);
    }
    
    // Open file for writing with gzip
    gzFile file_write = gzopen("./dummy_file", "wb");
    if (file_write) {
        // Test gzprintf with different formats
        if (Size > 0) {
            // Use first byte as format specifier selector
            uint8_t fmt_selector = Data[0] % 4;
            switch (fmt_selector) {
                case 0:
                    gzprintf(file_write, "Data size: %zu\n", Size);
                    break;
                case 1:
                    gzprintf(file_write, "First byte: 0x%02x\n", Data[0]);
                    break;
                case 2:
                    // Create a null-terminated string from Data for safe printing
                    {
                        size_t str_len = Size < 100 ? Size : 100;
                        char safe_str[101];
                        memcpy(safe_str, Data, str_len);
                        safe_str[str_len] = '\0';
                        gzprintf(file_write, "String: %s\n", safe_str);
                    }
                    break;
                case 3:
                    gzprintf(file_write, "Integer: %d\n", (int)Size);
                    break;
            }
        } else {
            gzprintf(file_write, "Empty input\n");
        }
        
        gzclose(file_write);
    }
    
    // Additional zError tests with random error codes
    if (Size > 0) {
        // Use first few bytes to generate error codes
        for (size_t i = 0; i < Size && i < 10; i++) {
            zError((int)Data[i]);
        }
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}