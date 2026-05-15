// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer data
    create_dummy_file(Data, Size);
    
    // Test gzdopen
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        gzFile file1 = gzdopen(fd, "rb");
        if (file1) {
            // Test gzfread
            char read_buf[1024];
            z_size_t items_read = gzfread(read_buf, 1, sizeof(read_buf), file1);
            
            // Test gzclose
            gzclose(file1);
        } else {
            close(fd);
        }
    }
    
    // Test writing functions
    gzFile file2 = gzopen("./dummy_file", "ab");
    if (file2) {
        // Test gzfwrite
        z_size_t items_written = gzfwrite(Data, 1, Size % 256, file2);
        
        // Test gzwrite
        int bytes_written = gzwrite(file2, Data, Size % 1024);
        
        // Test gzprintf with various format strings
        if (Size > 10) {
            gzprintf(file2, "Data: %.*s\n", (int)(Size % 100), Data);
            gzprintf(file2, "Size: %zu\n", Size);
            gzprintf(file2, "Value: %d\n", (int)Data[0]);
        }
        
        gzclose(file2);
    }
    
    // Test with different modes
    gzFile file3 = gzopen("./dummy_file", "rb");
    if (file3) {
        // Test reading with different buffer sizes
        char buf2[512];
        z_size_t read2 = gzfread(buf2, 1, sizeof(buf2), file3);
        gzclose(file3);
    }
    
    // Test gzdopen with different modes
    fd = open("./dummy_file", O_RDWR);
    if (fd >= 0) {
        gzFile file4 = gzdopen(fd, "rb+");
        if (file4) {
            char buf3[256];
            gzfread(buf3, 1, sizeof(buf3), file4);
            gzclose(file4);
        } else {
            close(fd);
        }
    }
    
    cleanup_dummy_file();
    return 0;
}