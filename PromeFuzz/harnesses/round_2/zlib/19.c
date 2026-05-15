// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
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

#include "code/zlib.h"

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);
    
    // Open the file for reading with gzip
    gzFile file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Test gzdirect - check if file is being read directly or as gzip stream
    int direct = gzdirect(file);
    (void)direct; // Use result to avoid unused variable warning
    
    // Read some bytes to test various states
    int bytes_read = 0;
    int byte;
    while (bytes_read < 10 && (byte = gzgetc_(file)) != -1) {
        bytes_read++;
        
        // Occasionally test gzungetc
        if (bytes_read == 3 && byte != -1) {
            int unget_result = gzungetc(byte, file);
            (void)unget_result;
        }
        
        // Test gzeof periodically
        if (bytes_read % 4 == 0) {
            int eof = gzeof(file);
            (void)eof;
        }
    }
    
    // Test gzrewind if we've read some data
    if (bytes_read > 0) {
        int rewind_result = gzrewind(file);
        (void)rewind_result;
        
        // After rewinding, test gzeof again
        int eof_after_rewind = gzeof(file);
        (void)eof_after_rewind;
        
        // Read a byte after rewinding
        byte = gzgetc_(file);
        (void)byte;
    }
    
    // Test gzungetc with various characters if we have data
    if (Size > 0) {
        // Use first byte of input as character to push back
        int unget_char = Data[0] & 0xFF;
        if (unget_char != -1) {  // Can't push EOF
            int unget_result = gzungetc(unget_char, file);
            (void)unget_result;
        }
    }
    
    // Test gzeof one more time
    int final_eof = gzeof(file);
    (void)final_eof;
    
    // Close the file
    int close_result = gzclose_r(file);
    (void)close_result;
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}