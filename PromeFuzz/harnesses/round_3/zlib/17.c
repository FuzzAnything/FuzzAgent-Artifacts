// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzzed data to dummy file for file-based operations
    write_dummy_file(Data, Size);
    
    // Test gzopen and gzopen64
    const char *modes[] = {"r", "w", "a", "rb", "wb", "ab", "r+", "w+", "a+"};
    int mode_idx = Data[0] % (sizeof(modes)/sizeof(modes[0]));
    
    gzFile file1 = gzopen("./dummy_file", modes[mode_idx]);
    if (file1) {
        gzclose(file1);
    }
    
    gzFile file2 = gzopen64("./dummy_file", modes[mode_idx]);
    if (file2) {
        gzclose(file2);
    }
    
    // Test gzdopen with various file descriptors
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        gzFile file3 = gzdopen(fd, "r");
        if (file3) {
            // Test gzfread
            char read_buf[1024];
            z_size_t items_read = 0;
            if (Size > 0) {
                size_t read_size = (Size > sizeof(read_buf)) ? sizeof(read_buf) : Size;
                items_read = gzfread(read_buf, 1, read_size, file3);
            }
            
            // Test gzfwrite (if file opened for writing, but we opened for reading)
            // We'll test with a separate file opened for writing
            gzclose(file3);
        } else {
            close(fd);
        }
    }
    
    // Test gzfwrite and gzprintf with a new file
    gzFile write_file = gzopen("./test_output.gz", "wb");
    if (write_file) {
        // Test gzfwrite
        if (Size > 0) {
            z_size_t items_written = gzfwrite(Data, 1, Size, write_file);
            
            // Test gzprintf
            char format_buf[256];
            if (Size < sizeof(format_buf)) {
                memcpy(format_buf, Data, Size);
                format_buf[Size % sizeof(format_buf)] = '\0';
            } else {
                memcpy(format_buf, Data, sizeof(format_buf) - 1);
                format_buf[sizeof(format_buf) - 1] = '\0';
            }
            
            int printed = gzprintf(write_file, "Test: %s", format_buf);
        }
        gzclose(write_file);
        unlink("./test_output.gz");
    }
    
    // Test with invalid/null arguments
    gzFile null_file = gzopen(NULL, "r");
    if (null_file) {
        gzclose(null_file);
    }
    
    gzFile invalid_fd_file = gzdopen(-1, "r");
    if (invalid_fd_file) {
        gzclose(invalid_fd_file);
    }
    
    // Cleanup
    cleanup_dummy_file();
    
    return 0;
}