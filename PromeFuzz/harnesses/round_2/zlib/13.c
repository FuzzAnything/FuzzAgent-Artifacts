// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzread at gzread.c:396:13 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
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

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    write_dummy_file(Data, Size);
    
    // Open file for reading
    gzFile file_read = gzopen("./dummy_file", "rb");
    if (!file_read) {
        remove_dummy_file();
        return 0;
    }
    
    // Open file for writing
    gzFile file_write = gzopen("./dummy_file", "wb");
    if (!file_write) {
        gzclose(file_read);
        remove_dummy_file();
        return 0;
    }
    
    // Use first byte to decide which operations to perform
    uint8_t op_selector = Data[0];
    
    // Test gzseek and gzseek64 on read file
    if (op_selector & 0x01) {
        // Check we have enough data for the operations
        if (Size >= 4) {
            // Use second and third bytes for offset (modulo to keep reasonable)
            z_off_t offset = (z_off_t)(Data[1] | (Data[2] << 8)) % 1000;
            int whence = (Data[3] & 0x01) ? SEEK_SET : SEEK_CUR;
            
            z_off_t result = gzseek(file_read, offset, whence);
            (void)result; // Ignore return value
            
            // Test gztell64 after seek
            z_off64_t tell_result = gztell64(file_read);
            (void)tell_result;
            
            // Test gzseek64 if we have enough data
            if (Size >= 7) {
                z_off64_t offset64 = (z_off64_t)(Data[4] | (Data[5] << 8) | (Data[6] << 16)) % 2000;
                z_off64_t result64 = gzseek64(file_read, offset64, whence);
                (void)result64;
            }
        }
    }
    
    // Test gzoffset and gzoffset64
    if (op_selector & 0x02) {
        z_off_t offset_result = gzoffset(file_read);
        (void)offset_result;
        
        z_off64_t offset64_result = gzoffset64(file_read);
        (void)offset64_result;
    }
    
    // Test gzputc on write file
    if (op_selector & 0x04) {
        // Use available bytes as characters to write
        size_t write_limit = Size < 11 ? Size : 11;
        for (size_t i = 7; i < write_limit; i++) {
            int putc_result = gzputc(file_write, Data[i]);
            (void)putc_result;
        }
        gzflush(file_write, Z_SYNC_FLUSH);
        
        // Test seek on write file (forward seeks only for writing)
        if (Size >= 13) {
            z_off_t write_offset = (z_off_t)(Data[11] | (Data[12] << 8)) % 500;
            gzseek(file_write, write_offset, SEEK_SET);
            
            // Write one more byte after seek if available
            if (Size >= 14) {
                gzputc(file_write, Data[13]);
            }
        }
    }
    
    // Test mixed operations
    if (op_selector & 0x08) {
        // Read some data to change state
        unsigned char buffer[10];
        size_t read_size = sizeof(buffer) < Size ? sizeof(buffer) : Size;
        gzread(file_read, buffer, read_size);
        
        // Test offset after read
        z_off64_t offset_after_read = gzoffset64(file_read);
        (void)offset_after_read;
        
        // Test tell after read
        z_off64_t tell_after_read = gztell64(file_read);
        (void)tell_after_read;
    }
    
    // Test error cases by seeking beyond reasonable bounds
    if (op_selector & 0x10) {
        // Large offset that might cause errors
        z_off64_t large_offset = (z_off64_t)1 << 40; // 1TB offset
        gzseek64(file_read, large_offset, SEEK_SET);
        
        // Should return -1, test offset after error
        z_off64_t offset_after_error = gzoffset64(file_read);
        (void)offset_after_error;
    }
    
    gzclose(file_read);
    gzclose(file_write);
    remove_dummy_file();
    
    return 0;
}