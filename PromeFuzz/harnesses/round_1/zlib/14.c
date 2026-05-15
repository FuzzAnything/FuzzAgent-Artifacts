// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose_r at gzread.c:645:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        return 0;
    }
    
    /* Test gzdirect */
    int direct = gzdirect(file);
    (void)direct; /* Use variable to avoid unused warning */
    
    /* Test gzgetc_ */
    int byte = gzgetc_(file);
    
    /* Test gzungetc if we read a valid byte */
    if (byte != -1) {
        int unget_result = gzungetc(byte, file);
        (void)unget_result;
    }
    
    /* Test gzeof */
    int eof = gzeof(file);
    (void)eof;
    
    /* Test gzrewind */
    int rewind_result = gzrewind(file);
    (void)rewind_result;
    
    /* Read a few more bytes to change state */
    for (int i = 0; i < 10 && gzgetc_(file) != -1; i++) {}
    
    /* Test gzeof again */
    eof = gzeof(file);
    
    /* Test gzclose_r */
    int close_result = gzclose_r(file);
    (void)close_result;
    
    /* Clean up dummy file */
    remove("./dummy_file");
    
    return 0;
}