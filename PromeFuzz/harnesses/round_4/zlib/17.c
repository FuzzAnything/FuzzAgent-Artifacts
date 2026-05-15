// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zlib.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    gzFile file = gzopen(filename, "rb");
    if (file == NULL) {
        remove(filename);
        return 0;
    }

    int direct_initial = gzdirect(file);
    
    int byte;
    int read_count = 0;
    while (read_count < 100 && (byte = gzgetc(file)) != -1) {
        read_count++;
        
        if (read_count % 10 == 0) {
            int eof_check = gzeof(file);
            int direct_check = gzdirect(file);
            (void)eof_check;
            (void)direct_check;
        }
        
        if (read_count == 50) {
            int rewind_result = gzrewind(file);
            if (rewind_result == 0) {
                read_count = 0;
            }
        }
    }

    int eof_final = gzeof(file);
    (void)eof_final;

    int close_result = gzclose_r(file);
    (void)close_result;

    remove(filename);
    return 0;
}