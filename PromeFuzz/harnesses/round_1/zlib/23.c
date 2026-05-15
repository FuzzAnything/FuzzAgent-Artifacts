// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
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

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Use first byte for mode selection
    uint8_t mode_selector = Data[0];
    const char *mode;
    switch (mode_selector % 4) {
        case 0: mode = "wb"; break;
        case 1: mode = "rb"; break;
        case 2: mode = "ab"; break;
        case 3: mode = "wb+"; break;
    }

    // Use second byte for operation selection
    uint8_t op_selector = Data[1];
    
    // Create dummy file
    const char *filename = "./dummy_file";
    
    // Write remaining data to dummy file for reading operations
    if (Size > 2) {
        FILE *fp = fopen(filename, "wb");
        if (fp) {
            fwrite(Data + 2, 1, Size - 2, fp);
            fclose(fp);
        }
    }

    // Test gzopen
    gzFile file1 = gzopen(filename, mode);
    if (file1) {
        // Test gzputc if file opened for writing
        if (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+')) {
            for (size_t i = 2; i < Size && i < 100; i++) {
                gzputc(file1, Data[i]);
            }
        }
        gzclose(file1);
    }

    // Test gzopen64
    gzFile file2 = gzopen64(filename, mode);
    if (file2) {
        // Test gzprintf with various format strings
        if (Size > 10 && (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+'))) {
            const char *formats[] = {
                "Data: %d\n",
                "Size: %zu\n",
                "Value: %02x\n",
                "Test: %s\n"
            };
            int fmt_idx = Data[2] % 4;
            gzprintf(file2, formats[fmt_idx], Data[3], Size, Data[4], "fuzz");
        }
        gzclose(file2);
    }

    // Test gzdopen with file descriptor
    int fd = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd >= 0) {
        gzFile file3 = gzdopen(fd, mode);
        if (file3) {
            // Write some data if in write mode
            if (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+')) {
                for (size_t i = 2; i < Size && i < 50; i++) {
                    gzputc(file3, Data[i] ^ 0xFF);
                }
            }
            gzclose(file3);
        } else {
            close(fd);
        }
    }

    // Clean up
    unlink(filename);
    
    return 0;
}