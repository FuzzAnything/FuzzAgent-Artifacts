// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer data
    create_dummy_file(Data, Size);
    
    // Test gzopen64
    gzFile file1 = gzopen64("./dummy_file", "rb");
    if (file1) {
        // Test gzfread
        char buf1[1024];
        z_size_t read_size = Size > sizeof(buf1) ? sizeof(buf1) : Size;
        z_size_t items_read = gzfread(buf1, 1, read_size, file1);
        (void)items_read; // Use result to avoid unused warning
        
        gzclose(file1);
    }
    
    // Test gzopen
    gzFile file2 = gzopen("./dummy_file", "rb");
    if (file2) {
        // Test gzfwrite
        char buf2[1024];
        memset(buf2, 'A', sizeof(buf2));
        z_size_t items_written = gzfwrite(buf2, 1, sizeof(buf2), file2);
        (void)items_written; // Use result to avoid unused warning
        
        // Test gzprintf
        gzprintf(file2, "Test format: %d %s\n", 123, "fuzz");
        
        gzclose(file2);
    }
    
    // Test gzdopen
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        gzFile file3 = gzdopen(fd, "rb");
        if (file3) {
            char buf3[512];
            z_size_t items_read2 = gzfread(buf3, 1, sizeof(buf3), file3);
            (void)items_read2; // Use result to avoid unused warning
            gzclose(file3);
        } else {
            close(fd);
        }
    }
    
    // Cleanup
    cleanup_dummy_file();
    
    return 0;
}