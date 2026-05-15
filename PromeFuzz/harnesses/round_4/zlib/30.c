// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzerror at gzlib.c:513:22 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
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

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Create dummy files for gzFile operations
    const char* read_file = "./dummy_file_read";
    const char* write_file = "./dummy_file_write";
    
    // Write data to read file
    FILE *fp = fopen(read_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open gzFiles for reading and writing
    gzFile gz_read = gzopen(read_file, "rb");
    gzFile gz_write = gzopen(write_file, "wb");
    
    if (!gz_read || !gz_write) {
        if (gz_read) gzclose(gz_read);
        if (gz_write) gzclose(gz_write);
        remove(read_file);
        remove(write_file);
        return 0;
    }
    
    // Use first byte to determine which functions to call
    uint8_t selector = Size > 0 ? Data[0] : 0;
    
    // Test gzgets
    if (selector & 0x01) {
        char buffer[256];
        for (int i = 0; i < 3; i++) {
            char* result = gzgets(gz_read, buffer, sizeof(buffer));
            (void)result; // Result may be NULL, that's OK
        }
    }
    
    // Test zError
    if (selector & 0x02) {
        // Test with various error codes
        for (int err = -5; err <= 5; err++) {
            const char* err_str = zError(err);
            (void)err_str;
        }
    }
    
    // Test gzseek
    if (selector & 0x04) {
        // Try different seek operations
        z_off_t offsets[] = {0, 10, -5, 100};
        int whences[] = {SEEK_SET, SEEK_CUR, SEEK_END};
        
        for (size_t i = 0; i < sizeof(offsets)/sizeof(offsets[0]); i++) {
            for (size_t j = 0; j < sizeof(whences)/sizeof(whences[0]); j++) {
                z_off_t result = gzseek(gz_read, offsets[i], whences[j]);
                (void)result;
            }
        }
    }
    
    // Test gzprintf
    if (selector & 0x08 && Size > 10) {
        // Use some data for format string parameters
        int int_val = Size > 1 ? Data[1] : 0;
        const char* str_val = Size > 5 ? (const char*)&Data[5] : "test";
        
        gzprintf(gz_write, "Int: %d, Str: %s\n", int_val, str_val);
        gzprintf(gz_write, "Size: %zu\n", Size);
    }
    
    // Test gzungetc
    if (selector & 0x10 && Size > 2) {
        // Try pushing back different characters
        for (int i = 0; i < 5 && i < (int)Size; i++) {
            int c = Data[i % Size];
            int result = gzungetc(c, gz_read);
            (void)result;
        }
    }
    
    // Test gzerror
    if (selector & 0x20) {
        int errnum;
        const char* errstr;
        
        // Get error from read file
        errstr = gzerror(gz_read, &errnum);
        (void)errstr;
        (void)errnum;
        
        // Get error from write file
        errstr = gzerror(gz_write, &errnum);
        (void)errstr;
        (void)errnum;
        
        // Test with NULL errnum
        errstr = gzerror(gz_read, NULL);
        (void)errstr;
    }
    
    // Clean up
    gzclose(gz_read);
    gzclose(gz_write);
    
    // Remove temporary files
    remove(read_file);
    remove(write_file);
    
    return 0;
}