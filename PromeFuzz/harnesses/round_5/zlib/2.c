// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // First gzclose on NULL (error case)
    gzclose(NULL);
    
    // Open the dummy file for reading
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        // Clean up and return if open fails
        remove("./dummy_file");
        return 0;
    }
    
    // Buffer for reading operations
    char buffer[1024];
    int errnum = 0;
    
    // gzread - read up to 1024 bytes
    int read_bytes = gzread(file, buffer, sizeof(buffer));
    
    // gzerror - check for errors after gzread
    const char *error_msg = gzerror(file, &errnum);
    
    // gzseek - try to seek to beginning (if possible)
    z_off_t seek_result = gzseek(file, 0L, SEEK_SET);
    
    // gztell - get current position (first call)
    z_off_t tell_result1 = gztell(file);
    
    // gztell - get current position (second call)
    z_off_t tell_result2 = gztell(file);
    
    // gzgetc - read one character
    int ch = gzgetc(file);
    
    // gzungetc - push character back if we read one
    if (ch != -1) {
        gzungetc(ch, file);
    }
    
    // gzgets - read a line (up to 256 characters to avoid overflow)
    char line_buf[256];
    char *line_result = gzgets(file, line_buf, sizeof(line_buf));
    
    // gzerror - check for errors again
    error_msg = gzerror(file, &errnum);
    
    // Final gzclose
    int close_result = gzclose(file);
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}