// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "zlib.h"

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the file for reading
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        remove("./dummy_file");
        return 0;
    }
    
    // Test gzread with various buffer sizes
    char buffer[4096];
    unsigned int read_len;
    
    // Try reading small chunks
    for (int i = 0; i < 3 && !gzeof(file); i++) {
        read_len = (Size % 1024) + 1;
        if (read_len > sizeof(buffer)) {
            read_len = sizeof(buffer);
        }
        gzread(file, buffer, read_len);
    }
    
    // Test gzgetc and gzgetc_
    gzrewind(file);
    int byte1 = gzgetc(file);
    int byte2 = gzgetc_(file);
    
    // Test gzungetc if we read something valid
    if (byte1 != -1) {
        gzungetc(byte1, file);
        int byte3 = gzgetc(file);  // Should get the same byte back
    }
    
    // Test gzrewind
    int rewind_result = gzrewind(file);
    
    // Read more data after rewind
    if (rewind_result == 0) {
        // Read in chunks
        int total_read = 0;
        while (total_read < (int)Size && !gzeof(file)) {
            read_len = (Size - total_read) % 256 + 1;
            if (read_len > sizeof(buffer)) {
                read_len = sizeof(buffer);
            }
            int bytes_read = gzread(file, buffer, read_len);
            if (bytes_read <= 0) {
                break;
            }
            total_read += bytes_read;
        }
    }
    
    // Test gzgetc until EOF
    gzrewind(file);
    while (gzgetc(file) != -1) {
        // Just consume bytes
    }
    
    // Test gzgetc_ until EOF
    gzrewind(file);
    while (gzgetc_(file) != -1) {
        // Just consume bytes
    }
    
    // Test gzungetc with various values
    gzrewind(file);
    for (int c = 0; c < 256; c++) {
        if (gzungetc(c, file) == -1) {
            break;
        }
        // Read it back
        gzgetc(file);
    }
    
    // Close the file
    int close_result = gzclose_r(file);
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}