// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

static int create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file for operations that need a file
    if (create_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Test gzopen and gzopen64 with various modes
    const char *modes[] = {"rb", "wb", "ab", "r", "w", "a"};
    int mode_idx = Data[0] % (sizeof(modes) / sizeof(modes[0]));
    
    gzFile file1 = gzopen("./dummy_file", modes[mode_idx]);
    if (file1) {
        // Test gzputc with a byte from the input
        if (Size > 1) {
            gzputc(file1, Data[1]);
        }
        
        // Test gzputs with a string from the input (null-terminated)
        if (Size > 2) {
            // Create null-terminated string from input
            size_t str_len = Size > 100 ? 100 : Size - 1;
            char *str = (char *)malloc(str_len + 1);
            if (str) {
                memcpy(str, Data + 1, str_len);
                str[str_len] = '\0';
                gzputs(file1, str);
                free(str);
            }
        }
        
        // Test gzprintf with format string and data
        if (Size > 10) {
            gzprintf(file1, "Data: %.*s\n", (int)(Size > 50 ? 50 : Size - 1), Data + 1);
        }
        
        gzclose(file1);
    }
    
    // Test gzopen64
    gzFile file2 = gzopen64("./dummy_file", modes[(mode_idx + 1) % (sizeof(modes) / sizeof(modes[0]))]);
    if (file2) {
        if (Size > 3) {
            gzputc(file2, Data[2]);
        }
        gzclose(file2);
    }
    
    // Test gzdopen with a file descriptor
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        gzFile file3 = gzdopen(fd, "rb");
        if (file3) {
            // Try to read a byte to exercise the file
            if (Size > 4) {
                gzgetc(file3);
            }
            gzclose(file3);
        } else {
            close(fd);
        }
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}