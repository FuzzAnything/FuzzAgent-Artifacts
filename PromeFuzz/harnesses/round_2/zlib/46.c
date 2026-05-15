// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
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
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file with fuzzer input
    create_dummy_file(Data, Size);
    
    // Test gzopen with various modes
    const char *modes[] = {"rb", "wb", "ab", "r", "w", "a"};
    int mode_idx = Data[0] % (sizeof(modes) / sizeof(modes[0]));
    
    gzFile file = gzopen("./dummy_file", modes[mode_idx]);
    if (!file) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Test gzerror
    int errnum = 0;
    const char *error_msg = gzerror(file, &errnum);
    
    // Test gzungetc with various characters
    if (Size > 1) {
        int c = Data[1] % 256;  // Ensure valid character (0-255)
        int unget_result = gzungetc(c, file);
    }
    
    // Test zError with various error codes
    if (Size > 2) {
        int error_code = Data[2];
        const char *zlib_error = zError(error_code);
    }
    
    // Test gzgets if file is opened for reading
    if (mode_idx % 3 == 0) {  // "rb" or "r" modes
        char buffer[256];
        if (Size > 3) {
            int len = Data[3] % 255 + 1;  // Ensure len between 1-255
            char *gets_result = gzgets(file, buffer, len);
        }
        
        // Try to read some data to potentially generate errors
        char read_buf[128];
        gzread(file, read_buf, sizeof(read_buf));
        
        // Test gzerror again after reading
        gzerror(file, &errnum);
    }
    
    // Test gzprintf if file is opened for writing/appending
    if (mode_idx % 3 != 0) {  // "wb", "ab", "w", or "a" modes
        if (Size > 4) {
            int value = Data[4];
            int printf_result = gzprintf(file, "Test value: %d\n", value);
            
            // Test gzerror after writing
            gzerror(file, &errnum);
        }
    }
    
    // Test zError with another random error code
    if (Size > 5) {
        int another_error = Data[5];
        const char *another_error_msg = zError(another_error);
    }
    
    // Try gzungetc again with different character
    if (Size > 6) {
        int c2 = Data[6] % 256;
        gzungetc(c2, file);
    }
    
    // Final gzerror check
    gzerror(file, &errnum);
    
    gzclose(file);
    cleanup_dummy_file();
    
    return 0;
}