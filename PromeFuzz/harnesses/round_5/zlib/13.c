// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek64 at gzlib.c:367:19 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzoffset at gzlib.c:490:17 in zlib.h
// gzoffset64 at gzlib.c:469:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gztell64 at gzlib.c:446:19 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    // Test with reading mode
    gzFile file_read = gzopen("./dummy_file", "rb");
    if (file_read) {
        // Use first byte to determine seek offset base
        uint8_t offset_base = Data[0];
        
        // Test gzseek and gzseek64 with different seek modes
        gzseek(file_read, (z_off_t)offset_base, SEEK_SET);
        gzseek64(file_read, (z_off64_t)offset_base, SEEK_SET);
        
        // Test gztell64
        z_off64_t tell_pos = gztell64(file_read);
        (void)tell_pos; // Use result to avoid unused variable warning
        
        // Test gzoffset and gzoffset64
        z_off_t offset1 = gzoffset(file_read);
        z_off64_t offset2 = gzoffset64(file_read);
        (void)offset1;
        (void)offset2;
        
        // Test SEEK_CUR mode
        if (Size > 1) {
            gzseek(file_read, (z_off_t)Data[1], SEEK_CUR);
            gzseek64(file_read, (z_off64_t)Data[1], SEEK_CUR);
        }
        
        gzclose(file_read);
    }
    
    // Test with writing mode
    gzFile file_write = gzopen("./dummy_file", "wb");
    if (file_write) {
        // Write some data first
        if (Size > 0) {
            for (size_t i = 0; i < Size && i < 100; i++) {
                gzputc(file_write, Data[i]);
            }
        }
        
        // Test seek in write mode (forward seeks only)
        if (Size > 2) {
            gzseek(file_write, (z_off_t)Data[2], SEEK_SET);
            gzseek64(file_write, (z_off64_t)Data[2], SEEK_SET);
        }
        
        // Test position functions in write mode
        z_off64_t write_tell = gztell64(file_write);
        z_off_t write_offset = gzoffset(file_write);
        z_off64_t write_offset64 = gzoffset64(file_write);
        (void)write_tell;
        (void)write_offset;
        (void)write_offset64;
        
        gzclose(file_write);
    }
    
    // Test with append mode
    gzFile file_append = gzopen("./dummy_file", "ab");
    if (file_append) {
        // Write a byte
        if (Size > 0) {
            gzputc(file_append, Data[0]);
        }
        
        // Test position functions
        z_off64_t append_tell = gztell64(file_append);
        (void)append_tell;
        
        gzclose(file_append);
    }
    
    remove_dummy_file();
    return 0;
}