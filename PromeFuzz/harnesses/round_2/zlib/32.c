// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
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

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create dummy file for gzopen/gzdopen operations
    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    // Write some data to the file for reading operations
    uint8_t dummy_data[] = "test data for fuzzing";
    write(fd, dummy_data, sizeof(dummy_data) - 1);
    lseek(fd, 0, SEEK_SET);

    // Use first byte to determine mode for various operations
    uint8_t mode_selector = Data[0] % 6;
    const char *modes[] = {"rb", "wb", "ab", "r", "w", "a"};
    const char *mode = modes[mode_selector % 6];

    // Test gzopen
    gzFile file1 = gzopen(filename, mode);
    if (file1) {
        // Test gzputc with second byte (if available)
        if (Size > 1) {
            int c = Data[1] % 256;
            gzputc(file1, c);
        }

        // Test gzprintf with available data
        if (Size > 10) {
            // Use a safe format string
            const char *format = "Data: %.*s\n";
            size_t format_len = strlen(format);
            size_t data_len = Size > 100 ? 100 : Size - 1;
            gzprintf(file1, format, (int)data_len, Data + 1);
        }

        // Test gzseek with various offsets
        if (Size > 2) {
            // Use bytes 2-5 for offset (if available)
            z_off_t offset = 0;
            size_t offset_bytes = Size > 6 ? 4 : Size - 2;
            for (size_t i = 0; i < offset_bytes; i++) {
                offset = (offset << 8) | Data[2 + i];
            }
            
            // Use byte 6 (if available) for whence
            int whence = SEEK_SET;
            if (Size > 6) {
                switch (Data[6] % 3) {
                    case 0: whence = SEEK_SET; break;
                    case 1: whence = SEEK_CUR; break;
                    case 2: whence = SEEK_END; break;
                }
            }
            
            gzseek(file1, offset, whence);
        }

        // Close the file
        gzclose(file1);
    }

    // Test gzdopen with the file descriptor
    lseek(fd, 0, SEEK_SET);
    
    // Use different mode for gzdopen
    uint8_t gzdopen_mode_selector = (Data[0] + 1) % 6;
    const char *gzdopen_mode = modes[gzdopen_mode_selector % 6];
    
    gzFile file2 = gzdopen(fd, gzdopen_mode);
    if (file2) {
        // Perform some operations on the gzdopen file
        if (Size > 7) {
            // Test gzputc
            int c = Data[7] % 256;
            gzputc(file2, c);
            
            // Test gzseek with different parameters
            if (Size > 11) {
                z_off_t offset2 = 0;
                size_t offset_bytes2 = Size > 15 ? 4 : Size - 11;
                for (size_t i = 0; i < offset_bytes2; i++) {
                    offset2 = (offset2 << 8) | Data[11 + i];
                }
                
                int whence2 = SEEK_SET;
                if (Size > 15) {
                    switch (Data[15] % 3) {
                        case 0: whence2 = SEEK_SET; break;
                        case 1: whence2 = SEEK_CUR; break;
                        case 2: whence2 = SEEK_END; break;
                    }
                }
                
                gzseek(file2, offset2, whence2);
            }
        }
        
        // Close the gzdopen file
        gzclose(file2);
    } else {
        // If gzdopen failed, close the file descriptor
        close(fd);
    }

    // Clean up
    close(fd);
    unlink(filename);

    return 0;
}