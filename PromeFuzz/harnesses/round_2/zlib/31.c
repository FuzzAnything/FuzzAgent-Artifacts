// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write data to dummy file for file-based operations
    write_dummy_file(Data, Size);
    
    // Use first byte to control mode and operation variations
    uint8_t mode_selector = Data[0];
    const char *mode = (mode_selector & 1) ? "rb" : "wb";
    
    // Test gzopen64
    gzFile file1 = gzopen64("./dummy_file", mode);
    if (file1) {
        // Test gzfread if in read mode, gzfwrite if in write mode
        char buffer[1024];
        if (mode[0] == 'r') {
            gzfread(buffer, 1, sizeof(buffer), file1);
        } else {
            size_t write_size = Size > sizeof(buffer) ? sizeof(buffer) : Size;
            gzfwrite(Data, 1, write_size, file1);
        }
        gzclose(file1);
    }
    
    // Test gzopen
    gzFile file2 = gzopen("./dummy_file", mode);
    if (file2) {
        // Test gzprintf in write mode
        if (mode[0] == 'w') {
            gzprintf(file2, "Test format: %d bytes\n", (int)Size);
        }
        gzclose(file2);
    }
    
    // Test gzdopen with file descriptor
    int fd = open("./dummy_file", (mode[0] == 'r') ? O_RDONLY : O_WRONLY);
    if (fd >= 0) {
        gzFile file3 = gzdopen(fd, mode);
        if (file3) {
            // Perform some operations
            char buffer[512];
            if (mode[0] == 'r') {
                gzfread(buffer, 1, sizeof(buffer), file3);
            }
            gzclose(file3);
        } else {
            close(fd);
        }
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}