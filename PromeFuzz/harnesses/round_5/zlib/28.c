// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "zlib.h"

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

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    // Test gzfread and gzfwrite
    gzFile file = gzopen("./dummy_file", "wb");
    if (file) {
        // Use part of data for buffer
        size_t buf_size = Size / 4;
        if (buf_size > 0) {
            void *buf = malloc(buf_size);
            if (buf) {
                // Test gzfwrite
                size_t nitems = buf_size / 2;
                if (nitems > 0) {
                    size_t size_per_item = 2;
                    gzfwrite(Data, size_per_item, nitems, file);
                }
                
                // Test gzwrite
                gzwrite(file, Data, buf_size % 256);
                
                // Test gzprintf with various formats
                gzprintf(file, "Test %d %s %x\n", 
                        Data[0] & 0xFF, 
                        "string", 
                        Data[1] & 0xFF);
                
                free(buf);
            }
        }
        gzclose(file);
    }
    
    // Test gzdopen with file descriptor
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        gzFile fd_file = gzdopen(fd, "rb");
        if (fd_file) {
            // Test gzfread
            size_t read_buf_size = Size / 8;
            if (read_buf_size > 0) {
                void *read_buf = malloc(read_buf_size);
                if (read_buf) {
                    size_t read_nitems = read_buf_size / 4;
                    if (read_nitems > 0) {
                        size_t read_size = 4;
                        gzfread(read_buf, read_size, read_nitems, fd_file);
                    }
                    free(read_buf);
                }
            }
            gzclose(fd_file);
        } else {
            close(fd);
        }
    }
    
    // Test reading from the file we wrote
    file = gzopen("./dummy_file", "rb");
    if (file) {
        size_t read_size = Size / 16;
        if (read_size > 0) {
            void *read_buf = malloc(read_size);
            if (read_buf) {
                gzfread(read_buf, 1, read_size, file);
                free(read_buf);
            }
        }
        gzclose(file);
    }
    
    cleanup_dummy_file();
    return 0;
}