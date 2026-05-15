// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create a dummy file with fuzzer data
    create_dummy_file(Data, Size);
    
    // Open the file for reading
    gzFile file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        remove_dummy_file();
        return 0;
    }
    
    // Test gzdirect
    int is_direct = gzdirect(file);
    (void)is_direct; // Use result to avoid unused variable warning
    
    // Test gzgetc_ multiple times
    int bytes_read = 0;
    int ch;
    while (bytes_read < 100 && (ch = gzgetc_(file)) != -1) {
        bytes_read++;
        
        // Occasionally test gzungetc
        if (bytes_read % 5 == 0 && ch != -1) {
            int unget_result = gzungetc(ch, file);
            (void)unget_result;
            // Read again
            ch = gzgetc_(file);
        }
        
        // Occasionally test gzeof
        if (bytes_read % 7 == 0) {
            int eof_status = gzeof(file);
            (void)eof_status;
        }
    }
    
    // Test gzrewind if we read some data
    if (bytes_read > 0) {
        int rewind_result = gzrewind(file);
        (void)rewind_result;
        
        // After rewind, read a few more bytes
        for (int i = 0; i < 10; i++) {
            ch = gzgetc_(file);
            if (ch == -1) break;
        }
    }
    
    // Test gzeof again
    int final_eof = gzeof(file);
    (void)final_eof;
    
    // Test gzdirect again
    int final_direct = gzdirect(file);
    (void)final_direct;
    
    // Close the file
    int close_result = gzclose_r(file);
    (void)close_result;
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}