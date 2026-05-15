// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_files(void) {
    remove("./dummy_file");
    remove("./dummy_output");
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Clean up any leftover files from previous runs
    cleanup_files();
    
    // Create dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test gzopen64
    gzFile gz1 = gzopen64("./dummy_file", "rb");
    if (gz1) {
        gzclose(gz1);
    }
    
    // Test gzopen
    gzFile gz2 = gzopen("./dummy_file", "rb");
    if (gz2) {
        gzclose(gz2);
    }
    
    // Test gzdopen with file descriptor
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        gzFile gz3 = gzdopen(fd, "rb");
        if (gz3) {
            gzclose(gz3);
        } else {
            close(fd);
        }
    }
    
    // Test writing functions
    gzFile gz_out = gzopen64("./dummy_output", "wb");
    if (gz_out) {
        // Test gzputc with some bytes from input
        for (size_t i = 0; i < Size && i < 100; i++) {
            gzputc(gz_out, Data[i]);
        }
        
        // Test gzputs with a string from input (null-terminated)
        if (Size > 0) {
            char *str = malloc(Size + 1);
            if (str) {
                memcpy(str, Data, Size);
                str[Size] = 0;
                gzputs(gz_out, str);
                free(str);
            }
        }
        
        // Test gzprintf with format string
        if (Size > 0) {
            char *fmt_str = malloc(Size + 1);
            if (fmt_str) {
                memcpy(fmt_str, Data, Size);
                fmt_str[Size] = 0;
                gzprintf(gz_out, "Data: %s\n", fmt_str);
                free(fmt_str);
            }
        }
        
        gzclose(gz_out);
    }
    
    // Clean up created files
    cleanup_files();
    
    return 0;
}