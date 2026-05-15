// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_dummy_file(Data, Size);
    
    // First gzclose (should fail on NULL)
    gzclose(NULL);
    
    // gzopen
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        remove_dummy_file();
        return 0;
    }
    
    // gzread
    char buffer[4096];
    int bytes_read = gzread(file, buffer, sizeof(buffer));
    
    // gzerror
    int errnum;
    const char *errmsg = gzerror(file, &errnum);
    
    // gzseek - try different seek modes
    z_off_t seek_result;
    seek_result = gzseek(file, 0, SEEK_SET);
    seek_result = gzseek(file, 10, SEEK_CUR);
    if (bytes_read > 20) {
        seek_result = gzseek(file, -10, SEEK_CUR);
    }
    seek_result = gzseek(file, 0, SEEK_END);
    seek_result = gzseek(file, 0, SEEK_SET);
    
    // gztell (twice as specified)
    z_off_t tell_result1 = gztell(file);
    z_off_t tell_result2 = gztell(file);
    
    // gzgetc
    int got_char = gzgetc(file);
    
    // gzungetc (if we got a valid character)
    if (got_char != -1) {
        gzungetc(got_char, file);
    }
    
    // gzgets
    char line_buf[256];
    char *line_result = gzgets(file, line_buf, sizeof(line_buf));
    
    // gzerror again
    errmsg = gzerror(file, &errnum);
    
    // Final gzclose
    int close_result = gzclose(file);
    
    remove_dummy_file();
    return 0;
}