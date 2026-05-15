// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write data to dummy file
    write_dummy_file(Data, Size);
    
    // Open the file for reading
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) return 0;
    
    // Use first byte to determine operation mode
    uint8_t mode = Data[0] % 6;
    
    // Extract parameters from fuzzer input
    z_off_t offset = 0;
    z_off64_t offset64 = 0;
    int whence = SEEK_SET;
    
    if (Size >= sizeof(z_off_t) + 2) {
        memcpy(&offset, Data + 1, sizeof(z_off_t));
        memcpy(&offset64, Data + 1, sizeof(z_off64_t));
        whence = Data[sizeof(z_off_t) + 1] % 3;
    }
    
    // Call target functions based on mode
    switch (mode) {
        case 0:
            gzseek(file, offset, whence);
            break;
        case 1:
            gzseek64(file, offset64, whence);
            break;
        case 2:
            gzoffset64(file);
            break;
        case 3:
            gztell64(file);
            break;
        case 4:
            gzoffset(file);
            break;
        case 5:
            gzgetc(file);
            break;
    }
    
    // Try some additional operations to explore more states
    if (Size > 10) {
        // Try seeking with different parameters
        gzseek(file, offset / 2, SEEK_CUR);
        gzseek64(file, offset64 / 4, SEEK_SET);
        
        // Try getting current position
        gztell64(file);
        
        // Try reading a few bytes
        for (int i = 0; i < 3 && !gzeof(file); i++) {
            gzgetc(file);
        }
        
        // Try getting compressed offset
        gzoffset64(file);
        gzoffset(file);
    }
    
    // Clean up
    gzclose(file);
    unlink("./dummy_file");
    
    return 0;
}