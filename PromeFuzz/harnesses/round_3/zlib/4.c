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
#include "code/zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // First gzclose (should fail with NULL file)
    gzclose(NULL);
    
    // gzopen the dummy file for reading
    gzFile file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        unlink("./dummy_file");
        return 0;
    }
    
    // gzread some data
    char read_buf[1024];
    int read_len = gzread(file, read_buf, sizeof(read_buf) - 1);
    
    // gzerror after read
    int errnum;
    const char *errmsg = gzerror(file, &errnum);
    
    // gzseek to beginning
    z_off_t seek_result = gzseek(file, 0L, SEEK_SET);
    
    // First gztell
    z_off_t tell1 = gztell(file);
    
    // Second gztell
    z_off_t tell2 = gztell(file);
    
    // gzgetc
    int got_char = gzgetc(file);
    
    // gzungetc (only if we got a valid character)
    if (got_char != -1) {
        gzungetc(got_char, file);
    }
    
    // gzgets
    char line_buf[256];
    char *line = gzgets(file, line_buf, sizeof(line_buf));
    
    // gzerror again
    errmsg = gzerror(file, &errnum);
    
    // Final gzclose
    int close_result = gzclose(file);
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}