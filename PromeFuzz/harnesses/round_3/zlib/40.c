// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
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
#include <unistd.h>
#include <fcntl.h>

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Create dummy file for gz operations
    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    // Write data to file if we have any
    if (Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
    
    // Test with reading mode
    gzFile file_read = gzopen(filename, "rb");
    if (file_read != NULL) {
        // Set buffer size if we have enough data
        if (Size >= 2) {
            unsigned buffer_size = (Data[0] << 8) | Data[1];
            gzbuffer(file_read, buffer_size);
        }
        
        // Try to read some data
        if (Size > 0) {
            unsigned char read_buf[1024];
            unsigned read_len = Size > sizeof(read_buf) ? sizeof(read_buf) : Size;
            gzread(file_read, read_buf, read_len);
            
            // Try gzgetc and gzungetc
            int c = gzgetc(file_read);
            if (c != -1) {
                gzungetc(c, file_read);
                // Read it back
                gzgetc(file_read);
            }
        }
        
        gzclose(file_read);
    }
    
    // Test with writing mode
    gzFile file_write = gzopen(filename, "wb");
    if (file_write != NULL) {
        // Set buffer size if we have enough data
        if (Size >= 2) {
            unsigned buffer_size = (Data[0] << 8) | Data[1];
            gzbuffer(file_write, buffer_size);
        }
        
        // Try to write some data
        if (Size > 0) {
            unsigned write_len = Size > 1024 ? 1024 : Size;
            gzwrite(file_write, (voidpc)Data, write_len);
        }
        
        gzclose(file_write);
    }
    
    // Clean up
    unlink(filename);
    
    return 0;
}