// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
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

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer data
    create_dummy_file(Data, Size);
    
    // Test gzgets
    {
        gzFile file = gzopen("./dummy_file", "rb");
        if (file) {
            char buffer[1024];
            for (int i = 0; i < 5 && !gzeof(file); i++) {
                gzgets(file, buffer, sizeof(buffer));
            }
            gzclose(file);
        }
    }
    
    // Test gzseek
    {
        gzFile file = gzopen("./dummy_file", "rb");
        if (file) {
            // Try different seek operations
            z_off_t offset = Size > 8 ? *(const z_off_t*)Data : 0;
            int whence = Size > 12 ? Data[8] % 3 : 0;
            gzseek(file, offset, whence);
            
            // Try another seek with different parameters
            if (Size > 16) {
                gzseek(file, Data[9], SEEK_CUR);
            }
            gzclose(file);
        }
    }
    
    // Test gzerror
    {
        gzFile file = gzopen("./dummy_file", "rb");
        if (file) {
            int errnum = 0;
            const char *errstr = gzerror(file, &errnum);
            
            // Try to trigger errors
            char buffer[10];
            gzread(file, buffer, sizeof(buffer));
            errstr = gzerror(file, &errnum);
            
            gzclose(file);
        }
    }
    
    // Test gzungetc
    {
        gzFile file = gzopen("./dummy_file", "rb");
        if (file) {
            // Try to unget multiple characters
            for (int i = 0; i < 10 && i < Size; i++) {
                gzungetc(Data[i], file);
            }
            
            // Read some data
            char buffer[10];
            gzread(file, buffer, sizeof(buffer));
            
            gzclose(file);
        }
    }
    
    // Test gzprintf
    {
        gzFile file = gzopen("./dummy_file", "wb");
        if (file) {
            // Use different format strings
            if (Size > 20) {
                gzprintf(file, "Data: %.*s\n", (int)(Size > 100 ? 100 : Size), Data);
            }
            
            // Try with different format specifiers
            int int_val = Size > 4 ? *(const int*)Data : 0;
            gzprintf(file, "Integer: %d\n", int_val);
            
            gzclose(file);
        }
    }
    
    // Test zError
    {
        // Test with various error codes
        for (int i = -10; i < 20; i++) {
            const char *errstr = zError(i);
        }
        
        // Test with fuzzer-derived error codes
        if (Size > 0) {
            int errcode = Data[0];
            const char *errstr = zError(errcode);
        }
    }
    
    return 0;
}