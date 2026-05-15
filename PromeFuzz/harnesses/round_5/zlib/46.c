// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
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
#include <stdio.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer data
    create_dummy_file(Data, Size);
    
    // Open file for reading
    gzFile file_read = gzopen("./dummy_file", "rb");
    if (!file_read) {
        remove_dummy_file();
        return 0;
    }
    
    // Open another file for writing
    gzFile file_write = gzopen("./dummy_file_write", "wb");
    if (!file_write) {
        gzclose(file_read);
        remove_dummy_file();
        return 0;
    }
    
    // Use first byte to determine buffer size (2-255, minimum 2 for gzbuffer)
    unsigned buffer_size = (Data[0] % 254) + 2;
    
    // Test gzbuffer on both files
    gzbuffer(file_read, buffer_size);
    gzbuffer(file_write, buffer_size);
    
    // Test gzgetc and gzungetc
    int c = gzgetc(file_read);
    if (c != -1) {
        // Try to push back the character we just read
        gzungetc(c, file_read);
        // Read it again
        gzgetc(file_read);
    }
    
    // Test gzread with varying buffer sizes
    size_t read_offset = 1 % Size;
    size_t read_size = Size > read_offset ? Size - read_offset : 0;
    if (read_size > 0) {
        // Limit read size to avoid excessive memory usage
        if (read_size > 1024) read_size = 1024;
        char *read_buf = (char *)malloc(read_size);
        if (read_buf) {
            gzread(file_read, read_buf, read_size);
            free(read_buf);
        }
    }
    
    // Test gztell on read file
    gztell(file_read);
    
    // Test gzwrite
    size_t write_size = Size > 10 ? 10 : Size;
    if (write_size > 0) {
        gzwrite(file_write, Data, write_size);
    }
    
    // Test gztell on write file
    gztell(file_write);
    
    // Try another gzgetc to potentially hit EOF
    gzgetc(file_read);
    
    // Clean up
    gzclose(file_read);
    gzclose(file_write);
    
    remove_dummy_file();
    remove("./dummy_file_write");
    
    return 0;
}