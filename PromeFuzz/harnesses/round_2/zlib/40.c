// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    fwrite(data, 1, size, f);
    fclose(f);
    return 0;
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer data
    if (create_dummy_file(Data, Size) != 0) return 0;
    
    // Test gzopen with various modes
    const char *modes[] = {"rb", "wb", "ab", "xb"};
    int mode_idx = Data[0] % 4;
    gzFile file1 = gzopen("./dummy_file", modes[mode_idx]);
    
    if (file1) {
        // Test gzprintf if opened for writing
        if (mode_idx == 1 || mode_idx == 2 || mode_idx == 3) {
            if (Size > 10) {
                // Create a safe format string
                char format[32];
                size_t safe_len = Size > 100 ? 100 : (Size - 1);
                if (safe_len > 0) {
                    snprintf(format, sizeof(format), "Data: %%.%lus", (unsigned long)safe_len);
                    // Create a null-terminated string from Data+1
                    char *safe_str = (char*)malloc(safe_len + 1);
                    if (safe_str) {
                        memcpy(safe_str, Data + 1, safe_len);
                        safe_str[safe_len] = '\0';
                        gzprintf(file1, format, safe_str);
                        free(safe_str);
                    }
                }
            }
        }
        
        // Test gzseek with various parameters - ensure we have enough data
        if (Size >= sizeof(z_off_t) + 2) {
            z_off_t offset;
            memcpy(&offset, Data + 1, sizeof(z_off_t));
            int whence = Data[sizeof(z_off_t) + 1] % 3;
            gzseek(file1, offset, whence);
        }
        
        // Test gzputc if opened for writing
        if (mode_idx == 1 || mode_idx == 2 || mode_idx == 3) {
            size_t putc_limit = Size > 100 ? 100 : Size;
            for (size_t i = 0; i < putc_limit; i++) {
                gzputc(file1, Data[i]);
            }
        }
        
        // Close the file
        gzclose(file1);
    }
    
    // Test gzdopen with file descriptor
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        const char *dmodes[] = {"rb", "wb"};
        int dmode_idx = Data[0] % 2;
        gzFile file2 = gzdopen(fd, dmodes[dmode_idx]);
        
        if (file2) {
            // Test some operations on the gzdopen file
            if (Size >= sizeof(z_off_t) + 1) {
                z_off_t offset;
                memcpy(&offset, Data + 1, sizeof(z_off_t));
                gzseek(file2, offset % 1000, SEEK_SET);
            }
            
            gzclose(file2);
        } else {
            close(fd);
        }
    }
    
    // Test gzopen with invalid paths (should return NULL)
    if (Size > 20) {
        char invalid_path[50];
        size_t copy_len = Size > 49 ? 49 : Size;
        memcpy(invalid_path, Data, copy_len);
        invalid_path[copy_len] = '\0';
        gzFile invalid_file = gzopen(invalid_path, "rb");
        if (invalid_file) {
            gzclose(invalid_file);
        }
    }
    
    // Cleanup
    cleanup_dummy_file();
    
    return 0;
}