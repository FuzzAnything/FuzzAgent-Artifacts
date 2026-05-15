// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer data
    create_dummy_file(Data, Size);
    
    // Test reading operations
    gzFile read_file = gzopen("./dummy_file", "rb");
    if (read_file) {
        // Test gzeof on read file
        int eof_result = gzeof(read_file);
        (void)eof_result; // Use result to avoid unused variable warning
        
        // Test gzclose_r
        int close_r_result = gzclose_r(read_file);
        (void)close_r_result;
        
        // Reopen for testing gzclose
        read_file = gzopen("./dummy_file", "rb");
        if (read_file) {
            int close_result = gzclose(read_file);
            (void)close_result;
        }
    }
    
    // Test writing operations
    gzFile write_file = gzopen("./dummy_file", "wb");
    if (write_file) {
        // Test gzputc with first byte
        int putc_result = gzputc(write_file, Data[0]);
        (void)putc_result;
        
        // Test gzwrite with remaining data (if any)
        if (Size > 1) {
            size_t write_len = Size - 1;
            if (write_len > 0x7FFFFFFF) {
                write_len = 0x7FFFFFFF; // Cap to fit in int
            }
            int write_result = gzwrite(write_file, Data + 1, (unsigned)write_len);
            (void)write_result;
        }
        
        // Test gzeof on write file (should return 0)
        int write_eof = gzeof(write_file);
        (void)write_eof;
        
        // Test gzclose_w
        int close_w_result = gzclose_w(write_file);
        (void)close_w_result;
        
        // Reopen for testing gzclose on write file
        write_file = gzopen("./dummy_file", "wb");
        if (write_file) {
            // Write some data before closing
            if (Size > 0) {
                gzputc(write_file, Data[Size - 1]);
            }
            int close_result = gzclose(write_file);
            (void)close_result;
        }
    }
    
    // Test edge cases with invalid files
    gzFile null_file = NULL;
    int null_close_r = gzclose_r(null_file);
    (void)null_close_r;
    
    int null_close_w = gzclose_w(null_file);
    (void)null_close_w;
    
    int null_close = gzclose(null_file);
    (void)null_close;
    
    // Clean up
    cleanup_dummy_file();
    
    return 0;
}